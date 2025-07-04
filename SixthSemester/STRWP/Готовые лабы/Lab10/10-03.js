const WebSocket = require('ws');

const socket = new WebSocket.Server({ port: 5000, host: 'localhost', path: `/broadcast` });

socket.on('connection', (ws) => {
    console.log('New client connected');

    ws.on('message', (msg) => {
        console.log('Received:', msg.toString());

        socket.clients.forEach((client) => {
            if (client.readyState === WebSocket.OPEN) {
                client.send(`${msg}`);
            }
        });
    });

    ws.on('close', () => {
        console.log('Client disconnected (WS connection closed by client)');
    });
})

socket.on('error', (err) => {
    console.log(`WS error: ${err}`)
})

console.log(`WS-server is running on ws://${socket.options.host}:${socket.options.port}${socket.options.path}`);