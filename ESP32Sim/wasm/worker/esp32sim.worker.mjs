// Worker do core (E0-05). Instancia o `.wasm` e publica o ring para a main.
//
// Protocolo, sempre em postMessage:
//   main -> worker: { cmd: 'load', bytes }      (ArrayBuffer transferido)
//                   { cmd: 'init', capacity }   (potência de 2, >= 2)
//                   { cmd: 'run', cycles }
//                   { cmd: 'emit', type, code, payload }   (hook de teste)
//   worker -> main: { type: 'ready', abi, version }
//                   { type: 'ring', buffer, ringOffset, eventsOffset, capacity }
//                   { type: 'ok', ... }
//                   { type: 'error', message }
//
// Nada de estado de simulação aqui: o Worker só traduz comandos em chamadas da
// ABI. A UI lê o ring e decide o que mostrar.

// Node e browser expõem o worker de formas diferentes; o resto é igual.
const nodePort =
  typeof self === 'undefined' ? (await import('node:worker_threads')).parentPort : null;
const post = nodePort ? (message) => nodePort.postMessage(message) : (m) => self.postMessage(m);
const listen = (handler) =>
  nodePort ? nodePort.on('message', handler) : (self.onmessage = (event) => handler(event.data));

const decoder = new TextDecoder();

let abi = null;
let handle = 0;

function readCString(memory, pointer) {
  const bytes = new Uint8Array(memory.buffer, pointer);
  // `slice` copia: TextDecoder recusa uma view sobre SharedArrayBuffer.
  return decoder.decode(bytes.slice(0, bytes.indexOf(0)));
}

function check(status, what) {
  if (status !== 0) {
    throw new Error(`${what}: status ${status}`);
  }
}

const commands = {
  async load({ bytes }) {
    const { instance } = await WebAssembly.instantiate(bytes, {});
    abi = instance.exports;
    abi._initialize(); // construtores estáticos do modo standalone
    post({
      type: 'ready',
      abi: abi.esp32sim_abi_version(),
      version: readCString(abi.memory, abi.esp32sim_version_string()),
    });
  },

  init({ capacity }) {
    const out = abi.malloc(4);
    try {
      check(abi.esp32sim_create(0, out), 'esp32sim_create');
      handle = new DataView(abi.memory.buffer, out, 4).getUint32(0, true);
    } finally {
      abi.free(out);
    }
    check(abi.esp32sim_event_ring_init(handle, capacity), 'esp32sim_event_ring_init');
    post({
      type: 'ring',
      buffer: abi.memory.buffer,
      ringOffset: abi.esp32sim_event_ring_header(handle),
      eventsOffset: abi.esp32sim_event_ring_events(handle),
      capacity,
    });
  },

  run({ cycles }) {
    check(abi.esp32sim_run(handle, BigInt(cycles)), 'esp32sim_run');
    post({ type: 'ok', cycles: abi.esp32sim_cycle_count(handle) });
  },

  emit({ type, code, payload }) {
    check(abi.esp32sim_test_emit(handle, type, code, BigInt(payload ?? 0)), 'esp32sim_test_emit');
    post({ type: 'ok' });
  },
};

listen(async (message) => {
  try {
    const command = commands[message.cmd];
    if (!command) {
      throw new Error(`comando desconhecido: ${message.cmd}`);
    }
    await command(message);
  } catch (error) {
    post({ type: 'error', message: String(error?.stack ?? error) });
  }
});
