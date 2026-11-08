// Tipos de ring.mjs (E0-05). O código continua JS puro; isto descreve a
// fronteira consumida pelo studio. Layout: docs/03-arquitetura.md §5.3.
export const EVENT_SIZE: number;

export const EVENT: {
  SERIAL_TX: 1;
  PIN_CHANGE: 2;
  IRQ: 3;
  DEVICE_STATE: 4;
  TRACE_MARK: 5;
  ERROR: 6;
  STOPPED: 7;
  BREAKPOINT: 8;
};

export type CoreEvent = {
  type: number;
  code: number;
  cycle: bigint;
  payload: bigint;
};

export function drain(
  buffer: ArrayBufferLike,
  ringOffset: number,
  eventsOffset: number,
  onEvent: (event: CoreEvent) => void,
): number;

export function dropped(buffer: ArrayBufferLike, ringOffset: number): number;
