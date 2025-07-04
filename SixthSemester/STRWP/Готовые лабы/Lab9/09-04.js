const http = require('http');

const jsonObject = JSON.stringify({
    "comment": "Запрос.Лабораторная работа 8/10",
    "x": 1,
    "y": 2,
    "s": "Сообщение",
    "m": ["a", "b", "c", "d"],
    "o": {"surname": "Иванов", "name": "Иван"}
});

const options = {
    host: 'localhost',
    path: `/09-04`,
    port: 5000,
    method: 'POST',
    headers: {
        'Accept': 'application/json; charset=utf-8'
    }
}

const request = http.request(options, (response) => {
    let data = '';
    console.log(`Response statusCode: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        const result = JSON.parse(data);
        console.log(`Response data: ${JSON.stringify(result, null, 4)}`);
    });
});

request.on('error', (err) => {
    console.log(`Request error: ${err.message}`);
});

request.write(jsonObject);
request.end();