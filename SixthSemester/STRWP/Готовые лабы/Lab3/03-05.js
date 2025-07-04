const http = require('http');
const url = require('url');
const fs = require('fs');

let fact = (n, callback) => {
    setImmediate(() => {
        if (n <= 1) {
            callback(1);
        } else {
            fact(n - 1, (result) => {
                callback(n * result);
            });
        }
    });
};

const server = http.createServer(function (request, response) {
    const parsedUrl = url.parse(request.url, true);
    const pathname = parsedUrl.pathname;
    let rc = JSON.stringify({k: 0});

    if (pathname === '/fact') {
        let k = parsedUrl.query.k;

        if (typeof k !== 'undefined') {
            if (Array.isArray(k)) k = k[0];
            k = parseInt(k);
            if (Number.isInteger(k)) {
                fact(k, (result) => {
                    response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
                    response.end(JSON.stringify({k: k, fact: result}));
                })
            } else {
                response.writeHead(400, {'Content-Type': 'application/json; charset=utf-8'});
                response.end(JSON.stringify({error: 'Invalid value for k'}));
            }
        } else {
            response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
            response.end(rc);
        }
    } else if (pathname === '/') {
        let html = fs.readFileSync('./03-03.html');
        response.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
        response.end(html);
    } else {
        response.writeHead(404, {'Content-Type': 'application/json; charset=utf-8'});
        response.end(JSON.stringify({error: 'Not Found'}));
    }
});

server.listen(5000, () => {
    console.log('Server is running at http://localhost:5000');
});