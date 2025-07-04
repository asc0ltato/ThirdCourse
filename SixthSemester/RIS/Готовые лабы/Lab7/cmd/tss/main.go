package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"sync"
	"time"

	"Lab7/internal/config"
)

const (
	MsgElection    = "ELECTION"
	MsgOK          = "OK"
	MsgCoordinator = "COORDINATOR"
	MsgTimeRequest = "TIME"
	MsgPing        = "PING" // Запрос
	MsgPong        = "PONG" // Ответ

	udpTimeout          = 2 * time.Second // Время ожидания ответов по протоколу UDP (OK, PONG)
	healthCheckInterval = 5 * time.Second // Как часто связываться с координатором
	electionGracePeriod = 1 * time.Second // Короткая задержка после попытки избрания
)

var (
	myAddr          string
	servers         []string
	coordinator     string
	isCoordinator   bool
	electionOngoing bool
	mu              sync.RWMutex // Мьютекс для защиты общего состояния (coordinator, isCoordinator, electionOngoing)
)

func main() {
	// Получает собственный адрес из переменной окружения, переданной Docker Compose
	myAddr = os.Getenv("MY_ADDR")
	if myAddr == "" {
		log.Fatal("Переменная окружения MY_ADDR не установлена")
	}
	log.Printf("[%s] Запуск сервера TSS...", myAddr)

	// Начальная загрузка конфигурации
	if err := loadConfig(); err != nil {
		log.Fatalf("[%s] Ошибка загрузки конфигурации: %v", myAddr, err)
	}

	// Первоначальная проверка координатора (алгоритм забияки)
	// Проверка, должен ли я быть координатором, основываясь на самом высоком адресе
	if isHighestAddress(myAddr, servers) {
		log.Printf("[%s] У меня наивысший адрес. Становлюсь координатором.", myAddr)
		becomeCoordinator()
		// Объявление об этом, если другие начали первыми, но оказались ниже
		announceCoordinator()
	} else {
		// Жив ли назначенный координатор
		go checkCoordinatorHealth()
	}

	// Запустить прослушиватель UDP
	addr, err := net.ResolveUDPAddr("udp", myAddr)
	if err != nil {
		log.Fatalf("[%s] Ошибка разрешения UDP адреса: %v", myAddr, err)
	}

	lconn, err := net.ListenUDP("udp", addr)
	if err != nil {
		log.Fatalf("[%s] Ошибка прослушивания UDP-сервера: %v", myAddr, err)
	}
	defer lconn.Close()
	log.Printf("[%s] Прослушивание на %s", myAddr, myAddr)

	buffer := make([]byte, 1024)
	for {
		n, remoteAddr, err := lconn.ReadFromUDP(buffer)
		if err != nil {
			log.Printf("[%s] Ошибка чтения UDP: %v", myAddr, err)
			continue
		}

		message := string(buffer[:n])
		log.Printf("[%s] Получено сообщение '%s' от %s", myAddr, message, remoteAddr)

		// Обрабатывание сообщения в отдельном, чтобы избежать блокировки прослушивателя
		go handleMessage(lconn, remoteAddr, message)
	}
}

func handleMessage(conn *net.UDPConn, remoteAddr *net.UDPAddr, message string) {
	mu.RLock() // Блокировка для считывания общего состояния
	currentIsCoordinator := isCoordinator
	currentElectionOngoing := electionOngoing
	mu.RUnlock()

	switch message {
	case MsgTimeRequest:
		if currentIsCoordinator {
			currentTime := time.Now().Format("02012006:15:04:05") // DDMMYYYY:hh:mm:ss
			_, err := conn.WriteToUDP([]byte(currentTime), remoteAddr)
			if err != nil {
				log.Printf("[%s] Ошибка при отправке time response на %s: %v", myAddr, remoteAddr, err)
			}
			log.Printf("[%s] Отправка time %s к %s", myAddr, currentTime, remoteAddr)
		} else {
			// Некоординаторы могут перенаправить или сообщить клиенту, кто является координатором
			// Они просто игнорируют запросы о времени, направленные непосредственно им
			log.Printf("[%s] Игнорирую time request от %s, так как я не являюсь координатором.", myAddr, remoteAddr)
		}
	case MsgElection:
		// Ответить отправителю "ОК"
		_, err := conn.WriteToUDP([]byte(MsgOK), remoteAddr)
		if err != nil {
			log.Printf("[%s] Не удалось отправить OK на %s: %v", myAddr, remoteAddr, err)
		}
		// Начать собственные выборы, если они еще не начались
		if !currentElectionOngoing {
			go startElection()
		}
	case MsgOK:
		// Получено разрешение от сервера с более высоким приоритетом во время голосования
		// Прекратите ждать результатов голосования, их получит сервер с более высоким приоритетом
		log.Printf("[%s]Получил OK от %s, прервав мою попытку избрания.", myAddr, remoteAddr)
		mu.Lock()
		electionOngoing = false // Разрешить запуск нового позже, если потребуется
		mu.Unlock()
	case MsgCoordinator:
		// Избран новый координатор.
		// Обновите локальное состояние и конфигурационный файл (хотя это должен сделать новый координатор)
		log.Printf("[%s] Получено сообщение COORDINATOR от %s. Обновлена информация о координаторе.", myAddr, remoteAddr)
		mu.Lock()
		electionOngoing = false
		isCoordinator = false // Предположим, что отправителем является новый координатор
		mu.Unlock()
		// Перезагрузка конфигурации, чтобы получить нового координатора
		if err := loadConfig(); err != nil {
			log.Printf("[%s] Не удалось перезагрузить конфигурацию после сообщения COORDINATOR: %v", myAddr, err)
		}
	case MsgPing:
		_, err := conn.WriteToUDP([]byte(MsgPong), remoteAddr)
		if err != nil {
			log.Printf("[%s] Не удалось отправить PONG в %s: %v", myAddr, remoteAddr, err)
		}
	default:
		log.Printf("[%s] Получено неизвестное сообщение '%s' от %s", myAddr, message, remoteAddr)
	}
}

