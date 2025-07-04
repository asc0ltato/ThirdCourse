const http = require('http');
const fs = require('fs');

const fileName = './client.png'; // Файл на клиенте для записи
const file = fs.createWriteStream(fileName);

const options = {
    host: 'localhost',
    path: `/09-08`,
    port: 5000,
    method: 'GET'
}

const request = http.request(options, (response) => {
    console.log(`Response statusCode: ${response.statusCode}`);

    response.pipe(file); // Записываем файл, полученный от сервера

    response.on('end', () => {
        console.log(`Response file ${fileName} received`);
    });
});

request.on('error', (err) => {
    console.log(`Request error: ${err.message}`);
});

request.end();