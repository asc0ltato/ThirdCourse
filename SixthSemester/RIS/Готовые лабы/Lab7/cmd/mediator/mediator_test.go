package main

import (
	"net"
	"testing"
	"time"
)

func TestMediator(t *testing.T) {
	t.Parallel()

	conn, err := net.DialUDP("udp4", nil, &net.UDPAddr{
		IP:   net.IPv4(127, 0, 0, 1),
		Port: 5555,
	})
	if err != nil {
		t.Fatalf("Не удалось подключиться к UDP-серверу: %v", err)
	}
	defer conn.Close()

	message := []byte("TIME")
	t.Logf("Отправка сообщения: %s", string(message))
	_, err = conn.Write(message)
	if err != nil {
		t.Fatalf("Не удалось отправить сообщение на UDP-сервер: %v", err)
	}

	deadline := time.Now().Add(5 * time.Second)
	err = conn.SetReadDeadline(deadline)
	if err != nil {
		t.Fatalf("Не удалось установить таймаут чтения: %v", err)
	}

	buffer := make([]byte, 1024)
	t.Log("Ожидание ответа от сервера...")
	n, err := conn.Read(buffer)
	if err != nil {
		if netErr, ok := err.(net.Error); ok && netErr.Timeout() {
			t.Fatalf("Таймаут при ожидании ответа от UDP-сервера: %v", err)
		}
		t.Fatalf("Ошибка при чтении ответа от UDP-сервера: %v", err)
	}

	t.Logf("Получен ответ: %s", string(buffer[:n]))

	if n == 0 {
		t.Error("Получен пустой ответ от посредника")
	}
}
