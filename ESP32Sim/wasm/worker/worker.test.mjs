// Worker real (node:worker_threads) + SharedArrayBuffer + ring (E0-05).
//
// Prova o caminho que a UI usará: o core roda no Worker, publica o ring uma vez
// e a main thread drena por frame com Atomics, sem chamada JS por ciclo.

import assert from 'node:assert/strict';
import fs from 'node:fs';
import { Worker } from 'node:worker_threads';

import { drain, dropped, EVENT } from './ring.mjs';

const wasmPath = process.argv[2];
if (!wasmPath) {
  console.error('uso: node worker.test.mjs <esp32sim.wasm>');
  process.exit(2);
}

const worker = new Worker(new URL('./esp32sim.worker.mjs', import.meta.url));

// O protocolo é estritamente request/response: uma mensagem por comando.
const next = () =>
  new Promise((resolve, reject) => {
    worker.once('message', resolve);
    worker.once('error', reject);
  });

// Carrega o `.wasm` cru: o Worker não depende de glue JS.
const file = fs.readFileSync(wasmPath);
const bytes = file.buffer.slice(file.byteOffset, file.byteOffset + file.byteLength);
worker.postMessage({ cmd: 'load', bytes }, [bytes]);

const ready = await next();
assert.equal(ready.type, 'ready', ready.message);
assert.equal(ready.abi, 0x0001, 'ABI version');
assert.equal(ready.version, '0.1.0', 'build version');

worker.postMessage({ cmd: 'init', capacity: 8 });
const ring = await next();
assert.equal(ring.type, 'ring', ring.message);
assert.ok(ring.buffer instanceof SharedArrayBuffer, 'ring memory is a SharedArrayBuffer');
assert.equal(ring.capacity, 8);

worker.postMessage({ cmd: 'run', cycles: 1000 });
const ran = await next();
assert.equal(ran.type, 'ok', ran.message);
assert.equal(ran.cycles, 1000n, 'cycles advanced');

// Oito eventos cabem; a main thread drena todos de uma vez.
for (let code = 0; code < 8; code += 1) {
  worker.postMessage({ cmd: 'emit', type: EVENT.PIN_CHANGE, code, payload: code * 10 });
  const ack = await next();
  assert.equal(ack.type, 'ok', ack.message);
}

const events = [];
assert.equal(drain(ring.buffer, ring.ringOffset, ring.eventsOffset, (e) => events.push(e)), 8);
assert.equal(events.length, 8, 'one callback per event, one drain per frame');
assert.deepEqual(events[0], {
  type: EVENT.PIN_CHANGE,
  code: 0,
  cycle: 1000n,
  payload: 0n,
});
assert.deepEqual(events[7], {
  type: EVENT.PIN_CHANGE,
  code: 7,
  cycle: 1000n,
  payload: 70n,
});
assert.equal(drain(ring.buffer, ring.ringOffset, ring.eventsOffset, () => {}), 0, 'ring is empty');

// Ring cheio: o mais novo é descartado e o contador sobe.
for (let code = 0; code < 10; code += 1) {
  worker.postMessage({ cmd: 'emit', type: EVENT.TRACE_MARK, code });
  const ack = await next();
  assert.equal(ack.type, 'ok', ack.message);
}
assert.equal(dropped(ring.buffer, ring.ringOffset), 2, 'overflow counted');
assert.equal(drain(ring.buffer, ring.ringOffset, ring.eventsOffset, () => {}), 8, 'ring full');

await worker.terminate();

console.log('worker smoke OK: SharedArrayBuffer, ring 8, 8 eventos drenados, 2 descartados');
