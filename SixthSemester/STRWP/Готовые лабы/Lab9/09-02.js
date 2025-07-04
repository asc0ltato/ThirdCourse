const http = require('http');

const x = 1.5;
const y = 3;

const options = {
    host: 'localhost',
    path: `/09-02?x=${x}&y=${y}`,
    port: 5000,
    method: 'GET'
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

request.end();