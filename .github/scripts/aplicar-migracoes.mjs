// Aplica no D1 remoto as migrações alteradas pelo merge na main.
// Uso (no CI): CHANGED_FILES="..." node .github/scripts/aplicar-migracoes.mjs
// Requer `wrangler` no PATH, CLOUDFLARE_API_TOKEN e CLOUDFLARE_ACCOUNT_ID.

import { execFileSync } from 'node:child_process';
import { existsSync, readFileSync } from 'node:fs';
import { dirname, join, resolve } from 'node:path';

const RAIZ = process.cwd();
const CONFIGS = ['wrangler.toml', 'wrangler.jsonc', 'wrangler.json'];

// Agrupa os arquivos alterados pela raiz do projeto (pai de `migrations/`).
const projetos = new Map();
for (const arquivo of (process.env.CHANGED_FILES ?? '').split(/\r?\n/)) {
  const partes = arquivo.trim().replaceAll('\\', '/').split('/');
  const indice = partes.indexOf('migrations');
  if (indice === -1) continue;
  const raizProjeto = indice === 0 ? '.' : partes.slice(0, indice).join('/');
  projetos.set(raizProjeto, resolve(RAIZ, raizProjeto, 'migrations'));
}

if (projetos.size === 0) {
  console.log('::notice::Nenhuma migração alterada; nada para aplicar.');
  process.exit(0);
}

if (!process.env.CLOUDFLARE_API_TOKEN || !process.env.CLOUDFLARE_ACCOUNT_ID) {
  console.error(
    '::error::Configure os segredos CLOUDFLARE_API_TOKEN e CLOUDFLARE_ACCOUNT_ID no repositório.',
  );
  process.exit(1);
}

function acharConfig(inicio) {
  let dir = resolve(RAIZ, inicio);
  while (true) {
    for (const nome of CONFIGS) {
      const caminho = join(dir, nome);
      if (existsSync(caminho)) return caminho;
    }
    if (dir === RAIZ) return null;
    const pai = dirname(dir);
    if (pai === dir) return null;
    dir = pai;
  }
}

function bancosToml(texto) {
  const bancos = [];
  let atual = null;
  for (const linha of texto.split(/\r?\n/)) {
    const limpa = linha.trim();
    if (limpa.startsWith('[')) {
      atual = limpa === '[[d1_databases]]' ? {} : null;
      if (atual) bancos.push(atual);
      continue;
    }
    if (!atual) continue;
    const par = /^([A-Za-z0-9_]+)\s*=\s*"([^"]*)"/.exec(limpa);
    if (par) atual[par[1]] = par[2];
  }
  return bancos;
}

function bancosJson(texto) {
  // Remove comentários e vírgulas finais do JSONC preservando strings.
  let saida = '';
  for (let i = 0; i < texto.length; i++) {
    const c = texto[i];
    if (c === '"') {
      const inicio = i;
      for (i++; i < texto.length && texto[i] !== '"'; i++) {
        if (texto[i] === '\\') i++;
      }
      saida += texto.slice(inicio, i + 1);
    } else if (c === '/' && texto[i + 1] === '/') {
      while (i < texto.length && texto[i] !== '\n') i++;
      saida += '\n';
    } else if (c === '/' && texto[i + 1] === '*') {
      i += 2;
      while (i < texto.length && !(texto[i] === '*' && texto[i + 1] === '/')) i++;
      i++;
    } else {
      saida += c;
    }
  }
  const dados = JSON.parse(saida.replace(/,(\s*[}\]])/g, '$1'));
  return Array.isArray(dados.d1_databases) ? dados.d1_databases : [];
}

function escolherBanco(bancos, config, migrationsDir) {
  const correspondentes = bancos.filter(
    (banco) => resolve(dirname(config), banco.migrations_dir ?? 'migrations') === migrationsDir,
  );
  if (correspondentes.length === 1) return correspondentes[0];
  if (bancos.length === 1) return bancos[0];
  return null;
}

let falhou = false;

for (const [raizProjeto, migrationsDir] of projetos) {
  const config = acharConfig(raizProjeto);
  if (!config) {
    console.error(
      `::error::${raizProjeto}: nenhum wrangler.toml/jsonc encontrado para as migrações.`,
    );
    falhou = true;
    continue;
  }

  const texto = readFileSync(config, 'utf8');
  const bancos = config.endsWith('.toml') ? bancosToml(texto) : bancosJson(texto);
  const banco = escolherBanco(bancos, config, migrationsDir);

  if (!banco) {
    console.error(
      `::error::${raizProjeto}: não foi possível identificar a [[d1_databases]] de ${config}.`,
    );
    falhou = true;
    continue;
  }

  // O nome do banco é estável; a binding pode mudar (recomendação do D1).
  const alvo = banco.database_name ?? banco.binding;
  if (!alvo) {
    console.error(`::error::${config}: binding de D1 sem database_name nem binding.`);
    falhou = true;
    continue;
  }

  console.log(`\nAplicando migrações de ${raizProjeto} em "${alvo}" (remoto).`);
  try {
    execFileSync('wrangler', ['d1', 'migrations', 'apply', alvo, '--remote', '--config', config], {
      cwd: dirname(config),
      stdio: 'inherit',
      env: { ...process.env, CI: 'true' },
    });
  } catch (erro) {
    console.error(`::error::${raizProjeto}: ${erro.message}`);
    falhou = true;
  }
}

if (falhou) process.exit(1);
console.log('\nMigrações aplicadas.');
