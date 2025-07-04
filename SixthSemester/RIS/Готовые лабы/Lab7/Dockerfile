FROM golang:1.24-alpine AS builder

WORKDIR /app

# Копирование файлов модуля go
COPY go.mod go.sum* ./
# Зависимости от загрузки
RUN go mod download

# Копия исходного кода
COPY . .

# Создание TSS server
RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-s -w" -o /app/bin/tss ./cmd/tss

# Создание Mediator server
RUN CGO_ENABLED=0 GOOS=linux go build -ldflags="-s -w" -o /app/bin/mediator ./cmd/mediator

# Минимальное изображение без дистрибутива для заключительного этапа
FROM gcr.io/distroless/static-debian11

WORKDIR /app

# Копия созданных двоичных файлов со стадии создания
COPY --from=builder /app/bin/tss /app/tss
COPY --from=builder /app/bin/mediator /app/mediator

# Порт, используемый серверами и посредником
EXPOSE 5555/udp