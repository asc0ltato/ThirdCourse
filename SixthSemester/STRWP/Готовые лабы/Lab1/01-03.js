const http = require('http');

let getHeaders = (request) => { 
    let headers = '';
    let i = 0;
    for (let key in request.headers) {
        headers += '<h3>' + ++i + '. ' + key + ':' + request.headers[key] + '</h3>';
    }
    return headers;
}

http.createServer(function (request, response) {
    let body = '';
    request.on('data', 
        str => {
            body +=str; 
            console.log('data', body);
        })
    response.writeHead(200, {'Content-Type': 'text/html; charset=utf-8'});
    request.on('end', () => response.end( 
            '<!DOCTYPE html>' +
            '<html lang="en">' +
            '<head>' +
            '<title>01-03</title>' +
            '</head>' +
            '<body>' +
            '<h1>Request structure</h1>' +
            '<h2>' + 'Method: ' + request.method + '</h2>' +
            '<h2>' + 'Uri: ' + request.url + '</h2>' +
            '<h2>' + 'Version: ' + request.httpVersion + '</h2>' +
            '<h2>' + 'Headers: ' + '</h2>' + getHeaders(request) +
            '<h2>' + 'Body: ' + body + '</h2>' +
            '</body>' +
            '</html>'
    ));
}).listen(3000);

console.log("Server running at http://localhost:3000/");