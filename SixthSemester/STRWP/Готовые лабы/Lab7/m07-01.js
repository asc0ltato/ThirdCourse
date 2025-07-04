const fs = require('fs');
const path = require('path');

module.exports = (staticDir) => {
    return (request, response) => {
        if (request.method !== 'GET') {
            response.statusCode = 405;
            response.end('Method Not Allowed');
        }

        const filePath = path.join(staticDir, request.url);

        fs.stat(filePath, (err, stats) => {
            if (err || !stats.isFile()) {
                response.statusCode = 404;
                response.end('Not Found');
            }
        })

        const extension = path.extname(filePath).slice(1);
        let mimeType = 'application/octet-stream';
        switch (extension) {
            case 'html': mimeType = 'text/html'; break;
            case 'css': mimeType = 'text/css'; break;
            case 'js': mimeType = 'text/javascript'; break;
            case 'png': mimeType = 'image/png'; break;
            case 'docx': mimeType = 'application/msword'; break;
            case 'json': mimeType = 'application/json'; break;
            case 'xml': mimeType = 'application/xml'; break;
            case 'mp4': mimeType = 'video/mp4'; break;
        }

        response.setHeader('Content-Type', mimeType);
        fs.createReadStream(filePath).pipe(response);
    }
}