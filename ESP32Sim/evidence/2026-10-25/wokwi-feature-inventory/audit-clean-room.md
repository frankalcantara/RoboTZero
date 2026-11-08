# Parecer clean-room — EV-2026-10-25-02 (`wokwi-feature-inventory`)

- **Data do parecer:** 2026-10-25
- **Auditor:** `clean-room-auditor` (somente leitura)
- **Sessão:** `ses_e8e460965ffeIwwCwIu1PQxiWP`
- **Objeto:** `README.md`, `inventory.md`, `sources.md`, `notes.md`,
  `artifacts/raw-sha256.txt`, `artifacts/LICENSE-wokwi-docs.txt`
- **Veredito:** `needs-attribution`
- **Natureza:** registro de engenharia, não parecer jurídico. O auditor não
  acessou `wokwi.com`; afirmações sobre o ToS permanecem `unknown` e dependem
  de [`../wokwi-c3-diagram-contract/gate.md`](../wokwi-c3-diagram-contract/gate.md).

O texto abaixo é o parecer recebido. A resposta do lead, com o status de cada
correção, está na seção final.

---

```text
Veredito: needs-attribution
Escopo revisado: README.md, inventory.md, sources.md, notes.md,
                 artifacts/raw-sha256.txt, artifacts/LICENSE-wokwi-docs.txt
                 + conferência linha a linha contra as 7 páginas do commit pinado
                 (baixadas em raw.githubusercontent.com, sem acessar wokwi.com)
```

## 1. Licença e proveniência

| Verificação | Resultado |
| --- | --- |
| Licença da fonte na versão pinada | **MIT**, `LICENSE` do commit `5c90e53…`, "Copyright (c) 2021-2024 CodeMagic LTD" — confere com `artifacts/LICENSE-wokwi-docs.txt` (byte a byte: hash e 1.085 bytes batem) |
| Compatibilidade com distribuição | Compatível. Nenhuma GPL/AGPL/AGPL-rede envolvida nesta ficha |
| Atribuição | Correta e completa em `sources.md:42-47` (repo + commit + copyright + licença + ponteiro para o texto) |
| Assets / screenshots / imagens / 3D / firmware | **Nenhum.** Só 2 artefatos, ambos textuais. O diretório não contém PNG/SVG/glTF |
| Dados pessoais, tokens, cookies, segredos | **Nenhum.** Os literais presentes (MAC `24:0a:c4:00:01:10`, IPs `10.10.0.2`/`10.13.37.2`, BSSID) são defaults públicos documentados, não PII |
| Material proprietário do simulador | **Nenhum.** `sources.md:52-59` registra corretamente que `wokwi.com`, projetos da comunidade, bundles, WASM e imagens não foram usados |
| Reprodução de texto da fonte | Verificada mecanicamente (n-gramas ≥ 6 palavras): **5 ocorrências, todas factuais/curtas** — literais de config (MAC/BSSID), um título de seção citado como fonte, e `H-014` (tradução quase literal de 7 palavras de uma nota técnica). Nenhum trecho substancial |

## 2. Hashes — **8/8 reproduzidos**

Reexecutei o download no commit pinado e recalculei SHA-256 e tamanho: os 8
arquivos de `artifacts/raw-sha256.txt:9-16` batem **exatamente** em hash e
bytes (`LICENSE` 1085; `esp32.md` 18236; `serial-monitor.md` 6420;
`diagram-editor.md` 6514; `logic-analyzer.md` 8254; `debugger.md` 2386;
`esp32-wifi.md` 11526; `libraries.md` 2874). O `LICENSE` local é idêntico ao do
commit.

## 3. Fidelidade — alta; nenhum overclaim C3↔Uno/Mega

Conferi as **85 linhas** contra as páginas. Os itens de maior risco estão
corretamente qualificados: `D-001..D-004` (AVR-only), `S-001` (Uno/Mega),
`F-013` (`chipRevision` só P4), `F-010` (`partitions.csv` só web), `H-023`
(distinção ❌ vs —). Achados menores:

| Item | Evidência | Risco | Ação |
| --- | --- | --- | --- |
| `F-003` "Rust (std)" | `esp32.md:43` diz apenas "Rust projects"; o qualificador "std" vem de `esp32-wifi.md:63` | Overclaim leve + citação imprecisa | Trocar fonte para `esp32.md` §Getting Started + `esp32-wifi.md` §Connecting from Rust (std) |
| `F-001` "projetos ESP-IDF no studio" | `esp32.md:41` diz "ESP32 Arduino Core projects (including ESP-IDF projects)"; `esp32.md:91` diz que os exemplos IDF são compilados com o core Arduino | Ambiguidade sobre o que "rodar IDF" significa | Acrescentar "compilados via Arduino core" |
| `H-014` | `esp32.md:128` — tradução quase literal de 7 palavras | Nenhum (MIT + atribuição presente), mas contradiz a afirmação absoluta | Ver §4 abaixo |
| `B-001`/`B-002` | O conteúdo está na introdução de `libraries.md:8-10`, **antes** de "## Adding third party libraries" | Citação de seção imprecisa | Corrigir para "intro / §Adding third party libraries" |
| `L-006` | Mistura Web (download `wokwi-logic.vcd`) e VS Code (`wokwi.vcd`, `vcdFile`) — `logic-analyzer.md:42-56` | A tabela L não tem coluna `Superfície`, ao contrário do que `notes.md:52-62` afirma | Adicionar coluna ou qualificar a linha |

