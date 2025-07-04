//Пользователь с возможностью логина и создания поста
class User {
    constructor(username, email, service, database) {
        this.username = username;
        this.email = email;
        this.token = ''; //Изначально токен пуст
        this.service = service; //сервис авторизации (Service)
        this.database = database; //сервис базы данных (Database)
    }

    //Метод логина. Получает токен через вызов loginUser() из сервиса
    async login() {
        if (!this.service?.loginUser) {
            throw new Error('Сервис авторизации недоступен'); //Проверка доступности метода
        }
        this.token = await this.service.loginUser(this); //Установка токена после авторизации
    }

    //Метод записи поста. Получает ошибку через вызов writePost() из бд
    async writePost() {
        if (!this.database?.writePost) {
            throw new Error('База данных недоступна'); //Проверка доступности метода
        }
        return await this.database.writePost(this); //Возвращаем результат
    }
}

module.exports = { User };