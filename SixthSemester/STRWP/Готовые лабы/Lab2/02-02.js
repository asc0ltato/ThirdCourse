const http = require('http');
const fs = require('fs');

http.createServer(function (request, response) {
    if (request.method === 'GET' && request.url === '/png') {
        const name = './pic.png';

        fs.stat (name, (error, stat) => {
            if (error) {
                console.log('error: ' + error);
            } else {
                let png = fs.readFileSync(name);
                response.writeHead(200, {'Content-Type': 'image/png', 'Content-Length': stat.size});
                response.end(png, 'binary');
            }
        })
    }
}).listen(5000);

console.log("Server running at http://localhost:5000/");