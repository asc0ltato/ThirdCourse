let WebSocket = require('ws');

let wss = new WebSocket.Server({port: 5000, host: 'localhost'});

let n = 0;
wss.on('connection', ws => {
    ws.isAlive = true;

    setInterval(() => {
        wss.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(`11-03-server: ${++n}`);
            }
        });
    }, 15000);

    setInterval(() => {
        let activeCount = 0;

        wss.clients.forEach((client) => {
            if (client.isAlive === false) {
                client.terminate();
            } else {
                client.isAlive = false;
                client.ping();
                activeCount++;
            }
        });

        console.log(`available: ${activeCount}`);
    }, 5000);

    ws.on('pong', () => {
        ws.isAlive = true;
    })
})