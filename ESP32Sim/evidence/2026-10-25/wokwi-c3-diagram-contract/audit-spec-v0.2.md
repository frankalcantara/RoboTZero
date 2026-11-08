# Reauditoria clean-room — spec v0.2

- **Agente:** `clean-room-auditor` (subagente)
- **Sessão:** `ses_e8e706c85ffex7hj7t2E14P4kb`
- **Data:** 2026-10-25
- **Modo:** somente leitura; nenhum arquivo criado, editado ou apagado
- **Objeto:** evidência `2026-10-25/wokwi-c3-diagram-contract` e `spec.md` v0.2
- **Classificação:** **`needs-attribution`**
  - eixo licença/proveniência: `needs-attribution` (MIT OK; aviso ainda fora do
    caminho de distribuição)
  - eixo spec v0.2: `clean` para implementar o adaptador, com 1 correção
    obrigatória + nits editoriais

> O auditor não acessou `wokwi.com` nesta revisão. Avaliação de engenharia,
> não parecer jurídico. Texto reemitido pelo próprio auditor sem alteração de
> conclusões.

---

## Parecer de reauditoria — `evidence/2026-10-25/wokwi-c3-diagram-contract` (spec v0.2)

```text
Veredito: needs-attribution
  eixo licença/proveniência: needs-attribution (MIT OK; aviso ainda fora do caminho de distribuição)
  eixo spec v0.2: clean para implementar o adaptador, com 1 correção obrigatória + nits editoriais
```

**Escopo revisado:** os 10 arquivos entregues (`README`, `gate`, `sources`,
`notes`, `spec` v0.2, `handoff`, `audit-clean-room`, `audit-spec`,
`MANIFEST.sha256`, `artifacts/*`). Verificações executadas: `Get-FileHash` do
`MANIFEST` (local) e **recálculo in-memory** dos 5 fontes contra
`raw.githubusercontent.com/wokwi/wokwi-docs@5c90e532…` + contagens de
ocorrência e leitura dos trechos normativos. Não acessei `wokwi.com`; nenhum
arquivo foi criado, editado ou apagado.

### Verificações que eu reproduzi

| Verificação | Resultado |
| --- | --- |
| `MANIFEST.sha256` (§6.2 do README) | **10/10 `OK`** |
| SHA-256 + tamanho dos 5 fontes upstream | **5/5 idênticos** a `artifacts/raw-sha256.txt` (inclusive `LICENSE` = `43095626…fc67`, 1085 B) |
| `artifacts/LICENSE-wokwi-docs.txt` | MIT integral, "Copyright (c) 2021-2024 CodeMagic LTD" — igual ao hash upstream |
| Contagens de §6.3 / `notes.md` §4 | **conferem**: `board-esp32-c3-devkitm-1`=1; `wokwi-led`=5 e 9; `$serialMonitor`=8 e 1; `serialInterface`=1; `USB_SERIAL_JTAG`=1 |
| Fidelidade de C-02/C-03/C-05/C-06/C-09/C-10/C-12 | **fiel** ao upstream |
| Segredos/tokens/cookies/dados pessoais | **nenhum** (só prosa sobre a política) |

### Achados por item pedido

- **A1 — clean (com ressalvas processuais).** `MANIFEST.sha256` e
  `audit-spec.md` existem e o comando passa 10/10; README §6.2 agora aponta
  para §13 (referência circular corrigida). Ressalvas: (a) `audit-spec.md:165`
  diz *"ver §Reauditoria abaixo"* e **essa seção não existe** — a reauditoria é
  este parecer e deve ser registrada em arquivo próprio; (b) o arquivo
  hasheado como parecer do auditor contém seção de autoria do lead (bem
  delimitada, mas mistura autoria em artefato de auditoria); (c) o `MANIFEST`
  não se auto-cobre e seu hash não está registrado em commit/ADR — integridade
  fraca contra regeneração; (d) qualquer edição futura de `audit-spec.md`
  quebra a verificação (procedimento: regenerar o manifest no mesmo commit).
- **A2 — clean.** `sources.md` §2 usa `SRC-001..SRC-005`; grep confirma
  **zero** `EV-` em `sources.md` e **zero** `SRC-` em `README`/`spec`.
  Colisão resolvida. Cosmético: `README` §7 cita caminho de arquivo em vez de
  `SRC-*`.