// Считывает конфигурацию и обновляет глобальное состояние. Требуется блокировка записи
func loadConfig() error {
	cfg, err := config.ReadConfig()
	if err != nil {
		return fmt.Errorf("Не удалось прочитать конфигурацию: %w", err)
	}

	mu.Lock()
	servers = cfg.Servers
	coordinator = cfg.Coordinator
	isCoordinator = (myAddr == coordinator)
	log.Printf("[%s] Конфигурация загружена. Серверы: %v, Координатор: %s, являюсь ли я координатором: %t", myAddr, servers, coordinator, isCoordinator)
	mu.Unlock()
	return nil
}

// Проверяет, является ли данный адрес самым высоким в списке
func isHighestAddress(addr string, serverList []string) bool {
	for _, srv := range serverList {
		if srv > addr {
			return false
		}
	}
	return true
}

// Возвращает список серверов с адресами, превышающими myAddr
func getHigherServers() []string {
	mu.RLock()
	defer mu.RUnlock()
	higher := []string{}
	for _, srv := range servers {
		if srv > myAddr {
			higher = append(higher, srv)
		}
	}
	return higher
}

// Инициация алгоритма забияки
func startElection() {
	mu.Lock()
	if electionOngoing {
		log.Printf("[%s] Выборы уже начались.", myAddr)
		mu.Unlock()
		return
	}
	log.Printf("[%s] Начинаем выборы...", myAddr)
	electionOngoing = true
	mu.Unlock()

	// В конечном итоге ход выборов будет восстановлен
	defer func() {
		time.Sleep(electionGracePeriod) // Выделить время для сообщений COORDINATOR
		mu.Lock()
		electionOngoing = false
		mu.Unlock()
	}()

	higherServers := getHigherServers()
	if len(higherServers) == 0 {
		// Никаких вышестоящих серверов, я являюсь координатором
		log.Printf("[%s] Серверы более высокого уровня не найдены. Становлюсь координатором.", myAddr)
		becomeCoordinator()
		announceCoordinator()
		return
	}

	// Отправить сообщение ELECTION на все вышестоящие серверы и дождаться подтверждения
	okReceived := false
	var wg sync.WaitGroup
	resultChan := make(chan bool, len(higherServers))

	for _, higherSrv := range higherServers {
		wg.Add(1)
		go func(targetSrv string) {
			defer wg.Done()
			log.Printf("[%s] Отправка результатов ELECTION в %s", myAddr, targetSrv)
			response, err := sendUDPMessage(targetSrv, MsgElection, udpTimeout)
			if err != nil {
				log.Printf("[%s] Нет ответа от%s на запрос о ELECTION: %v", myAddr, targetSrv, err)
				return // Предположим, что он не работает или не ответил вовремя
			}
			if response == MsgOK {
				log.Printf("[%s] Получен OK от %s", myAddr, targetSrv)
				resultChan <- true
			}
		}(higherSrv)
	}

	// Дождаться завершения всех процедур
	go func() {
		wg.Wait()
		close(resultChan)
	}()

	// Проверить, было ли получен ОК
	for ok := range resultChan {
		if ok {
			okReceived = true
			break // Нет необходимости в дальнейшей проверке
		}
	}

	if !okReceived {
		// Ни с одного вышестоящего сервера не получено ОК, станьте координатором
		log.Printf("[%s] Не получено никаких ОК от вышестоящих серверов. Становлюсь координатором.", myAddr)
		becomeCoordinator()
		announceCoordinator()
	} else {
		// ОК получено, дождаться сообщения COORDINATOR
		// defer func() перезапустит процесс выбора по истечении grace периода
		log.Printf("[%s] Получено подтверждение от вышестоящего сервера. Ждем сообщения COORDINATOR.", myAddr)
	}
}

