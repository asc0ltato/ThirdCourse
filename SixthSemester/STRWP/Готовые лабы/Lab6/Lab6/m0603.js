const nodemailer = require("nodemailer");
const gmail = 'superpando4ka@gmail.com';
const password = 'babq xoxl lxlb ueiv';

const transporter = nodemailer.createTransport({
    host: 'smtp.gmail.com',
    port: 587,
    secure: false,
    auth: {
        user: gmail,
        pass: password
    }
});

function send(email) {
    transporter.sendMail({
        from: gmail,
        to: 'eva5562@mail.ru',
        subject: '06-03',
        text: email
    }, (smtpErr, info) => {
        if (smtpErr) {
            console.error('Ошибка отправки через nodemailer:', smtpErr);
        } else {
            console.log('Сообщение успешно отправлено!');
        }
    });
    return email.toString();
}

exports.send = send;