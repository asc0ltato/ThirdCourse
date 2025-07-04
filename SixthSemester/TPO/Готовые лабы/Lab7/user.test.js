const { User } = require('./user');

//Подключение всех модулей через safeRequire с заглушками
const { safeRequire } = require('./safe-require');

//Заглушки
const { GUI } = safeRequire('./gui');
const { Service } = safeRequire('./service');
const { Database } = safeRequire('./database');

//Моки. Переопределяем реальные модули и методы
jest.mock('./service', () => ({
    Service: {
        loginUser: jest.fn().mockResolvedValue('7Gsq$q')
    }
}), { virtual: true });

jest.mock('./database', () => ({
    Database: {
        writePost: jest.fn().mockResolvedValue('Пост создан успешно')
    }
}), { virtual: true });

jest.mock('./gui', () => ({
    GUI: class {
        getInputForm() {
            return { username: 'svetlanchik', email: 'svetlanchik@gmail.com' };
        }
    }
}), { virtual: true });

describe('User module - unit tests', () => {
    let user;
    let gui;

    //Инициализация User перед каждым тестом
    beforeEach(() => {
        gui = new GUI();
        const input = gui.getInputForm();
        user = new User(input.username, input.email, Service, Database);
    });

    it('Создание пользователя с валидными данными', () => {
        expect(user.username).toBe('svetlanchik');
        expect(user.email).toBe('svetlanchik@gmail.com');
        expect(user.token).toBe('');
    });

    test('Логин с валидным токеном', async () => {
        await user.login();
        expect(user.token).toBe('7Gsq$q');
    });

    test('Ошибка при логине (сервис падает)', async () => {
        const brokenService = {
            loginUser: jest.fn().mockRejectedValue(new Error('Ошибка авторизации'))
        };
        user = new User('x', 'y@mail.com', brokenService, Database);
        await expect(user.login()).rejects.toThrow('Ошибка авторизации');
    });

    test('Успешная запись поста', async () => {
        await expect(user.writePost()).resolves.toBe('Пост создан успешно');
    });

    test('Ошибка при записи поста (БД недоступна)', async () => {
        const brokenDb = { writePost: jest.fn().mockRejectedValue(new Error('Ошибка базы данных')) };
        user = new User('x', 'x@mail.com', Service, brokenDb);
        await expect(user.writePost()).rejects.toThrow('Ошибка базы данных');
    });

    test('Попытка логина без сервиса авторизации', async () => {
        const userNoService = new User('x', 'x@mail.com', null, Database);
        await expect(userNoService.login()).rejects.toThrow('Сервис авторизации недоступен');
    });

    test('Тест с отсутствующей БД', async () => {
        const userNoDb = new User('x', 'x@mail.com', Service, null);
        await expect(userNoDb.writePost()).rejects.toThrow('База данных недоступна');
    });

    test('Логин возвращает токен', async () => {
        const fakeService = {
            loginUser: jest.fn().mockResolvedValue('fake123')
        };
        const newUser = new User('user1', 'user1@mail.com', fakeService, Database);
        await newUser.login();
        expect(newUser.token).toBe('fake123');
    });

    test('База данных возвращает кастомный результат', async () => {
        const fakeDb = {
            writePost: jest.fn().mockResolvedValue('Пост успешно записан')
        };
        const newUser = new User('user2', 'user2@mail.com', Service, fakeDb);
        const result = await newUser.writePost();
        expect(result).toBe('Пост успешно записан');
    });

    test('GUI форма (fallback)', () => {
        const { username, email } = new GUI().getInputForm();
        expect(username).toBeDefined();
        expect(email).toMatch(/@/);
    });
});