// Обновляет состояние и конфигурационный файл
func becomeCoordinator() {
	mu.Lock()
	if isCoordinator {
		mu.Unlock()
		return // Уже координатор
	}
	isCoordinator = true
	coordinator = myAddr
	log.Printf("[%s] Становлюсь координатором.", myAddr)
	mu.Unlock() // Разблокировать перед записью в файл

	err := config.SetCoordinator(myAddr)
	if err != nil {
		log.Printf("[%s] КРИТИЧНО: Не удалось обновить конфигурационный файл после того, как вы стали координатором: %v", myAddr, err)
		mu.Lock()
		isCoordinator = false // Восстановить состояние, если запись не удалась
		mu.Unlock()
	} else {
		log.Printf("[%s] Успешно обновлен конфигурационный файл. Я являюсь координатором.", myAddr)
	}
}

// Отправка сообщения COORDINATOR на все остальные серверы
func announceCoordinator() {
	mu.RLock()
	currentServers := make([]string, len(servers)) // Скопировать список заблокированных серверов
	copy(currentServers, servers)
	mu.RUnlock()

	log.Printf("[%s] Объявляю себя новым координатором на всех серверах.", myAddr)
	for _, srv := range currentServers {
		if srv == myAddr {
			continue
		}
		go func(targetSrv string) {
			// Отправить, не ожидая ответа (запустить и забыть)
			_, err := sendUDPMessage(targetSrv, MsgCoordinator, 0) // 0 timeout
			if err != nil {
				// Log error, but don't retry for simplicity
				log.Printf("[%s]Не удалось объявить статус координатора %s: %v", myAddr, targetSrv, err)
			}
		}(srv)
	}
}

// Периодическая проверка работоспособности координатора, отправляя ему ping
func checkCoordinatorHealth() {
	ticker := time.NewTicker(healthCheckInterval)
	defer ticker.Stop()

	for range ticker.C {
		mu.RLock()
		currentCoordinator := coordinator
		currentIsCoordinator := isCoordinator
		currentElectionOngoing := electionOngoing
		mu.RUnlock()

		if currentIsCoordinator || currentElectionOngoing {
			continue // Не проверять, являюсь ли я координатором или проходят выборы
		}

		if currentCoordinator == "" || currentCoordinator == myAddr {
			// Координатор не назначен, или это я (этого не должно произойти, если !currentIsCoordinator)
			// Начать выборы, если координатор неизвестен
			if currentCoordinator == "" {
				log.Printf("[%s] Координатор неизвестен. Начинаем выборы.", myAddr)
				go startElection()
			}
			continue
		}

		log.Printf("[%s] Pinging координатор %s", myAddr, currentCoordinator)
		response, err := sendUDPMessage(currentCoordinator, MsgPing, udpTimeout)
		if err != nil || response != MsgPong {
			log.Printf("[%s] Координатор %s не работает (ошибка: %v, ответ: %s). Начинаем выборы.", myAddr, currentCoordinator, err, response)
			// Сначала перезагрузить конфигурацию, чтобы убедиться, что у нас есть последний список серверов перед выборами
			if loadErr := loadConfig(); loadErr != nil {
				log.Printf("[%s] Не удалось перезагрузить конфигурацию перед выборами: %v", myAddr, loadErr)
			}
			go startElection()
		} else {
			log.Printf("[%s] Координатор %s жив (ответил PONG).", myAddr, currentCoordinator)
		}
	}
}

// Отправляет сообщение и ожидает ответа (если тайм-аут > 0).
func sendUDPMessage(targetAddr string, message string, timeout time.Duration) (string, error) {
	serverAddr, err := net.ResolveUDPAddr("udp", targetAddr)
	if err != nil {
		return "", fmt.Errorf("Не удалось определить целевой адрес %s: %w", targetAddr, err)
	}

	// Используем случайный локальный порт для отправки, указав нулевой локальный адрес
	conn, err := net.DialUDP("udp", nil, serverAddr)
	if err != nil {
		return "", fmt.Errorf("Не удалось достучаться до %s: %w", targetAddr, err)
	}
	defer conn.Close()

	_, err = conn.Write([]byte(message))
	if err != nil {
		return "", fmt.Errorf("Не удалось записать UDP-сообщение в %s: %w", targetAddr, err)
	}

	if timeout <= 0 {
		return "", nil // Fire and forget
	}

	// Дождаться ответа
	deadline := time.Now().Add(timeout)
	err = conn.SetReadDeadline(deadline)
	if err != nil {
		return "", fmt.Errorf("Не удалось установить крайний срок чтения: %w", err)
	}

	buffer := make([]byte, 1024)
	n, _, err := conn.ReadFromUDP(buffer)
	if err != nil {
		// При необходимости отличайте ошибку тайм-аута от других ошибок
		if netErr, ok := err.(net.Error); ok && netErr.Timeout() {
			return "", fmt.Errorf("Тайм-аут на чтение UDP-ответа от %s: %w", targetAddr, err)
		}
		return "", fmt.Errorf("Не удалось прочитать UDP-ответ от %s: %w", targetAddr, err)
	}

	return string(buffer[:n]), nil
}
