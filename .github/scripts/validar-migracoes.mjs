// Valida as migrações do D1 sem rede: nome, ordem e SQL.
// Cada diretório `migrations/` do repositório é aplicado, na ordem, em um
// SQLite em memória — o mesmo motor do D1.
//
// Uso: node .github/scripts/validar-migracoes.mjs

import { readdirSync, readFileSync } from 'node:fs';
import { join, relative } from 'node:path';
import { DatabaseSync } from 'node:sqlite';

const RAIZ = process.cwd();
const IGNORADOS = new Set([
  '.git',
  '.wrangler',
  'node_modules',
  'build',
  // Outros projetos do repositório não entram no gate do ESP32Sim.
  'AuralFocus',
  'Docs',
  'LineFollower',
  'LineFollower_Simulator',
]);
// Convenção do `wrangler d1 migrations create`.
const PADRAO = /^(\d{4})_[a-z0-9_]+\.sql$/;

const diretorios = [];
function procurar(dir) {
  for (const entrada of readdirSync(dir, { withFileTypes: true })) {
    if (!entrada.isDirectory() || IGNORADOS.has(entrada.name)) continue;
    const caminho = join(dir, entrada.name);
    if (entrada.name === 'migrations') diretorios.push(caminho);
    else procurar(caminho);
  }
}
procurar(RAIZ);

if (diretorios.length === 0) {
  console.log('::notice::Nenhum diretório migrations/ encontrado; nada para validar.');
  process.exit(0);
}

const erros = [];
let total = 0;

for (const dir of diretorios) {
  const nomeDir = relative(RAIZ, dir).replaceAll('\\', '/') || dir;
  const arquivos = readdirSync(dir)
    .filter((nome) => nome.endsWith('.sql'))
    .sort();

  if (arquivos.length === 0) {
    console.warn(`::warning::${nomeDir}/ não tem arquivos .sql.`);
    continue;
  }

  const errosAntes = erros.length;
  const versoes = new Map();
  const migracoes = [];
  for (const arquivo of arquivos) {
    const nome = `${nomeDir}/${arquivo}`;
    const casou = PADRAO.exec(arquivo);
    if (!casou) {
      erros.push(`${nome}: nome inválido (use NNNN_nome_em_snake_case.sql).`);
      continue;
    }
    if (versoes.has(casou[1])) {
      erros.push(`${nome}: versão ${casou[1]} duplicada com ${versoes.get(casou[1])}.`);
      continue;
    }
    versoes.set(casou[1], arquivo);

    const sql = readFileSync(join(dir, arquivo), 'utf8');
    if (sql.trim() === '') {
      erros.push(`${nome}: arquivo vazio.`);
      continue;
    }
    migracoes.push({ nome, sql });
  }

  if (erros.length > errosAntes) continue;

  const db = new DatabaseSync(':memory:');
  try {
    for (const { nome, sql } of migracoes) {
      try {
        db.exec(sql);
      } catch (erro) {
        erros.push(`${nome}: ${erro.message}`);
        break;
      }
    }
    if (erros.length === errosAntes) {
      total += migracoes.length;
      console.log(`${nomeDir}/: ${migracoes.length} migração(ões) válida(s).`);
    }
  } finally {
    db.close();
  }
}

if (erros.length > 0) {
  for (const erro of erros) console.error(`::error::${erro}`);
  console.error(`\n${erros.length} problema(s) encontrado(s).`);
  process.exit(1);
}

console.log(`\n${total} migração(ões) validada(s) em ${diretorios.length} diretório(s).`);
