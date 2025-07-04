const WebSocket = require('ws');

const socket = new WebSocket('ws://localhost:4000/ws');

socket.onopen = () => {
    let n = 0;
    console.log('socket.open');

    messageInterval = setInterval( () => {
        socket.send(++n);
    }, 3000);

    setTimeout(() => {
        clearInterval(messageInterval);
        socket.close();
    }, 25000);
};

socket.onmessage = (e) => {
    console.log('socket.onmessage: ' + e.data);
};

socket.onclose = () => {
    console.log('socket.onclose');
};

socket.onerror = (error) => {
    console.log('socket.onerror: ' + error.message);
};