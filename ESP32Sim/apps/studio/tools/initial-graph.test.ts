// AP-01: o bundle inicial não pode importar three.js.
//
// A verificação do ADR 0007 é "teste que falha se o grafo do bundle inicial
// importar three.js ou o analisador". Aqui o grafo vem do próprio bundler
// (Vite 8/Rolldown): parte do chunk de entrada e segue apenas `imports`
// estáticos. `dynamicImports` fica de fora de propósito — é o que mantém
// three.js fora do carregamento inicial.
//
// Quando existir o analisador lógico, ele entra na mesma lista de proibidos.
import assert from 'node:assert/strict';
import test from 'node:test';
import { fileURLToPath } from 'node:url';
import { build } from 'vite';

const ROOT = fileURLToPath(new URL('..', import.meta.url));
const THREE = /node_modules[\\/]three[\\/]/;

// Só o que este teste usa da saída do bundler. Declarar o formato aqui evita
// depender dos tipos do Rollup/Rolldown, que mudam entre versões; as asserções
// abaixo falham se o formato real divergir.
type Chunk = {
  type: 'chunk';
  fileName: string;
  isEntry: boolean;
  imports: string[];
  dynamicImports: string[];
  modules: Record<string, unknown>;
};

test('o grafo do bundle inicial não inclui three.js', async () => {
  // Usa o vite.config.ts real (mesmos plugins e assetsInlineLimit): o teste
  // mede o build que vai para produção, não uma configuração paralela.
  const result = (await build({
    root: ROOT,
    configFile: fileURLToPath(new URL('../vite.config.ts', import.meta.url)),
    logLevel: 'silent',
    build: { write: false },
  })) as unknown as { output: Array<Chunk | { type: 'asset' }> };

  const chunks = result.output.filter((item): item is Chunk => item.type === 'chunk');
  const entry = chunks.find((chunk) => chunk.isEntry);
  assert.ok(entry, 'build sem chunk de entrada');
  assert.ok(entry.modules && typeof entry.modules === 'object', 'bundler sem chunk.modules');

  const byFile = new Map(chunks.map((chunk) => [chunk.fileName, chunk]));

  // Fecho estático a partir da entrada.
  const reachable = new Set<string>();
  const queue: Chunk[] = [entry];
  while (queue.length > 0) {
    const chunk = queue.pop()!;
    if (reachable.has(chunk.fileName)) {
      continue;
    }
    reachable.add(chunk.fileName);
    for (const name of chunk.imports) {
      const next = byFile.get(name);
      assert.ok(next, `chunk importado mas ausente do build: ${name}`);
      queue.push(next);
    }
  }

  const offenders = [...reachable].flatMap((name) =>
    Object.keys(byFile.get(name)!.modules)
      .filter((id) => THREE.test(id))
      .map((id) => `${name} ← ${id}`),
  );
  assert.deepEqual(offenders, [], 'three.js alcançável por import estático');

  // Sem isto o teste passaria se three.js simplesmente não fosse usado.
  const anywhere = chunks.flatMap((chunk) =>
    Object.keys(chunk.modules).filter((id) => THREE.test(id)),
  );
  assert.ok(anywhere.length > 0, 'three.js não aparece no build: o teste não prova nada');
  assert.ok(
    entry.dynamicImports.length > 0,
    'a cena precisa continuar em import dinâmico (AP-01)',
  );
});
