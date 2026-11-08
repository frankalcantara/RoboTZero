// Servidor estático mínimo para validar o E0-05 no navegador.
//
// SharedArrayBuffer só existe em página cross-origin isolated, então o servidor
// manda COOP + COEP. É a mesma exigência do deploy (E0-07).
//
//   node wasm/dev-server.cjs [porta]
//   abra http://localhost:8080/

'use strict';

const http = require('node:http');
const fs = require('node:fs');
const path = require('node:path');

const root = path.resolve(__dirname, '..');
const port = Number(process.argv[2] ?? 8080);

const routes = {
  '/': path.join(root, 'wasm', 'demo', 'e0-05.html'),
  '/esp32sim.wasm': path.join(root, 'build', 'wasm', 'wasm', 'esp32sim.wasm'),
};

const contentTypes = {
  '.html': 'text/html; charset=utf-8',
  '.js': 'text/javascript; charset=utf-8',
  '.mjs': 'text/javascript; charset=utf-8',
  '.wasm': 'application/wasm',
};

http
  .createServer((request, response) => {
    const pathname = decodeURIComponent(new URL(request.url, 'http://localhost').pathname);
    const file = routes[pathname] ?? path.join(root, pathname);

    // Dev server: não serve nada fora da raiz do projeto.
    if (!file.startsWith(root + path.sep)) {
      response.writeHead(403).end();
      return;
    }

    fs.readFile(file, (error, data) => {
      if (error) {
        response.writeHead(404).end('not found');
        return;
      }
      response.writeHead(200, {
        'content-type': contentTypes[path.extname(file)] ?? 'application/octet-stream',
        // Dev server: nunca cachear, senão o navegador guarda Worker e wasm velhos.
        'cache-control': 'no-store',
        'cross-origin-opener-policy': 'same-origin',
        'cross-origin-embedder-policy': 'require-corp',
        'cross-origin-resource-policy': 'same-origin',
      });
      response.end(data);
    });
  })
  .listen(port, () => {
    console.log(`http://localhost:${port}/  (COOP/COEP ativos)`);
  });
