// Catálogo de mensagens (AP-05): nenhuma string visível inline no componente.
const messages = {
  'app.title': 'ESP32Sim Studio',
  'scene.label': 'Placa ESP32-C3 com LED',
  'bench.led': 'LED (GPIO {pin})',
  'status.loading': 'carregando core…',
  'status.ready': 'core pronto',
  'status.error': 'falha: {message}',
  'hud': 'v{version} · ABI {abi} · {cycles} ciclos · {events} eventos · {dropped} perdidos · LED {led}',
  'hud.on': 'ligado',
  'hud.off': 'desligado',
} as const;

export type MessageKey = keyof typeof messages;

// Catálogo exposto para o teste de AP-05 (chave ausente / chave órfã).
export const messageKeys: readonly MessageKey[] = Object.keys(messages) as MessageKey[];

export function t(key: MessageKey, vars: Record<string, string | number> = {}): string {
  return messages[key].replace(/\{(\w+)\}/g, (match, name: string) =>
    name in vars ? String(vars[name]) : match,
  );
}
