// Leitura do ring de eventos publicado pelo core (E0-05).
//
// O ring vive na memória linear do `.wasm`, que é um SharedArrayBuffer: o Worker
// escreve, a main thread lê, sem cópia e sem chamada JS por ciclo.
// Layout: docs/03-arquitetura.md §5.3.

export const EVENT_SIZE = 24;

export const EVENT = {
  SERIAL_TX: 1,
  PIN_CHANGE: 2,
  IRQ: 3,
  DEVICE_STATE: 4,
  TRACE_MARK: 5,
  ERROR: 6,
  STOPPED: 7,
  BREAKPOINT: 8,
};

// Consome até `capacity` eventos e avança `tail` de uma vez. Chame uma vez por
// frame: é isso que mantém N eventos de pino fora da camada reativa (AP-02).
// Devolve quantos eventos foram entregues.
export function drain(buffer, ringOffset, eventsOffset, onEvent) {
  const header = new Int32Array(buffer, ringOffset, 4);
  const capacity = Atomics.load(header, 0);
  const tail = Atomics.load(header, 2);
  const head = Atomics.load(header, 1);
  // Contadores de 32 bits: a subtração em Int32 já fecha certo no wrap.
  const count = Math.min(head - tail, capacity);
  if (count <= 0) {
    return 0;
  }

  const view = new DataView(buffer, eventsOffset, capacity * EVENT_SIZE);
  for (let i = 0; i < count; i += 1) {
    const base = ((tail + i) & (capacity - 1)) * EVENT_SIZE;
    onEvent({
      type: view.getUint32(base, true),
      code: view.getUint32(base + 4, true),
      cycle: view.getBigUint64(base + 8, true),
      payload: view.getBigUint64(base + 16, true),
    });
  }
  Atomics.store(header, 2, tail + count);
  return count;
}

// Eventos perdidos desde o início da simulação (ring cheio).
export function dropped(buffer, ringOffset) {
  return Atomics.load(new Int32Array(buffer, ringOffset, 4), 3);
}
