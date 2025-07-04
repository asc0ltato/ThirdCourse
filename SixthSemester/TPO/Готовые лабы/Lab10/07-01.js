const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');
const stat = require('./m07-01')('/static');

const PORT = 5000;

// Хранилище пользователей (память)
let users = [
    { id: 1, name: "Ivan Ivanov", email: "ivan@example.com", birthday: "1990-01-01" },
    { id: 2, name: "Maria Petrova", email: "maria@example.com", birthday: "1992-05-12" }
];

// Функция для чтения тела POST/PUT запроса
function getRequestBody(req) {
    return new Promise((resolve, reject) => {
        let body = '';
        req.on('data', chunk => { body += chunk.toString(); });
        req.on('end', () => {
            try {
                resolve(JSON.parse(body));
            } catch (e) {
                reject(e);
            }
        });
    });
}

// Валидация данных пользователя
function validateUserData(user, isUpdate = false) {
    const errors = [];

    if (!isUpdate || ('name' in user)) {
        if (!user.name || typeof user.name !== 'string' || user.name.length > 50) {
            errors.push("Invalid 'name': required string max length 50");
        }
    }

    if (!isUpdate || ('email' in user)) {
        if (!user.email || typeof user.email !== 'string' || user.email.length > 100 || !user.email.includes('@')) {
            errors.push("Invalid 'email': required valid email string max length 100");
        }
    }

    if (!isUpdate || ('birthday' in user)) {
        // Простая проверка формата YYYY-MM-DD
        if (!user.birthday || !/^\d{4}-\d{2}-\d{2}$/.test(user.birthday)) {
            errors.push("Invalid 'birthday': required format YYYY-MM-DD");
        }
    }

    if (isUpdate && ('id' in user)) {
        if (typeof user.id !== 'number' || user.id <= 0) {
            errors.push("Invalid 'id': positive number required");
        }
    }

    return errors;
}

http.createServer(async function(req, resp) {
    const parsedURL = url.parse(req.url, true);
    const pathName = parsedURL.pathname;
    const method = req.method;

    console.log(`Received request: ${method} ${pathName}`);

    // --- API /api/users ---
    if (pathName.startsWith('/api/users')) {
        resp.setHeader('Content-Type', 'application/json; charset=utf-8');

        try {
            if (method === 'GET') {
                const id = parsedURL.query.id ? Number(parsedURL.query.id) : null;
                const page = parsedURL.query.page ? Number(parsedURL.query.page) : null;
                const limit = parsedURL.query.limit ? Number(parsedURL.query.limit) : null;

                if (id) {
                    const user = users.find(u => u.id === id);
                    if (user) {
                        resp.writeHead(200);
                        resp.end(JSON.stringify(user));
                    } else {
                        resp.writeHead(404);
                        resp.end(JSON.stringify({ error: "User not found" }));
                    }
                } else if (page !== null && limit !== null) {
                    // Проверка валидности параметров пагинации
                    if (!Number.isInteger(page) || page < 1 || !Number.isInteger(limit) || limit < 1) {
                        resp.writeHead(400);
                        resp.end(JSON.stringify({ error: "Invalid pagination parameters" }));
                        return;
                    }
                    const startIndex = (page - 1) * limit;
                    if (startIndex >= users.length) {
                        // Если запрашиваемая страница выходит за пределы, возвращаем ошибку 404
                        resp.writeHead(404);
                        resp.end(JSON.stringify({ error: "Page not found" }));
                        return;
                    }
                    const paginatedUsers = users.slice(startIndex, startIndex + limit);
                    resp.writeHead(200);
                    resp.end(JSON.stringify(paginatedUsers));
                } else {
                    // Вернуть всех пользователей без пагинации
                    resp.writeHead(200);
                    resp.end(JSON.stringify(users));
                }
            }
            else if (method === 'POST') {
                const newUser = await getRequestBody(req); // Получаем тело запроса (новый пользователь)
                const errors = validateUserData(newUser, false); // Валидация данных
                if (errors.length > 0) {
                    resp.writeHead(400);
                    resp.end(JSON.stringify({ error: "Validation error", details: errors }));
                    return;
                }
                newUser.id = users.length ? Math.max(...users.map(u => u.id)) + 1 : 1;
                users.push(newUser);
                resp.writeHead(201); // Код 201 — создано
                resp.end(JSON.stringify(newUser));
            }
            else if (method === 'PUT') {
                const updatedUser = await getRequestBody(req);
                if (!updatedUser.id) {
                    resp.writeHead(400);
                    resp.end(JSON.stringify({ error: "Missing user id" }));
                    return;
                }
                const errors = validateUserData(updatedUser, true);
                if (errors.length > 0) {
                    resp.writeHead(400);
                    resp.end(JSON.stringify({ error: "Validation error", details: errors }));
                    return;
                }
                const userIndex = users.findIndex(u => u.id === updatedUser.id);
                if (userIndex === -1) {
                    resp.writeHead(404);
                    resp.end(JSON.stringify({ error: "User not found" }));
                    return;
                }
                // Обновляем поля пользователя, сохраняя остальные данные
                users[userIndex] = { ...users[userIndex], ...updatedUser };
                resp.writeHead(200);
                resp.end(JSON.stringify(users[userIndex]));
            }
            else if (method === 'DELETE') {
                const id = parsedURL.query.id ? Number(parsedURL.query.id) : null;
                if (!id) {
                    resp.writeHead(400);
                    resp.end(JSON.stringify({ error: "Missing user id" }));
                    return;
                }
                const userIndex = users.findIndex(u => u.id === id);
                if (userIndex === -1) {
                    resp.writeHead(404);
                    resp.end(JSON.stringify({ error: "User not found" }));
                    return;
                }
                users.splice(userIndex, 1);
                resp.writeHead(204); // Код 204 — без содержимого
                resp.end();
            }
            else {
                // Код 405 - метод не поддерживается
                resp.writeHead(405);
                resp.end(JSON.stringify({ error: "Method not allowed" }));
            }
        }
        catch (e) {
            resp.writeHead(400);
            resp.end(JSON.stringify({ error: "Invalid JSON" }));
        }
        return;
    }

    // Обработка статики и прочих запросов — только GET
    if (method !== 'GET') {
        resp.statusCode = 405;
        resp.statusMessage = 'Method not supported';
        resp.end("Method not supported");
        return;
    }

    if (pathName === '/') {
        fs.readFile(path.join(__dirname, './static/index.html'), (err, html) => {
            if (err) {
                resp.writeHead(500, 'Error', { 'Content-Type': 'text/html; charset=utf-8' });
                resp.end('Server error');
            } else {
                resp.writeHead(200, 'OK', { 'Content-Type': 'text/html' });
                resp.end(html);
            }
        });
    } else {
        if (stat.isStatic('css', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'text/css' });
        else if (stat.isStatic('js', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'text/javascript; charset=utf-8' });
        else if (stat.isStatic('json', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'application/json; charset=utf-8' });
        else if (stat.isStatic('xml', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'application/xml; charset=utf-8' });
        else if (stat.isStatic('mp4', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'video/mp4' });
        else if (stat.isStatic('docx', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'application/msword' });
        else if (stat.isStatic('png', req.url)) stat.sendFile(req, resp, { 'Content-Type': 'image/png' });
        else stat.writeHTTP404(resp);
    }
}).listen(PORT, () => {
    console.log(`Server listening on port ${PORT}`);
});
