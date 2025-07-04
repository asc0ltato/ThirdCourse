const http = require('http');
const fs = require('fs');

http.createServer(function (request, response) { 
    if (request.url === '/html') {
        let html = fs.readFileSync('./02-01.html');
        response.writeHead(200, {'Content-Type': 'text/html'});
        response.end(html);
    }
}).listen(5000);

console.log("Server running at http://localhost:5000/");