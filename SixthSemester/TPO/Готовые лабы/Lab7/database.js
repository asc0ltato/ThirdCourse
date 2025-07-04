class Database {
    //Симуляция ошибки подключения к БД
    static async writePost(user) {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                reject(new Error('База данных недоступна'));
            }, 1000);
        });
    }
}

module.exports = { Database };