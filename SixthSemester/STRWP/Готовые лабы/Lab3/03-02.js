const http = require('http');
const url = require('url');

let fact = (n) => (n <= 1 ? 1 : n * fact(n-1));

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
                response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
                response.end(JSON.stringify({k: k, fact: fact(k)}));
            } else {
                response.writeHead(400, {'Content-Type': 'application/json; charset=utf-8'});
                response.end(JSON.stringify({error: 'Invalid value for k'}));
            }
        } else {
            response.writeHead(200, {'Content-Type': 'application/json; charset=utf-8'});
            response.end(rc);
        }
    } else {
        response.writeHead(404, {'Content-Type': 'application/json; charset=utf-8'});
        response.end(JSON.stringify({error: 'Not Found'}));
    }
});

server.listen(5000, () => {
    console.log('Server is running at http://localhost:5000/fact?k=3');
});