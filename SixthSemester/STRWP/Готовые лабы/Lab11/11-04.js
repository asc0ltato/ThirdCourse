let WebSocket = require('ws');

let wss = new WebSocket.Server({port: 4000, host: 'localhost'});

wss.on('connection', (ws) => {
    let messageCount = 0;

    ws.on('message', (msg) => {
        console.log(`Received: ${msg.toString()}`);
        let params = JSON.parse(msg);
        const response = {
            server: ++messageCount,
            client: params.client,
            timestamp: new Date().toISOString()
        };

        ws.send(JSON.stringify(response, null, 2));
    });
});

wss.on('error', (err) => {
    console.log('Error: ', err);
})