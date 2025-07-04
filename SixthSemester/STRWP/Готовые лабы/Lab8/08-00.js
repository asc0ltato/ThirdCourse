const http = require("http");
const fs = require("fs");
const url = require("url");
const pathMod = require("path");
const multiparty = require('multiparty');
const parseString = require("xml2js").parseString;
const xmlbuilder = require("xmlbuilder");

const server = http.createServer((request, response) => {
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

server.on('connection', (socket) => {
    console.log('New connection');
    socket.on('close', () => {
        console.log('The connection is closed');
    });
});

function getHandler(request, response) {
    const path = url.parse(request.url, true);

    switch (path.pathname) {
        case "/connection": {
            // http://localhost:5000/connection
            // http://localhost:5000/connection?set=2000
            // видим, что connection через ~2 сек закрылось
            const setParam = path.query.set;

            if (setParam) {
                const newTimeout = parseInt(setParam);
                if (!isNaN(newTimeout)) {
                    server.keepAliveTimeout = newTimeout;
                    response.writeHead(200, {'Content-Type': 'text/plain'});
                    response.end(`KeepAliveTimeout set to ${server.keepAliveTimeout} ms\n`);
                } else {
                    response.writeHead(400, {'Content-Type': 'text/plain'});
                    response.end('Incorrect value of the set parameter\n');
                }
            } else {
                response.writeHead(200, {'Content-Type': 'text/plain'});
                response.end(`KeepAliveTimeout is ${server.keepAliveTimeout} ms\n`);
            }
            break;
        }

        case "/headers": {
            //http://localhost:5000/headers
            response.setHeader("University", "BSTU");
            response.setHeader("Faculty", "IT");
            response.setHeader("Course", "3");
            response.setHeader("Group", "2");
            response.setHeader("Student", "Zhuk Svetlana");
            response.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });

            response.write("<h1>Request headers:</h1>");
            for (let key in request.headers) {
                response.write(`<p><strong>${key}</strong>: ${request.headers[key]}</p>`);
            }

            response.write("<h1>Response headers:</h1>");
            const headers = response.getHeaders();
            for (let key in headers) {
                response.write(`<p><strong>${key}</strong>: ${headers[key]}</p>`);
            }

            response.end();
            break;
        }

        case "/parameter": {
            //http://localhost:5000/parameter?x=1.5&&y=5
            //http://localhost:5000/parameter?x=1.5&&y=0
            //http://localhost:5000/parameter
            const query = url.parse(request.url, true).query;
            const x = parseFloat(query.x);
            const y = parseFloat(query.y);

            response.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });

            if (!isNaN(x) && !isNaN(y)) {
                response.write(`<h2>x = ${x}, y = ${y}</h2>`);
                response.write(`<h2>+: ${x + y}</h2>`);
                response.write(`<h2>-: ${x - y}</h2>`);
                response.write(`<h2>*: ${x * y}</h2>`);
                response.end(`<h2>/: ${y !== 0 ? (x / y) : 'dividing by 0'}</h2>`);
            } else {
                response.end(`<h2>Error: The parameters x and y must be numbers.
                                  Received x = ${query.x}, y = ${query.y}</h2>`);
            }
            break;
        }

        case (path.pathname.startsWith("/parameter/") && path.pathname.split("/").length === 4 ? path.pathname : ""): {
            //http://localhost:5000/parameter/1.5/3
            //http://localhost:5000/parameter/1.5/0
            //http://localhost:5000/parameter
            //parts = ["", "parameter", "1.5", "3"]
            const parts = path.pathname.split("/");
            const x = parseFloat(parts[2]);
            const y = parseFloat(parts[3]);

            response.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });

            if (!isNaN(x) && !isNaN(y)) {
                response.write(`<h2>x = ${x}, y = ${y}</h2>`);
                response.write(`<h2>+: ${x + y}</h2>`);
                response.write(`<h2>-: ${x - y}</h2>`);
                response.write(`<h2>*: ${x * y}</h2>`);
                response.end(`<h2>/: ${y !== 0 ? (x / y) : 'dividing by 0'}</h2>`);
            } else {
                response.end(`<h2>Invalid URI: ${path.pathname}</h2>`);
            }
            break;
        }

        case "/close": {
            //http://localhost:5000/close
            response.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
            response.end("<h1>The server will stop after 10 seconds</h1>");
            setTimeout(() => {
                server.close();
                //process.exit();
            }, 10000);
            break;
        }

        case "/socket": {
            //http://localhost:5000/socket
            response.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
            response.end(`<h2>Client address: ${request.connection.remoteAddress}
                              Client port: ${request.connection.remotePort}</h2>
                          <h2>Server address: ${request.socket.localAddress} 
                              Server port: ${request.socket.localPort}</h2>`);
            break;
        }

        case "/req-data": {
            //Postman оч длинное сообщение написать в GET->Body->raw->text
            let body = '';

            request.on('data', chunk => {
                body += chunk;
                console.log(`Chunk: ${chunk.length} bytes`);
            });

            request.on('end', () => {
                response.writeHead(200, { 'Content-Type': 'text/html' });
                response.end(`<h2>Full body:</h2><p>${body}<p>`);
            });
            break;
        }

        case "/resp-status": {
            //http://localhost:5000/resp-status?code=200?mess=OK
            //http://localhost:5000/resp-status
            let query = request.url.split('?').slice(1).join('&');

            const queryParams = new URLSearchParams(query);

            const code = parseInt(queryParams.get('code'));
            const mess = queryParams.get('mess');

            if (!isNaN(code) && mess) {
                response.statusCode = code;
                response.statusMessage = mess;
                console.log(`Code: ${response.statusCode}, message: ${response.statusMessage}`);
                response.end(`code: ${code}; mess: ${mess}`);
            } else {
                response.statusCode = 400;
                response.statusMessage = "Bad Request";
                response.end("Invalid or missing code or mess parameter\n");
            }

            break;
        }

        case "/formparameter": {
            fs.createReadStream("form.html").pipe(response);
            break;
        }

        case "/files": {
            //Postman (GET)
            //http://localhost:5000/files
            fs.readdir("./static", (err, files) => {
                if (err) {
                    console.error("Error reading the directory:", err);
                    response.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
                    response.end("Server error when reading the directory static");
                    return;
                }

                const n = files.length;

                response.setHeader("X-static-files-count", n);
                response.writeHead(200, {"Content-Type": "text/plain; charset=utf-8"});
                response.end(`X-static-files-count: ${n}`);
            });
            break;
        }

        case (path.pathname.startsWith("/files/") ? request.url : ""): {
            //http://localhost:5000/files/css.css
            const filename = decodeURIComponent(path.pathname.replace("/files/", ""));
            const filepath = pathMod.join(__dirname, "static", filename);

            fs.readFile(filepath, (err, data) => {
                if (err) {
                    response.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
                    response.end("File not found");
                } else {
                    response.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
                    response.end(data);
                }
            });
            break;
        }

        case "/upload": {
            fs.createReadStream("upload.html").pipe(response);
            break;
        }
    }
}

