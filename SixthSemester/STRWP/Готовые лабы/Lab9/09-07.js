const http = require('http');
const fs = require('fs');
const FormData = require('form-data')

const form = new FormData();
form.append('file', fs.createReadStream('./MyFile.png'));

const options = {
    host: 'localhost',
    path: `/09-06-07`,
    port: 5000,
    method: 'POST',
    headers: form.getHeaders()
}

const request = http.request(options, (response) => {
    let data = '';
    console.log(`Response statusCode: ${response.statusCode}`);

    response.on('data', (chunk) => {
        data += chunk;
    });

    response.on('end', () => {
        console.log(`Response data: ${data}`);
    });
});

request.on('error', (err) => {
    console.log(`Request error: ${err.message}`);
});

form.pipe(request);