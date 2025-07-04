const http = require('http');
let state = 'norm';

const server = http.createServer(function (request, response) {
    response.writeHead(200, {'Content-Type': 'text/html'});
    response.end(`<h1>${state}</h1>`);
});

server.listen(5000, () => {
    console.log('Server is running at http://localhost:5000');
});

process.stdin.setEncoding('utf8');
process.stdin.on('readable', () => {
    let chunk;
    while ((chunk = process.stdin.read()) !== null) {
        chunk = chunk.trim();
        if (chunk === 'exit') {
            process.exit(0);
        }  else if (['norm', 'stop', 'test', 'idle'].includes(chunk)) {
            process.stdout.write(`${state}--> ${chunk}` + '\n');
            state = chunk;
        }  else {
            process.stdout.write(chunk + '\n');
        }
    }
})