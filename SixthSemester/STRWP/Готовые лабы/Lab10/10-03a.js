const WebSocket = require('ws');
const readline = require('readline');

const socket = new WebSocket('ws://localhost:5000/broadcast');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
})

socket.onopen = () => {
    console.log('socket.open');

    rl.on('line', (input) => {
        socket.send(input);
    });
};

socket.onmessage = (e) => {
    console.log('socket.onmessage: ' + e.data);
};

socket.onclose = () => {
    console.log('socket.onclose');
    rl.close();
};

socket.onerror = (error) => {
    console.log('socket.onerror: ' + error.message);
};