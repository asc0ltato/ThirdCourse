package main

import (
	"fmt"
	"log"
	"net"
	"time"

	"Lab7/internal/config"
)

const (
	listenAddr     = "0.0.0.0:5555" // Посредник прослушивает все интерфейсы
	MsgTimeRequest = "TIME"         // Тип сообщения для запроса времени от клиента
)

func main() {
	log.Println("[Mediator] Запуск сервера-посредника...")

	addr, err := net.ResolveUDPAddr("udp", listenAddr)
	if err != nil {
		log.Fatalf("[Mediator] Ошибка при разрешении UDP-адреса: %v", err)
	}

	lconn, err := net.ListenUDP("udp", addr)
	if err != nil {
		log.Fatalf("[Mediator] Не удалось начать прослушивание UDP: %v", err)
	}
	defer lconn.Close()
	log.Printf("[Mediator] Ожидание сообщений на %s", listenAddr)

	buffer := make([]byte, 1024)
	for {
		n, clientAddr, err := lconn.ReadFromUDP(buffer)
		if err != nil {
			log.Printf("[Mediator] Ошибка при чтении из UDP: %v", err)
			continue
		}

		message := string(buffer[:n])
		log.Printf("[Mediator] Получено сообщение '%s' от клиента %s", message, clientAddr)

		// Обработка запроса времени
		if message == MsgTimeRequest {
			go handleClientRequest(lconn, clientAddr)
		} else {
			log.Printf("[Mediator] Игнорируется запрос, отличный от TIME, от клиента %s", clientAddr)
		}
	}
}

func handleClientRequest(conn *net.UDPConn, clientAddr *net.UDPAddr) {
	// 1. Чтение текущего координатора из конфигурационного файла
	cfg, err := config.ReadConfig()
	if err != nil {
		log.Printf("[Mediator] Ошибка при чтении конфигурации для клиента %s: %v", clientAddr, err)
		sendErrorToClient(conn, clientAddr, "Не удалось прочитать конфигурацию координатора")
		return
	}
	coordinatorAddr := cfg.Coordinator
	if coordinatorAddr == "" {
		log.Printf("[Mediator] В конфигурации не указан координатор для клиента %s", clientAddr)
		sendErrorToClient(conn, clientAddr, "Координатор не задан")
		return
	}

	log.Printf("[Mediator] Пересылаю запрос TIME от клиента %s координатору %s", clientAddr, coordinatorAddr)

	// 2. Отправка запроса координатору
	coordUDPAddr, err := net.ResolveUDPAddr("udp", coordinatorAddr)
	if err != nil {
		log.Printf("[Mediator] Ошибка при разрешении адреса координатора %s: %v", coordinatorAddr, err)
		sendErrorToClient(conn, clientAddr, "Не удалось разрешить адрес координатора")
		return
	}

	remoteConn, err := net.DialUDP("udp", nil, coordUDPAddr)
	if err != nil {
		log.Printf("[Mediator] Ошибка при подключении к координатору %s: %v", coordinatorAddr, err)
		sendErrorToClient(conn, clientAddr, "Не удалось подключиться к координатору")
		return
	}
	defer remoteConn.Close()

	_, err = remoteConn.Write([]byte(MsgTimeRequest))
	if err != nil {
		log.Printf("[Mediator] Ошибка при отправке запроса TIME координатору %s: %v", coordinatorAddr, err)
		sendErrorToClient(conn, clientAddr, "Не удалось отправить запрос координатору")
		return
	}

	// 3. Ожидание ответа от координатора с таймаутом
	buffer := make([]byte, 1024)
	deadline := time.Now().Add(3 * time.Second) // Таймаут 3 секунды
	err = remoteConn.SetReadDeadline(deadline)
	if err != nil {
		log.Printf("[Mediator] Ошибка при установке таймаута чтения для координатора %s: %v", coordinatorAddr, err)
		sendErrorToClient(conn, clientAddr, "Внутренняя ошибка при установке таймаута")
		return
	}

	n, _, err := remoteConn.ReadFromUDP(buffer)
	if err != nil {
		log.Printf("[Mediator] Ошибка при получении ответа от координатора %s: %v", coordinatorAddr, err)
		sendErrorToClient(conn, clientAddr, fmt.Sprintf("Координатор %s не ответил", coordinatorAddr))
		return
	}

	coordinatorResponse := string(buffer[:n])
	log.Printf("[Mediator] Получен ответ '%s' от координатора %s", coordinatorResponse, coordinatorAddr)

	// 4. Отправка ответа клиенту с информацией о координаторе
	finalResponse := fmt.Sprintf("Время: %s (от %s)", coordinatorResponse, coordinatorAddr)
	_, err = conn.WriteToUDP([]byte(finalResponse), clientAddr)
	if err != nil {
		log.Printf("[Mediator] Ошибка при отправке ответа клиенту %s: %v", clientAddr, err)
	}
	log.Printf("[Mediator] Ответ '%s' отправлен клиенту %s", finalResponse, clientAddr)
}

func sendErrorToClient(conn *net.UDPConn, clientAddr *net.UDPAddr, errorMsg string) {
	response := fmt.Sprintf("Ошибка: %s", errorMsg)
	_, err := conn.WriteToUDP([]byte(response), clientAddr)
	if err != nil {
		log.Printf("[Mediator] Не удалось отправить сообщение об ошибке '%s' клиенту %s: %v", errorMsg, clientAddr, err)
	}
}
