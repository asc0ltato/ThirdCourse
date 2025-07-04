class Service {
    //Симуляция успешной авторизации
    static async loginUser(user) {
        //Возвращает токен через 1 сек
        return new Promise((resolve) => {
            setTimeout(() => {
                resolve('7Gsq$q');
            }, 1000);
        });
    }
}

module.exports = { Service };