function postHandler(request, response) {
    const path = url.parse(request.url, true);

    switch (path.pathname) {
        case "/formparameter": {
            //http://localhost:5000/formparameter
            let data = "";

            request.on("data", chunk => {
                data += chunk;
            });

            request.on("end", () => {
                response.writeHead(200, { "Content-Type": "text/plain" });
                response.end(data);
            });

            break;
        }

        case "/json": {
            //Postman (POST Body->raw->JSON)
            //http://localhost:5000/json
            let data = "";

            request.on("data", chunk => {
                data += chunk;
            });

            request.on("end", () => {
                const body = JSON.parse(data);

                const result =
                    "_comment:" + body.comment.replace("Запрос", "Ответ") + "\n" +
                    "x_plus_y: " + (body.x + body.y) + "\n" +
                    "Concatination_s_o: " + body.s + ": " + body.o.surname + " " + body.o.name + "\n" +
                    "Length_m: " + body.m.length;

                response.writeHead(200, { "Content-Type": "application/json; charset=utf-8" });
                response.end(result);
            });
            break;
        }

        case "/xml": {
            //Postman (POST Body->raw->JSON)
            //http://localhost:5000/xml
            let data = "";
            let sum = 0;
            let concat = "";
            let id = "";

            request.on("data", chunk => {
                data += chunk;
            });

            request.on("end", () => {
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

                        let xmldoc = xmlbuilder
                            .create("response")
                            .att("id", "33")
                            .att("request", id);
                        xmldoc.ele("sum").att("element", "x").att("result", sum);
                        xmldoc.ele("concat").att("element", "m").att("result", concat);

                        response.writeHead(200, { "Content-Type": "application/xml" });
                        response.end(xmldoc.toString());
                    }
                });
            });

            break;
        }

        case "/upload": {
            //http://localhost:5000/upload
            let form = new multiparty.Form({ uploadDir: "./static" })

            form.on("file", (name, file) => {
                console.log(
                    `name = ${name}; original filename: ${file.originalFilename}; path = ${file.path}`
                );
            });

            form.on("error", (err) => {
                response.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
                response.end(`${err}`);
            });

            form.on("close", () => {
                response.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
                response.end("File received");
            });

            form.parse(request);
            break;
        }
    }
}