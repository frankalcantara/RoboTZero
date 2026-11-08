// AP-05: o catálogo não pode ter chave usada que falta nem chave órfã.
//
// A verificação do ADR 0007 é "teste de catálogo que falha em chave ausente ou
// órfã". O teste varre as chamadas `t('...')` do código e compara com
// `messageKeys`. Não valida placeholders (`{x}`) — isso pede uma varredura das
// variáveis passadas em cada chamada e fica para quando houver mais de um
// locale.
import assert from 'node:assert/strict';
import { readFileSync, readdirSync } from 'node:fs';
import { join } from 'node:path';
import test from 'node:test';
import { fileURLToPath } from 'node:url';
import { messageKeys } from './i18n.ts';

const SOURCE_DIR = fileURLToPath(new URL('.', import.meta.url));

// Só a chamada direta `t(...)`; o argumento é tudo até o primeiro `)`.
const CALL = /\bt\(\s*([^)]*)\)/g;
const LITERAL = /'([^']*)'|"([^"]*)"/g;

function sources(dir: string): string[] {
  return readdirSync(dir, { withFileTypes: true }).flatMap((entry) => {
    const full = join(dir, entry.name);
    if (entry.isDirectory()) {
      return sources(full);
    }
    // Testes não são interface: fixtures não contam como uso.
    if (!/\.(ts|svelte)$/.test(entry.name) || entry.name.endsWith('.test.ts')) {
      return [];
    }
    return [full];
  });
}

/** Chave → arquivo onde aparece, para a mensagem de falha ser acionável. */
function usedKeys(): Map<string, string> {
  const used = new Map<string, string>();
  for (const file of sources(SOURCE_DIR)) {
    const text = readFileSync(file, 'utf8');
    for (const call of text.matchAll(CALL)) {
      for (const literal of call[1].matchAll(LITERAL)) {
        used.set(literal[1] ?? literal[2], file.slice(SOURCE_DIR.length));
      }
    }
  }
  return used;
}

test('o catálogo não está vazio', () => {
  assert.ok(messageKeys.length > 0);
});

test('toda chave usada existe no catálogo', () => {
  const catalog = new Set<string>(messageKeys);
  const missing = [...usedKeys()].filter(([key]) => !catalog.has(key));
  assert.deepEqual(
    missing.map(([key, file]) => `${key} (${file})`),
    [],
    'chave usada sem entrada no catálogo',
  );
});

test('nenhuma chave do catálogo fica órfã', () => {
  const used = usedKeys();
  assert.deepEqual(
    messageKeys.filter((key) => !used.has(key)),
    [],
    'chave do catálogo que ninguém usa',
  );
});
