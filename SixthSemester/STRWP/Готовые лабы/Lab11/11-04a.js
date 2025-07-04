let WebSocket = require('ws');

let ws = new WebSocket('ws://localhost:4000/');

const clientName = process.argv[2] || 'default';

ws.on('open', () => {
    ws.on('message', msg => {
        console.log(msg.toString());
    })

    const message = {
        client: clientName,
        timestamp: new Date().toISOString()
    };

    ws.send(JSON.stringify(message, null, 2));
})

ws.on('error', (err) => {
    console.log('Error: ', err)
});