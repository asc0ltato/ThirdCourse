const http = require('http');
const url = require("url");
const querystring = require('querystring');
const parseString = require('xml2js').parseString;
const builder = require('xmlbuilder');
const multiparty = require('multiparty');
const fs = require("fs");
const pathMod = require("path");

http.createServer((request, response) => {
    switch (request.method) {
        case "GET":
            getHandler(request, response);
            break;
        case "POST":
            postHandler(request, response);
            break;
        default:
            response.statusCode = 405;
            response.statusMessage = 'Method Not Allowed';
            break;
    }
}).listen(5000, () => console.log("Server is running at http://localhost:5000"));

function getHandler(request, response) {
    const path = url.parse(request.url, true);

    switch (path.pathname) {
        //Запустить сервер, потом клиента через Run
        case '/09-01': {
            response.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
            response.end('response from server 09-01');
            break;
        }

        case "/09-02": {
            response.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
            let x = path.query.x;
            let y = path.query.y;
            response.end(`x = ${x}, y = ${y}`);
            break;
        }

        case "/09-08": {
            const filePath = pathMod.join(__dirname, 'static', 'server.png');

            fs.stat(filePath, (err, stat) => {
                if (err) {
                    response.writeHead(404, { 'Content-Type': 'text/plain; charset=utf-8' });
                    response.end('File not found');
                    return;
                }

                response.writeHead(200, {
                    'Content-Type': 'image/png',
                    'Content-Disposition': 'attachment; filename="server.png"',
                    'Content-Length': stat.size
                });

                // Создаем поток и передаем файл клиенту
                const stream = fs.createReadStream(filePath);
                stream.pipe(response);
            });
            break;
        }
    }
}

function postHandler(request, response) {
    const path = url.parse(request.url, true);

    switch (path.pathname) {
        case "/09-03": {
            let data = '';

            request.on('data', (chunk) => {
                data += chunk;
            });

            request.on('end', () => {
                //{x: '1.5', y: '3', s: 'Svetlana'}
                const parsedBody = querystring.parse(data);
                const x = parsedBody.x;
                const y = parsedBody.y;
                const s = parsedBody.s;
                response.writeHead(200, {'Content-Type': 'text/plain; charset=utf-8'});
                response.end(`x = ${x}, y = ${y}, s = ${s}`);
            });
            break;
        }

        case "/09-04": {
            let data = '';

            request.on('data', (chunk) => {
                data += chunk;
            });

            request.on('end', () => {
                const body = JSON.parse(data);

                const result = {
                    _comment: body.comment.replace("Запрос", "Ответ"),
                    x_plus_y: body.x + body.y,
                    Concatination_s_o: `${body.s}: ${body.o.surname} ${body.o.name}`,
                    Length_m: body.m.length
                };

                response.writeHead(200, {'Content-type': 'application/json; charset=utf-8'});
                response.end(JSON.stringify(result));
            });
            break;
        }

        case "/09-05": {
            let data = '';
            let sum = 0;
            let concat = "";
            let id = "";

            request.on('data', (chunk) => {
                data += chunk;
            });

            request.on('end', () => {
                parseString(data, (err, result) => {
                    if (err) {
                        console.error("XML Parse Error:", err);
                        response.writeHead(400, { "Content-Type": "text/plain" });
                        response.end("Invalid XML");
                    }
                    else {
                        id = result.request.$.id;

                        if (result.request.x) {
                            result.request.x.forEach((elem) => {
                                sum += parseInt(elem.$.value);
                            });
                        }

                        if (result.request.m) {
                            result.request.m.forEach((elem) => {
                                concat += elem.$.value;
                            });
                        }

                        response.writeHead(200, { "Content-Type": "application/xml; charset=utf-8" });

                        const responseXml = builder.create('response')
                            .att('id', 33)
                            .att('request', id)
                            .ele('sum', {'element': 'x', 'result': sum}).up()
                            .ele('concat', {'element': 'm', 'result': concat})
                            .end({ pretty: true });

                        response.end(responseXml);
                    }
                });
            });
            break;
        }

        case "/09-06-07": {
            let result = '';
            let form = new multiparty.Form({ uploadDir:'./static' });
            form.parse(request);

            form.on('file', (name, file) => {
                result += `original filename: ${file.originalFilename}; path = ${file.path}`;
            });

           form.on("error", (err) => {
                response.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
                response.end(`${err}`);
            });

           form.on("close", () => {
               response.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
               response.end(result);
           });

           break;
        }
    }
}