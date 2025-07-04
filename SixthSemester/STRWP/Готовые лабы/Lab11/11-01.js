let WebSocket = require('ws');
let fs = require('fs');
const path = require('path');

let wss = new WebSocket.Server({port: 4000, host: 'localhost'});

let fileCounter = 0;

wss.on('connection', (ws) => {
    console.log('Client connected');

    // Сообщение от сервера -> stdout, от клиента stdin
    let duplex = WebSocket.createWebSocketStream(ws, {encoding: 'utf8'});
    const filePath = path.join(__dirname, 'upload', `file${++fileCounter}.txt`);
    const wfile = fs.createWriteStream(filePath);
    duplex.pipe(wfile);

    wfile.on('finish', () => {
        console.log(`File saved as ${filePath}`);
    });
})

wss.on('error', (e) => {
    console.log(`Server error: ${e}`);
});

setTimeout(() => {
    console.log('Server closing...');
    wss.close();
}, 25000);