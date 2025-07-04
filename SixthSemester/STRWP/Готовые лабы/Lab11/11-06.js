let rpcWSS = require('rpc-websockets').Server;

let server = new rpcWSS({port:4000, host: 'localhost'});

server.event('A');
server.event('B');
server.event('C');

process.stdin.setEncoding('utf8');
process.stdin.on('readable', () => {
    let data = null;
    while((data = process.stdin.read()) !== null){
        switch (data.trim().toUpperCase()){
            case 'A':
                server.emit('A', 'Its event A');
                break;
            case 'B':
                server.emit('B', 'Its event B');
                break;
            case 'C':
                server.emit('C', 'Its event C');
                break;
            default: 
                console.log('There is no such event')
        }
    }
});