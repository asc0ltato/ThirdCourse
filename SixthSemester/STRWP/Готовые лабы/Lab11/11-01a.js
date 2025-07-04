let WebSocket = require('ws');
let fs = require('fs');

let ws = new WebSocket('ws://localhost:4000');

ws.on('open', () => {
    console.log('Connected to server');

    let duplex = WebSocket.createWebSocketStream(ws, {encoding: 'utf8'});
    let rfile = fs.createReadStream(`./upload/MyFile.txt`);
    rfile.pipe(duplex);

    rfile.on('end', () => {
        console.log('File send successfully');
    });
})

ws.on('error', (e) => {
    console.log(`Error: ${e}`);
});

setTimeout(() => {
    console.log('Client connection closing...');
    ws.close();
}, 25000);