const http = require('http');
const querystring = require('querystring');

//x=1.5&y=3&s=Svetlana
const postData = querystring.stringify({
    x: 1.5,
    y: 3,
    s: 'Svetlana'
});

const options = {
    host: 'localhost',
    path: `/09-03`,
    port: 5000,
    method: 'POST'
}

const request = http.request(options, (response) => {
    let data = '';
    console.log(`Response statusCode: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        console.log(`Response data: ${data}`);
    });
});

request.on('error', (err) => {
    console.log(`Request error: ${err.message}`);
});

request.write(postData); // Отправляем тело запроса
request.end();