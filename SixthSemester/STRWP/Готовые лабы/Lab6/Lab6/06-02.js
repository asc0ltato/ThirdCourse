const http = require('http');
const nodemailer = require('nodemailer');
const {parse} = require('querystring')
const smptTransport =require('nodemailer-smtp-transport');
const fs = require('fs');
const url = require('url');

const transporter = nodemailer.createTransport(smptTransport({
    host: 'smtp.gmail.com',
    port: 587,
    secure: false,
    auth: {
        user: 'superpando4ka@gmail.com',
        pass: 'babq xoxl lxlb ueiv'
    }
}));

http.createServer((request, response) => {
    const pathname = url.parse(request.url).pathname;
    response.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });

    if (pathname === '/' && request.method === 'GET') {
        fs.readFile('./index.html', (err, data) => {
            if (err) {
                console.error('Форма не загрузилась:', err);
            } else {
                response.end(data);
            }
        });
    } else if (pathname === '/' && request.method === 'POST') {
        let body = '';
        request.on('data', chunk => {
            body += chunk.toString();
        });
        request.on('end', () => {
            const {from, to, message} = parse(body);

            if (!from || !to || !message) {
                response.end('<h1>Все поля должны быть заполнены!</h1>');
                return;
            }

            transporter.sendMail({
                from: 'superpando4ka@gmail.com',
                to: to,
                subject: '06-02',
                text: message
            }, (smtpErr) => {
                if (smtpErr) {
                    response.end('<h1>Ошибка отправки сообщения. Попробуйте снова позже.</h1>');
                    console.error('Ошибка отправки через nodemailer:', smtpErr);
                } else {
                    response.end('<h1>Сообщение успешно отправлено!</h1>');
                }
            });
        });
    } else {
        response.end('<h1>Не поддерживается</h1>');
    }
}).listen(5001, () => console.log('Server running at http://localhost:5001/'));