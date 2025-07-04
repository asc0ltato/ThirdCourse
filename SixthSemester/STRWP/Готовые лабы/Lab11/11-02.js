let WebSocket = require('ws');
let fs = require('fs');

let wss = new WebSocket.Server({port: 4000, host: 'localhost'});

wss.on('connection', (ws) => {
    console.log('Client connected');

    let duplex = WebSocket.createWebSocketStream(ws, {encoding: 'utf8'});
    let rfile = fs.createReadStream(`./download/MyFile.txt`);
    rfile.pipe(duplex);

    rfile.on('end', () => {
        console.log('File send successfully');
    });
})

wss.on('error', (e) => {
    console.log(`Server error: ${e}`);
});

setTimeout(() => {
    console.log('Server closing...');
    wss.close();
}, 25000);