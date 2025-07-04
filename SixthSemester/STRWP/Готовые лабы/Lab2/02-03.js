const http = require('http');

http.createServer(function(request, response) {
    if(request.method === 'GET' && request.url === '/api/name') {
        response.writeHead(200, {'Content-Type': 'text/plain'});
        response.end('Zhuk Svetlana Sergeevna');
    }
}).listen(5000);

console.log("Server running at http://localhost:5000/");