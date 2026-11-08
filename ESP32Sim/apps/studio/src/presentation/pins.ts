// Estado de pinos da apresentação (AP-03): puro, sem DOM, sem three.js.
//
// PIN_CHANGE: `code` = GPIO, `payload` = nível (0 = baixo, != 0 = alto).
// Convenção do spike E0-06; o GPIO de verdade entra em E1-02 (docs/03 §5.3).
import { EVENT } from '../../../../wasm/worker/ring.mjs';
import type { CoreEvent } from '../../../../wasm/worker/ring.mjs';

export type PinState = Readonly<Record<number, 0 | 1>>;

// LED da placa: GPIO8 no ESP32-C3-DevKitM-1. O mapa real da placa é UN-009,
// resolvido em E1-06.
export const LED_PIN = 8;

// Agrega um frame inteiro de eventos em um único estado novo (AP-02).
export function applyEvents(state: PinState, events: readonly CoreEvent[]): PinState {
  const next: Record<number, 0 | 1> = { ...state };
  for (const event of events) {
    if (event.type === EVENT.PIN_CHANGE) {
      next[event.code] = event.payload === 0n ? 0 : 1;
    }
  }
  return next;
}

export function isHigh(state: PinState, pin: number): boolean {
  return state[pin] === 1;
}
