const http = require('http');
const xmlbuilder = require("xmlbuilder");

const xmldoc = xmlbuilder.create('request').att('id', 28);
xmldoc.ele('x').att('value', 1);
xmldoc.ele('x').att('value', 2);
xmldoc.ele('m').att('value', 'a');
xmldoc.ele('m').att('value', 'b');
xmldoc.ele('m').att('value', 'c');

const options = {
    host: 'localhost',
    path: `/09-05`,
    port: 5000,
    method: 'POST',
    headers: {
        'Accept': 'application/xml; charset=utf-8'
    }
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

request.write(xmldoc.toString({pretty: true}));
request.end();