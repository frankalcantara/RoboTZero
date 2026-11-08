import assert from 'node:assert/strict';
import test from 'node:test';
import { EVENT } from '../../../../wasm/worker/ring.mjs';
import type { CoreEvent } from '../../../../wasm/worker/ring.mjs';
import { applyEvents, isHigh, LED_PIN } from './pins.ts';

const pinChange = (pin: number, level: number): CoreEvent => ({
  type: EVENT.PIN_CHANGE,
  code: pin,
  cycle: 1n,
  payload: BigInt(level),
});

test('sem eventos o estado não muda', () => {
  const state = { 2: 1 as const };
  assert.deepEqual(applyEvents(state, []), state);
});

test('PIN_CHANGE liga o pino', () => {
  const state = applyEvents({}, [pinChange(LED_PIN, 1)]);
  assert.equal(isHigh(state, LED_PIN), true);
});

test('payload zero desliga o pino', () => {
  const state = applyEvents({ [LED_PIN]: 1 }, [pinChange(LED_PIN, 0)]);
  assert.equal(isHigh(state, LED_PIN), false);
});

test('o último evento do frame vence', () => {
  const state = applyEvents({}, [pinChange(LED_PIN, 1), pinChange(LED_PIN, 0)]);
  assert.equal(isHigh(state, LED_PIN), false);
});

test('evento de outro tipo é ignorado', () => {
  const serial: CoreEvent = { type: EVENT.SERIAL_TX, code: LED_PIN, cycle: 1n, payload: 1n };
  assert.equal(isHigh(applyEvents({}, [serial]), LED_PIN), false);
});

test('pino não visto é baixo', () => {
  assert.equal(isHigh({}, 42), false);
});
