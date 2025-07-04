const https = require('https');
const fs = require('fs');

const options = {
  key: fs.readFileSync('Resource/RS-LAB22-ZSS.key'),
  cert: fs.readFileSync('Resource/RS-LAB22-ZSS.crt'),
};

https.createServer(options, (req, res) => {
  res.writeHead(200);
  res.end('RESOURCE, req:' + req.url);
}).listen(8443, () => {
  console.log('HTTPS server running on port 8443 https://ZSS:8443');
});