const fs = require("fs");
const express = require('express');
const path = require("path");
const wasmFile = fs.readFileSync("a.out.wasm");

const wasiImports = {
  "wasi_snapshot_preview1": {
    "args_get": () => {},
    "args_sizes_get": () => {},
    "proc_exit": () => {},
  }
};

WebAssembly.instantiate(wasmFile, wasiImports).then(wasmModule => {
  const { sum, mul, sub } = wasmModule.instance.exports;

  console.log("sum(5,3) =", sum(5,3));
  console.log("mul(5,3) =", mul(5,3));
  console.log("sub(5,3) =", sub(5,3));
}).catch(err => console.error("Ошибка загрузки WASM:", err));

const server = express();

server.get('/funcs.wasm', (req, res) => {
  res.sendFile(path.join(__dirname, "./a.out.wasm"));
})
server.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, "./emcc.html"));
})

server.listen(3000);

// base64 a.out.wasm > a.out.txt WSL
// emcc funcs.c -s EXPORTED_FUNCTIONS="['_sum', '_mul', '_sub']" Windows