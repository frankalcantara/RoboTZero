// Ponte com o Worker do core (docs/03 §5.2/§5.3).
//
// Comandos vão por postMessage; o estado do frame vem do ring em
// SharedArrayBuffer, drenado uma vez por frame. Nenhum evento de pino vira
// atualização reativa (AP-02 do ADR 0007).
import wasmUrl from '../../../../build/wasm/wasm/esp32sim.wasm?url';
import workerUrl from '../../../../wasm/worker/esp32sim.worker.mjs?url';
import { drain, dropped as droppedCount, EVENT } from '../../../../wasm/worker/ring.mjs';
import type { CoreEvent } from '../../../../wasm/worker/ring.mjs';
import { applyEvents, type PinState } from '../presentation/pins';

type Reply =
  | { type: 'ready'; abi: number; version: string }
  | { type: 'ring'; buffer: SharedArrayBuffer; ringOffset: number; eventsOffset: number }
  | { type: 'ok'; cycles?: bigint }
  | { type: 'error'; message: string };

export type Frame = {
  /** Último ciclo reportado pelo core (chega um frame atrasado). */
  cycles: bigint;
  /** Eventos drenados desde o início. */
  events: number;
  /** Eventos perdidos por ring cheio desde o início. */
  dropped: number;
  pins: PinState;
};

export type CoreLink = {
  readonly version: string;
  readonly abi: number;
  frame(cycles: number): Frame;
  setPin(pin: number, level: 0 | 1): void;
};

export async function connect(capacity = 256): Promise<CoreLink> {
  const worker = new Worker(workerUrl, { type: 'module' });

  let pending: ((reply: Reply) => void) | null = null;
  let cycles = 0n;
  worker.onmessage = (event: MessageEvent<Reply>) => {
    const reply = event.data;
    if (reply.type === 'ok' && reply.cycles !== undefined) {
      cycles = reply.cycles;
    }
    pending?.(reply);
    pending = null;
  };

  const request = (message: unknown, transfer: Transferable[] = []): Promise<Reply> => {
    const reply = new Promise<Reply>((resolve) => {
      pending = resolve;
    });
    worker.postMessage(message, transfer);
    return reply;
  };
  const ok = (reply: Reply): Reply => {
    if (reply.type === 'error') {
      throw new Error(reply.message);
    }
    return reply;
  };

  const bytes = await (await fetch(wasmUrl)).arrayBuffer();
  const ready = ok(await request({ cmd: 'load', bytes }, [bytes]));
  const ring = ok(await request({ cmd: 'init', capacity }));
  if (ready.type !== 'ready' || ring.type !== 'ring') {
    throw new Error('worker: resposta inesperada');
  }

  let pins: PinState = {};
  let events = 0;

  return {
    version: ready.version,
    abi: ready.abi,
    frame(step) {
      // Só comandos de controle: o dado do frame vem do ring, não do postMessage.
      worker.postMessage({ cmd: 'run', cycles: step });
      const batch: CoreEvent[] = [];
      events += drain(ring.buffer, ring.ringOffset, ring.eventsOffset, (event) => batch.push(event));
      pins = applyEvents(pins, batch);
      return { cycles, events, dropped: droppedCount(ring.buffer, ring.ringOffset), pins };
    },
    setPin(pin, level) {
      // esp32sim_test_emit é o hook de teste da ABI: não há GPIO até E1-02.
      worker.postMessage({ cmd: 'emit', type: EVENT.PIN_CHANGE, code: pin, payload: level });
    },
  };
}
