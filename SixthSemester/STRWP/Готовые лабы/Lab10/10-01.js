const http = require('http');
const fs = require('fs');
const {Server} = require('ws');

http.createServer((request, response) => {
    if (request.url === '/start' && request.method === 'GET') {
        response.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
        fs.createReadStream('10-01.html').pipe(response);
    } else {
        response.writeHead(400, { 'Content-Type': 'text/plain; charset=utf-8' });
        response.end('Bad Request');
    }
}).listen(3000, () => {
    console.log('HTTP-server is running on http://localhost:3000/start');
});

const ws = new Server({ port: 4000, host: 'localhost', path: `/ws` });

ws.on('connection', (ws) => {
    console.log('New client connected');
    let k = 0, n = 0;

    ws.on('message', (msg) => {
        console.log(`10-01-client: ${msg}`);
        n = msg;
    })

    const messageInterval = setInterval(() => {
        if (ws.readyState === ws.OPEN) {
            ws.send(`10-01-server: ${n} -> ${++k}`);
        }
    }, 5000);

    ws.on('close', () => {
        clearInterval(messageInterval);
        console.log('Client disconnected (WS connection closed by client)');
    });
})

ws.on('error', (err) => {
    console.log(`WS error: ${err}`)
})

console.log(`WS-server is running on ws://${ws.options.host}:${ws.options.port}${ws.options.path}`);