package config

import (
	"encoding/json"
	"os"
	"sync"
)

// Config определяет структуру для общей конфигурации.
type Config struct {
	Servers     []string `json:"servers"`
	Coordinator string   `json:"coordinator"`
}

var (
	configPath = "/app/config/servers.json" // Путь внутри контейнера
	mu         sync.RWMutex
)

// ReadConfig считывает конфигурацию из общего файла.
func ReadConfig() (*Config, error) {
	mu.RLock()
	defer mu.RUnlock()

	data, err := os.ReadFile(configPath)
	if err != nil {
		// If the file doesn't exist, maybe create a default one or return error
		// For simplicity, we assume it exists after initial setup.
		return nil, err
	}

	var cfg Config
	err = json.Unmarshal(data, &cfg)
	if err != nil {
		return nil, err
	}
	return &cfg, nil
}

// WriteConfig записывает обновленную конфигурацию в общий файл.
func WriteConfig(cfg *Config) error {
	mu.Lock()
	defer mu.Unlock()

	data, err := json.MarshalIndent(cfg, "", "  ")
	if err != nil {
		return err
	}

	// Запись с разрешениями, позволяющими группе /другим пользователям читать
	return os.WriteFile(configPath, data, 0664)
}

// SetCoordinator обновляет только поле coordinator в конфигурационном файле.
func SetCoordinator(newCoordinator string) error {
	cfg, err := ReadConfig()
	if err != nil {
		// Handle case where config doesn't exist yet? For now, assume it does.
		return err
	}

	cfg.Coordinator = newCoordinator
	return WriteConfig(cfg)
}
