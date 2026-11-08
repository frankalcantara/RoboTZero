'use strict';

// Smoke test da ABI C dentro do módulo WASM (E0-04).
//
// O módulo é autônomo: nenhum import, nenhum glue JS. Instanciamos o `.wasm`
// cru exatamente como o Worker fará no E0-05 e chamamos os exports da ABI.
//
// Strings: a ABI devolve ponteiros para a memória linear; decodificamos aqui
// sem depender de runtime do Emscripten.

const assert = require('node:assert/strict');
const fs = require('node:fs');

const wasmPath = process.argv[2];
if (!wasmPath) {
  console.error('uso: node smoke.cjs <esp32sim.wasm>');
  process.exit(2);
}

const ABI_VERSION = 0x0001;
const OK = 0;
const INVALID_ARGUMENT = 1;
const STATE = 3;
const U64_MAX = 0xffffffffffffffffn;
const CONFIG_SIZE = 8;

const decoder = new TextDecoder();

function readCString(memory, pointer) {
  const bytes = new Uint8Array(memory.buffer, pointer);
  const end = bytes.indexOf(0);
  // `slice` copia: TextDecoder recusa uma view sobre SharedArrayBuffer.
  return decoder.decode(bytes.slice(0, end));
}

(async () => {
  const bytes = fs.readFileSync(wasmPath);
  const { instance } = await WebAssembly.instantiate(bytes, {});
  const e = instance.exports;

  // `_initialize` roda construtores estáticos; com ABI C pura é uma chamada
  // vazia, mas chamá-la mantém o contrato do modo standalone.
  e._initialize();

  // Identidade.
  assert.equal(e.esp32sim_abi_version(), ABI_VERSION, 'abi version');
  assert.equal(readCString(e.memory, e.esp32sim_version_string()), '0.1.0', 'version string');

  const versionPtr = e.malloc(16);
  assert.equal(e.esp32sim_get_version(versionPtr), OK, 'get_version status');
  const version = new DataView(e.memory.buffer, versionPtr, 16);
  assert.equal(version.getUint32(0, true), 0, 'major');
  assert.equal(version.getUint32(4, true), 1, 'minor');
  assert.equal(version.getUint32(8, true), 0, 'patch');
  assert.equal(version.getUint32(12, true), ABI_VERSION, 'abi field');
  e.free(versionPtr);
  assert.equal(e.esp32sim_get_version(0), INVALID_ARGUMENT, 'get_version null');

  // Ciclo de vida com config explícita (struct_size + flags).
  const configPtr = e.malloc(CONFIG_SIZE);
  const config = new DataView(e.memory.buffer, configPtr, CONFIG_SIZE);
  config.setUint32(0, CONFIG_SIZE, true);
  config.setUint32(4, 0, true);

  const outPtr = e.malloc(4);
  assert.equal(e.esp32sim_create(configPtr, outPtr), OK, 'create with config');
  const handle = new DataView(e.memory.buffer, outPtr, 4).getUint32(0, true);
  assert.notEqual(handle, 0, 'handle is not null');

  // Config rejeitada: struct menor do que o conhecido e flags reservadas.
  config.setUint32(0, 4, true);
  assert.equal(e.esp32sim_create(configPtr, outPtr), INVALID_ARGUMENT, 'short config');
  config.setUint32(0, CONFIG_SIZE, true);
  config.setUint32(4, 1, true);
  assert.equal(e.esp32sim_create(configPtr, outPtr), INVALID_ARGUMENT, 'reserved flags');
  config.setUint32(4, 0, true);

  // Config nula = defaults.
  assert.equal(e.esp32sim_create(0, outPtr), OK, 'create with null config');
  const defaultHandle = new DataView(e.memory.buffer, outPtr, 4).getUint32(0, true);
  assert.notEqual(defaultHandle, 0, 'default handle is not null');
  e.esp32sim_destroy(defaultHandle);
  assert.equal(e.esp32sim_create(0, 0), INVALID_ARGUMENT, 'create null out');

  e.free(configPtr);
  e.free(outPtr);

  // Contagem de ciclos.
  assert.equal(e.esp32sim_cycle_count(handle), 0n, 'initial count');
  assert.equal(e.esp32sim_run(handle, 1000n), OK, 'run 1000');
  assert.equal(e.esp32sim_cycle_count(handle), 1000n, 'count after 1000');
  assert.equal(e.esp32sim_run(handle, 500n), OK, 'run 500');
  assert.equal(e.esp32sim_cycle_count(handle), 1500n, 'count after 1500');
  assert.equal(e.esp32sim_run(handle, 0n), OK, 'run 0 is valid');
  assert.equal(e.esp32sim_cycle_count(handle), 1500n, 'count unchanged');

  // Guardas.
  assert.equal(e.esp32sim_run(handle, U64_MAX), STATE, 'overflow rejected');
  assert.equal(e.esp32sim_cycle_count(handle), 1500n, 'count unchanged after overflow');
  assert.equal(e.esp32sim_run(0, 1n), INVALID_ARGUMENT, 'run null');
  assert.equal(e.esp32sim_cycle_count(0), 0n, 'count null is 0');

  // Ring de eventos (E0-05): o host lê o header e o array direto da memória.
  assert.equal(e.esp32sim_event_ring_header(handle), 0, 'no ring before init');
  assert.equal(e.esp32sim_test_emit(handle, 2, 0, 0n), INVALID_ARGUMENT, 'emit before init');
  assert.equal(e.esp32sim_event_ring_init(handle, 3), INVALID_ARGUMENT, 'capacity 3');
  assert.equal(e.esp32sim_event_ring_init(handle, 4), OK, 'init ring 4');
  assert.equal(e.esp32sim_event_ring_init(handle, 4), STATE, 'double init rejected');

  const ringPtr = e.esp32sim_event_ring_header(handle);
  const eventsPtr = e.esp32sim_event_ring_events(handle);
  assert.notEqual(ringPtr, 0, 'ring header pointer');
  assert.notEqual(eventsPtr, 0, 'events pointer');
  assert.ok(e.memory.buffer instanceof SharedArrayBuffer, 'memory is a SharedArrayBuffer');

  const header = new DataView(e.memory.buffer, ringPtr, 16);
  assert.equal(header.getUint32(0, true), 4, 'ring capacity');
  assert.equal(header.getUint32(4, true), 0, 'ring head');
  assert.equal(header.getUint32(8, true), 0, 'ring tail');

  assert.equal(e.esp32sim_test_emit(handle, 5, 42, 7n), OK, 'emit');
  assert.equal(header.getUint32(4, true), 1, 'head after emit');

  const event = new DataView(e.memory.buffer, eventsPtr, 24);
  assert.equal(event.getUint32(0, true), 5, 'event type');
  assert.equal(event.getUint32(4, true), 42, 'event code');
  assert.equal(event.getBigUint64(8, true), 1500n, 'event cycle');
  assert.equal(event.getBigUint64(16, true), 7n, 'event payload');

  // Consumir é só avançar `tail` da main thread, como o Worker fará.
  Atomics.store(new Int32Array(e.memory.buffer, ringPtr, 4), 2, 1);
  assert.equal(e.esp32sim_test_emit(handle, 5, 43, 0n), OK, 'emit after consume');
  assert.equal(header.getUint32(4, true), 2, 'head after consume');

  e.esp32sim_destroy(handle);
  e.esp32sim_destroy(0);

  console.log(
    `wasm smoke OK: ${bytes.length} bytes, ABI 0.1, versão 0.1.0, config validada, 1500 ciclos, ring 4`,
  );
})().catch((error) => {
  console.error(error);
  process.exit(1);
});
