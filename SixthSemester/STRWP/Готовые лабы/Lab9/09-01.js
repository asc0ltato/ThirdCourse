const http = require('http');

const options = {
    host: 'localhost',
    path: '/09-01',
    port: 5000,
    method: 'GET'
}

const request = http.request(options, (response) => {
    let data = '';
    console.log(`Response statusCode: ${response.statusCode}`);
    console.log(`Response statusMessage: ${response.statusMessage}`);
    console.log(`Response server IP: ${response.socket.remoteAddress}`);
    console.log(`Response server Port: ${response.socket.remotePort}`);

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