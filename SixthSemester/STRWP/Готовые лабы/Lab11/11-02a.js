let WebSocket = require('ws');
let fs = require('fs');
const path = require('path');

let ws = new WebSocket('ws://localhost:4000');

let fileCounter = 0;

ws.on('open', () => {
    console.log('Connected to server');

    // Сообщение от сервера -> stdout, от клиента stdin
    const duplex = WebSocket.createWebSocketStream(ws, { encoding: 'utf8' });
    const filePath = path.join(__dirname, '', `file${++fileCounter}.txt`);
    const wfile = fs.createWriteStream(filePath);
    duplex.pipe(wfile);

    wfile.on('finish', () => {
        console.log(`File saved as ${filePath}`);
    });
})

ws.on('error', (e) => {
    console.log(`Server error: ${e}`);
});

setTimeout(() => {
    console.log('Server closing...');
    ws.close();
}, 25000);