- **A3 — clean, com 1 resíduo relevante.** `UN-009` existe (`README` §9,
  `spec` §7) e `GPIO8`/`GND` desapareceram da spec; `blink-c3-minimal` está
  marcada como bloqueada em `spec` §6, §8 e `handoff` §4/§9. **Resíduo (maior
  risco remanescente):** a fixture `blink-c3-serial-explicit` (`spec` §6) liga
  `$serialMonitor:TX/RX` aos "pinos RX/TX da UART" da placa C3 — depende de
  nomes de pino da placa, mas **não** está marcada como bloqueada por UN-009.
  É a mesma classe de lacuna que empurra o implementador ao alvo. Secundário:
  `README` DV-002 ainda diz "um pino **GPIO** da placa" e "um pino de
  referência".
- **A4 — parcial.** C-08 está corretamente qualificado: verifiquei que
  auto-attach + auto-baud aparecem **somente** em `serial-monitor.md`
  §"Arduino Uno and Mega" (l. 12-14) e que "só mostra com saída" é genérico
  (l. 30 + tabela `display` l. 109). Porém a qualificação **não** foi
  propagada: `README` §5 **H5** continua "confirmada (documental)" para a
  afirmação genérica, e `notes.md` §2.4 repete "Anexa automaticamente à UART de
  hardware e detecta o baud rate"; `DV-003` idem. Nit: C-08 cita só `EV-008`,
  mas a metade "display auto" é `EV-009`.
- **A5 — clean em C-01.** C-01 agora diz "aparecem na estrutura básica; a
  obrigatoriedade não é declarada (UN-002)" — coerente com o upstream (só a
  seção *Parts* marca required/optional). Resíduo menor: `DV-002`
  ("opcionalmente, `serialMonitor`") e `notes` §2.1 ("bloco opcional") mantêm a
  inferência que A5 mandou retirar.
- **A6 — clean, com 3 lacunas.** Namespace `$` reservado (terminologia), `$`
  proibido em `parts[].id` (R-08 + §5.1 passo 5), pseudo-partes resolvidas
  pelo adaptador, erro dedicado `unknown_pseudo_part` em §5.3. Faltam:
  (a) **código de erro para `$` em `parts[].id`** (nenhum dos 13 códigos
  cobre); (b) exigência explícita de **reemitir no export** as conexões com
  pseudo-parte (R-10 fala em preservar ordem, R-14 fala do bloco — nenhum diz
  "conexões `$serialMonitor:*` são reemitidas"); (c) fixture para
  `unknown_pseudo_part`. Nit de nível: C-11 classifica `$serialMonitor` como
  "identificador reservado" em nível **observado**, mas a doc só o usa em
  `connections` — é `derivado`.
- **A7 — clean.** R-14 existe, valida enums e gera `invalid_serial_monitor`;
  R-10 foi estendido a C-09/C-10 e define ordem fixa de chaves; fixture
  `serial-monitor-roundtrip` presente.
- **A8 — clean, com nits.** Cobertura confirmada: limites (R-15 +
  `limit_exceeded`), XSS (R-16), prototype pollution + chaves duplicadas
  (R-17), tipo de `version` (R-18 + `invalid_version_type`), campos
  ausentes/formato de conexão (R-19), enums (R-14). Nits: R-17 diz
  "rejeitadas (`duplicate_key`) **ou** tratadas por parser seguro documentado",
  enquanto §5.1 passo 1 manda rejeitar — escolher um; faltam fixtures para
  `invalid_version_type`, `duplicate_key`, `unknown_pseudo_part`,
  `invalid_serial_monitor`, `unknown_pin` e R-16.
- **A9 — registrado, pendente (não criado).** Obrigação MIT em `handoff`
  §4/§9, `sources` §4 e `gate` §8. Verifiquei que `docs/08` §2 **não**
  menciona `wokwi/wokwi-docs`; essa pendência específica não está explícita no
  handoff (só `THIRD_PARTY_NOTICES`/`provenance/`).
- **A10 — registrado, pendente e real.** `docs/01-avaliacao-rea.md:228` ainda
  diz "Adotar condicionalmente" e `docs/08` R-01 não foi atualizado. Dívida
  confirmada; não bloqueia esta ficha.
- **A11 — clean, com contradição residual.** `EV-013` está "observado
  (contextual, não normativo)" no `README` §7 e no cabeçalho da spec, e nenhum
  C o usa. Mas `notes.md` §3 ainda diz "Observado | EV-001 a EV-013 | pode
  virar requisito? **sim**, com citação" — contradiz a marcação.

