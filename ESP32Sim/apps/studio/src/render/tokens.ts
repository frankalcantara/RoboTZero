// Leitura dos tokens de tema (AP-07). O render nunca declara cor literal.
export type Tokens = {
  background: string;
  board: string;
  ledOff: string;
  ledOn: string;
  light: string;
};

export function readTokens(): Tokens {
  const style = getComputedStyle(document.documentElement);
  const read = (name: string) => style.getPropertyValue(name).trim();
  return {
    background: read('--scene-background'),
    board: read('--board-pcb'),
    ledOff: read('--led-off'),
    ledOn: read('--led-on'),
    light: read('--scene-light'),
  };
}