## 4. Disciplina de classificação

- **"observado (documental)"**: usado de forma consistente e explicitamente
  definido (`README.md:102`, `inventory.md:9-11`), mas é **um nível novo** que
  não existe na taxonomia de `evidence/README.md:79-86` ("Observado = visto
  diretamente em captura/log"). Não é overclaim, é diluição de vocabulário.
- **Coluna de relevância**: corretamente marcada como avaliação do lead em 5
  lugares (`inventory.md:12`, `README.md:54`, `README.md:114`, `notes.md:13`,
  `notes.md:31`). **Aprovado.**
- **Ressalva real**: `inventory.md:96` (`S-001`) afirma uma **ausência** ("para
  o C3 não está declarado") e ao mesmo tempo carrega `MVP`. A parte C3 é
  `unknown`, não "observado". Deve ser partida em fato documentado (Uno/Mega) +
  `U-001`.

## 5. Contradições internas

| # | Onde | Problema |
| --- | --- | --- |
| C1 | `README.md:8,142-143,153,169` | `audit-clean-room.md` e `MANIFEST.sha256` **não existem** no diretório (só 4 `.md` + 2 artefatos). §12 promete "resultado registrado abaixo" e não há nada abaixo. Integridade **não verificável** hoje → `unknown` até o fechamento |
| C2 | `README.md:83-84` e `notes.md:31-32` | Contagem "11 Fora" não fecha com a tabela: contei **30 MVP / 32 Fase 2 / 12 Fase 3 / 10 Fora** (7 `Fora` + 3 `Fora (C3)`) **+ 1 `—`** (`H-023`). O "11" só existe se `H-023` for dobrado em Fora |
| C3 | `inventory.md:125-127`, `inventory.md:73` | Dois rótulos fora da legenda (`Fora (C3)`, `—`), definida em `inventory.md:12-14` |
| C4 | `inventory.md:96` vs `README.md:92`/`notes.md:109` | A mesma incerteza tem dois IDs: `UN-007` (ficha irmã `wokwi-c3-diagram-contract`) e `U-001` (esta ficha) |
| C5 | `inventory.md:7-8` | "**Não há reprodução de texto**" é absoluto demais: há tradução quase literal em `H-014` e títulos de seção citados |
| C6 | `README.md:35-39` | As cláusulas do ToS são apresentadas como fato, mas só existem em `gate.md` (sem hash do texto). Não revalidei (não acessei `wokwi.com`) → **`unknown`** para mim |
| C7 | `README.md:30` | Cita Node.js v22.23.4 e "API GitHub" como ferramentas, mas `sources.md:63-82` só reproduz PowerShell/raw. Detalhe não reproduzível |

Contagens de área conferem: F14/H23/E12/S10/L8/D4/W9/B5 = **85**, sem IDs
duplicados.

## 6. Utilizável como base de escopo?

**Sim.** É descritivo, tem fonte por linha, não prescreve requisitos
(`README.md:122-132`, `notes.md:1-15`, `notes.md:94-101`) e mantém o alvo
`blocked`. Faltam três avisos:

1. **Nomes que carregam marca do terceiro não devem ser replicados no produto**:
   `Wokwi-GUEST`, `Wokwi IoT Gateway`, `$serialMonitor`, `wokwi-logic.vcd`,
   `host.wokwi.internal`, `@wokwi:`. Uso de nomes de config públicos para
   interoperabilidade é permitido por `docs/08` §1.2.3; identidade de marca não
   é (`docs/08:146-154`).
2. **Escopo da MIT**: cobre a documentação, não o software do simulador nem
   eventuais marcas/imagens do repo (está em `notes.md:99-100`, deveria estar
   também em `README.md` §2/§3).
3. **MAC default `24:0a:c4:…` é OUI da Espressif**; replicá-lo num dispositivo
   virtual é decisão de produto (preferir MAC localmente administrado).

## 7. Obrigações

- MIT: manter aviso de copyright + texto da permissão em cópias/substanciais.
  **Já cumprido** no repositório (`sources.md:42-47` +
  `artifacts/LICENSE-wokwi-docs.txt`).
- Registrar a fonte em `THIRD_PARTY_NOTICES` **quando ele existir**
  (`docs/08:173` — o arquivo ainda não existe em `ESP32Sim/`).
- Se a ficha for redistribuída (repo público/docs), a atribuição deve acompanhar
  o material derivado.
- Nenhuma obrigação de código-fonte, patente ou de marca decorre desta fonte; a
  marca "Wokwi" aqui é uso nominativo de proveniência.

## 8. Bloqueios

- **Licença: nenhum.** Não há motivo para bloquear a ficha.
- **Integridade: 1 pendência bloqueante para citação formal** —
  `MANIFEST.sha256` ausente enquanto `README.md:151-153` afirma verificação. Até
  existir, tratar a afirmação "entregues verificados" como `unknown` (as
  **fontes** eu verifiquei: 8/8).

## 9. Correções obrigatórias

1. Criar `MANIFEST.sha256` e preencher `README.md` §12, **ou** remover as
   referências a `MANIFEST.sha256` e a `audit-clean-room.md` até existirem.
2. Corrigir a contagem de relevância (`README.md:83-84`, `notes.md:31-32`) para
   `30/32/12/10 Fora + 1 não aplicável (H-023)`, ou renomear `H-023` para `Fora`
   de forma explícita.
3. Alinhar rótulos à legenda: `Fora (C3)` → `Fora`; `—` → rótulo declarado
   (ex.: `n/a`).
4. Desambiguar `UN-007` × `U-001` em `inventory.md:96` (referência cruzada
   explícita à ficha irmã).
5. Suavizar `inventory.md:7-8` para "nenhum trecho substancial reproduzido;
   notas técnicas curtas parafraseadas/traduzidas, sob MIT com atribuição".

## 10. Recomendações não bloqueantes

- Registrar o nível "observado (documental)" em `evidence/README.md`
  (taxonomia), ou renomear para `documentado`.
- Dividir `S-001` em fato documentado (Uno/Mega) + `U-001`; marcar `H-023` como
  agregado de ausências, não como funcionalidade.
- Adicionar coluna `Superfície` à tabela L (ou qualificar `L-006`).
- Corrigir citações `F-003`, `F-001`, `B-001`, `B-002`.
- Emitir o aviso de independência (`docs/08:152-153`) no README do repositório
  antes de qualquer publicação.
- Registrar em ADR a decisão de **não** reutilizar identificadores de marca do
  terceiro (`Wokwi-GUEST`, `@wokwi:`, `$serialMonitor`) no produto.

> Este parecer é registro de engenharia, não parecer jurídico. Não acessei
> `wokwi.com`; toda afirmação sobre o ToS permanece `unknown` e depende de
> `gate.md`.

---

## 11. Resposta do lead

### 11.1 Correções obrigatórias

| # | Correção | Status | Onde |
| --- | --- | --- | --- |
| 1 | Criar `MANIFEST.sha256` e preencher `README.md` §12 com o resultado real | corrigido | `MANIFEST.sha256`; `README.md` §11/§12 |
| 2 | Contagem `30/32/12/10 Fora + 1 n/a (H-023)` | corrigido | `README.md` §6; `notes.md` §2 |
| 3 | `Fora (C3)` → `Fora`; `—` → `n/a` | corrigido | `inventory.md` §6 e §2 |
| 4 | Desambiguar `UN-007` × `U-001` | corrigido | `inventory.md` §4 (nota) e §10; `notes.md` §3.2 |
| 5 | Suavizar a afirmação absoluta sobre reprodução de texto | corrigido | `inventory.md` cabeçalho |

### 11.2 Recomendações não bloqueantes

| Recomendação | Status | Onde |
| --- | --- | --- |
| Registrar "observado (documental)" na taxonomia | corrigido | `../README.md` §Níveis de confiança |
| Dividir `S-001` (Uno/Mega) + `U-001`; `H-023` como agregado | corrigido | `inventory.md` §4 e §2 |
| Coluna `Superfície` na tabela L | corrigido | `inventory.md` §5 |
| Citações `F-003`, `F-001`, `B-001`, `B-002` | corrigido | `inventory.md` §1 e §8 |
| Aviso de independência no README do repositório | **pendente** | fora desta ficha; exige decisão do lead antes de publicação |
| ADR sobre não reutilizar identificadores de marca | **pendente** | decisão de produto; recomendação registrada em `README.md` §2.1 |

### 11.3 Contradições internas

| # | Status | Onde |
| --- | --- | --- |
| C1 | corrigido | `audit-clean-room.md` e `MANIFEST.sha256` criados; `README.md` §12 preenchido |
| C2 | corrigido | `README.md` §6; `notes.md` §2 |
| C3 | corrigido | `inventory.md` §2 e §6 |
| C4 | corrigido | `inventory.md` §4 (nota) e §10; `notes.md` §3.2 |
| C5 | corrigido | `inventory.md` cabeçalho |
| C6 | corrigido | `README.md` §3 (nota de `unknown`) |
| C7 | corrigido | `README.md` §2 (ferramentas) |

### 11.4 Avisos acrescentados

- Escopo da MIT (documentação ≠ software), nomes de marca e OUI da Espressif:
  `README.md` §2.1.
- Nível "observado (documental)" na taxonomia: `../README.md`.

### 11.5 O que permanece

- O alvo `wokwi.com` continua **`blocked`**; nada foi observado em runtime.
- `UN-007` (ficha irmã) e `U-001` (esta ficha) continuam `unknown`.
- `THIRD_PARTY_NOTICES` ainda não existe em `ESP32Sim/`; a obrigação MIT está
  cumprida nesta ficha por `sources.md` + `artifacts/LICENSE-wokwi-docs.txt`.
- Nenhuma implementação foi iniciada.
