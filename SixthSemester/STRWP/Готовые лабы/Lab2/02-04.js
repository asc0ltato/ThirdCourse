const http = require('http');
const fs = require('fs');

http.createServer(function (request, response) { 
    if (request.url === '/api/name') {
        response.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
        response.end('Zhuk Svetlana Sergeevna');
    } else if (request.url === '/xmlhttprequest'){
        const name = './xmlhttprequest.html';

        fs.readFile (name, (error, data) => {
            if (error) {
                console.log('error: ' + error);
            } else {
                response.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
                response.end(data, 'binary');
            }
        });
    }
}).listen(5000); 

console.log("Server running at http://localhost:5000/");