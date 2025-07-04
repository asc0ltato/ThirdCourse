//Функция безопасного подключения всех модулей
//Если модуль не найден, возвращает объект-заглушку (fallback),
// чтобы избежать ошибок при require
function safeRequire(modulePath, fallback) {
    try {
        return require(modulePath);
    } catch {
        return fallback;
    }
}

module.exports = { safeRequire };