### Contradições, overclaims e artefatos ausentes

1. `audit-spec.md:165` referencia seção "Reauditoria" inexistente (A1a).
2. `notes.md` §3 vs. marcação "não normativo" de EV-013.
3. `README` H5 / DV-002 / DV-003 e `notes` §2.4 mantêm auto-attach genérico
   (resíduo de A4).
4. `spec` C-07: "depois de `*`, para o destino, **em ordem reversa**" é
   ambíguo; no upstream (l. 139-153) só as instruções **após** `*` são
   aplicadas em ordem reversa, as anteriores seguem a ordem escrita. Corrigir a
   redação.
5. C-03 apresenta tradução entre aspas ("pode não funcionar corretamente");
   upstream: *"the simulation may not function correctly"* — marcar como
   tradução.
6. `blink-c3-serial-explicit` sem marcação de bloqueio por UN-009.
7. Nenhum artefato citado está ausente além do item 1; `provenance/` e
   `THIRD_PARTY_NOTICES` são futuros e explicitamente pendentes.
8. **Nada fabricado**: hashes, tamanhos, contagens e comandos reproduzem
   (5/5 upstream, 10/10 local).

### Obrigações

- **MIT:** incluir copyright + texto integral + URL + commit em
  `THIRD_PARTY_NOTICES` **antes** de qualquer distribuição do adaptador/spec;
  registrar `wokwi/wokwi-docs` em `docs/08` §2.
- **Mídia:** não copiar imagens/GIFs/SVGs do `wokwi-docs` (o MIT do repo pode
  não cobrir obras de terceiros ali embutidas).
- **Marcas:** `wokwi-led` / `board-esp32-c3-devkitm-1` apenas como strings de
  interoperabilidade; nada de "Wokwi" em nome/logo/domínio; disclaimer de
  independência em material público.
- **Alvo:** `blocked`; nenhum contato/feedback; nenhum firmware nosso criado no
  alvo.
- **Ferramenta do fornecedor:** `wokwi-cli lint` (citado em
  `diagram-format.md:70`) segue fora de escopo sem revisão de licença —
  corretamente registrado em `handoff` §6.

### Bloqueios

1. **Distribuição:** não publicar o adaptador nem esta spec sem a entrada MIT
   em `THIRD_PARTY_NOTICES`.
2. **Fixtures dependentes de pino:** não criar `blink-c3-minimal` **nem
   `blink-c3-serial-explicit`** sem UN-009 resolvido por fonte licenciada
   (pinout Espressif citado ou `wokwi-elements` MIT com verificação por
   asset). Nada de pino inventado.
3. **Permanência:** nada aqui impede manter a evidência no repositório — o
   material é lícito e limpo.

### Próximos passos

1. Lead: marcar `blink-c3-serial-explicit` como dependente de UN-009 (1 linha
   em `spec` §6 + `handoff` §4) e adicionar código de erro para `$` em
   `parts[].id`.
2. Lead: registrar esta reauditoria em arquivo **separado** (ex.:
   `audit-spec-v0.2.md`), remover/renomear a referência a "§Reauditoria" e
   regenerar o `MANIFEST.sha256` no mesmo commit.
3. Lead: nits editoriais — C-07 (reversão só pós-`*`), R-17 (rejeitar, não
   "ou"), `notes` §3 (EV-013 não normativo), H5/DV-002/DV-003/`notes` §2.4
   (qualificar Uno/Mega), C-11 para nível `derivado`.
4. `verification-engineer`: fixtures faltantes (`invalid_version_type`,
   `duplicate_key`, `unknown_pseudo_part`, `invalid_serial_monitor`,
   `unknown_pin`) antes de fechar A8.
5. Lead: fechar A9 do gate (`docs/01` §5/§9, `docs/08` R-01) e criar
   `THIRD_PARTY_NOTICES` + `provenance/`.
6. `rea-evidence`: ficha de follow-up exclusiva para UN-009, sem tocar no alvo.
7. `clean-room-auditor`: revisar o diff do adaptador e os assets 3D quando
   existirem.

**Classificação final:** **`needs-attribution`** — proveniência e licença
`clean` (verificadas de forma independente), spec v0.2 sem overclaim de runtime
e sem `unknown` promovido; a obrigação MIT ainda não está no caminho de
distribuição e há uma correção obrigatória de bloqueio de fixture (UN-009)
antes de implementar.
