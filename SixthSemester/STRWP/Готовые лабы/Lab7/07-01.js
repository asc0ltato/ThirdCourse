const http = require('http');
const m07_01 = require('./m07-01');

const server = http.createServer(m07_01("static"));

//http://localhost:5000/html/client.html
server.listen(5000, () => {
    console.log(`Server is running at http://localhost:5000`);
});