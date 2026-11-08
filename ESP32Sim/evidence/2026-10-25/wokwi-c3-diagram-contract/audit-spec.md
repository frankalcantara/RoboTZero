# Parecer clean-room — spec e evidência (v0.1)

- **Agente:** `clean-room-auditor` (subagente)
- **Sessão:** `ses_e8e82820fffewkJt0ymCIoMc1n`
- **Data:** 2026-10-25
- **Modo:** somente leitura; nenhum arquivo criado, editado ou apagado
- **Objeto:** evidência `2026-10-25/wokwi-c3-diagram-contract` e `spec.md` v0.1
- **Classificação:** **`needs-decision`**
  - eixo licença/proveniência: `clean` (atribuição MIT pendente de registro)
  - eixo especificação/traceabilidade: `needs-decision` (correções obrigatórias)

> O auditor não acessou `wokwi.com` nesta revisão. As cláusulas C1–C6 do ToS
> permanecem `unknown` para ele; o parecer anterior foi aceito como fonte.
> Avaliação de engenharia, não parecer jurídico.

---

# Parecer clean-room — evidência `2026-10-25/wokwi-c3-diagram-contract`

> Auditoria **somente leitura**. Nenhum arquivo foi criado, editado ou apagado. Avaliação de engenharia, não parecer jurídico.
> **Não acessei `wokwi.com`** nesta revisão (para não reintroduzir o risco registrado em `gate.md` §3 e `audit-clean-room.md` §6). As cláusulas C1–C6 do ToS permanecem **`unknown`** para mim; aceito o registro do parecer anterior como fonte.

```text
Veredito: needs-decision
  eixo licença/proveniência: clean (com obrigação de atribuição pendente de registro)
  eixo especificação/traceabilidade: needs-decision (correções obrigatórias antes de implementar)
```

**Escopo revisado:** os 9 arquivos pedidos + conferência do diretório (que contém apenas 6 `.md` e `artifacts/`), verificação independente de hashes contra `raw.githubusercontent.com` no commit pinado, reconferência das contagens de §6.3/`notes.md` §4, e leitura cruzada de `docs/01` §5/§9, `docs/03` §5/§7/§10, `docs/05` §4, `docs/06`, `docs/08`, `evidence/README.md`.

## Verificações que eu consegui reproduzir (resultado real)

| Verificação | Resultado |
| --- | --- |
| SHA-256 + tamanho dos 5 fontes contra `raw.githubusercontent.com/wokwi/wokwi-docs@5c90e532…` | **5/5 idênticos**, inclusive `LICENSE` = `43095626…fc67`, 1085 bytes |
| `artifacts/LICENSE-wokwi-docs.txt` local vs upstream | **byte-idêntico** (`43095626…fc67`) — atribuição MIT literal, não paráfrase |
| Contagens de `README.md` §6.3 / `notes.md` §4 | **conferem exatamente**: `board-esp32-c3-devkitm-1`=1; `wokwi-led`=5 e 9; `$serialMonitor`=8 e 1; `serialInterface`=1; `USB_SERIAL_JTAG`=1 |
| Conteúdo de EV-002..EV-012 vs documentos | **fiel**, exceto as qualificações dos achados A3–A5 abaixo |
| `MANIFEST.sha256` e `audit-spec.md` | **não existem** (ver A1) |
| Material do alvo (DOM, bundle, WASM, screenshot, asset, projeto da comunidade) | **nenhum** |
| Segredos, cookies, tokens, dados pessoais | **nenhum** (só o ID de sessão interno `ses_e8e8b2…`, que não é dado pessoal) |

## Achados

**A1 — `needs-decision` (alto). Artefatos autoreferenciados inexistentes.**
`evidência`: `README.md` §6.2, §13, §14 e `handoff.md` §2, §5.1, `spec.md` cabeçalho e §8 citam `MANIFEST.sha256` e `audit-spec.md`; nenhum dos dois existe no diretório. `README.md` §6.2 diz "resultado registrado… (ver §10)", mas §10 é "Limitações" e não contém resultado; `handoff.md` §5.1 devolve para "(ver §5.1)" — referência circular.
`risco`: a ficha afirma uma verificação de integridade dos entregues que nunca foi executada nem registrada; critério de aceite de `spec.md` §8 ("revisão clean-room registrada em `audit-spec.md`") está não cumprido, embora o arquivo seja citado como existente.
`ação`: gerar `MANIFEST.sha256` (hash dos 8 arquivos) e executar §6.2, ou remover as referências e declarar a pendência explicitamente. Idem para `audit-spec.md`.

**A2 — `needs-decision` (alto). Colisão de IDs de evidência.**
`evidência`: `sources.md` §2 usa `EV-001..EV-005` para **arquivos** (`EV-002` = `docs/diagram-format.md`, `EV-004` = `docs/parts/wokwi-led.md`); `README.md` §7 e `spec.md` §3 usam `EV-001..EV-013` para **fatos**. Ex.: C-04 rastreia "EV-004" (= fato do board id), que em `sources.md` é o arquivo do LED.
`risco`: rastreabilidade quebrada — o auditor/implementador que seguir o ID pelo `sources.md` chega ao arquivo errado.
`ação`: renomear as linhas de `sources.md` §2 para `SRC-001..SRC-005` e ajustar as citações.

**A3 — `needs-decision` (alto). Nomes de pino da placa C3 não têm fonte.**
`evidência`: `README.md` §8 DV-002 afirma que o bundle mínimo tem "duas conexões (GPIO→A e C→GND)"; `spec.md` §2 usa `GPIO8` como exemplo de pino; `spec.md` §6 exige a fixture `blink-c3-minimal`. Nenhuma das 5 páginas coletadas lista pinos de `board-esp32-c3-devkitm-1` (verifiquei: zero ocorrências de `GPIO\d`/`GND` nas páginas). `docs/diagram-format.md` §Connections diz literalmente *"You can find the name of a component pin by moving the mouse over it"* — isto é, a fonte documentada para nomes de pino é a **UI do alvo**, que está `blocked`.
`risco`: é exatamente o tipo de lacuna que empurra o implementador a "completar" observando o alvo (contaminação irreparável, `audit-clean-room.md` §2) ou a inventar nomes incompatíveis.
`ação`: criar `UN-009` ("nomes de pino da placa C3 não documentados nas páginas coletadas") e retirar `GPIO`/`GND`/`GPIO8` de DV-002 e de `spec.md` §2/§6; a fixture mínima fica bloqueada até fonte licenciada (ver próximos passos).

**A4 — `needs-decision` (médio). C-08 extrapola o escopo do documento.**
`evidência`: `docs/guides/serial-monitor.md` §"Arduino Uno and Mega" atribui **auto-attach à UART de hardware + auto-detecção de baud** a Uno/Mega. A metade "só aparece quando há saída" é genérica (tabela `display: auto` = default). `spec.md` C-08 enuncia as duas metades como contrato geral, aplicável ao C3.
`risco`: requisito de formato apoiado em comportamento de outra família de placas; alimenta a expectativa de que o C3 está coberto.
`ação`: qualificar C-08 ("documentado para Uno/Mega; para C3 ver UN-007/UN-001").

**A5 — `needs-decision` (médio). C-01 afirma opcionalidade não declarada.**
`evidência`: `spec.md` C-01 diz que `author`, `editor` e `serialMonitor` são "metadados opcionais". `docs/diagram-format.md` §File structure apenas os apresenta no exemplo básico; só a seção **Parts** marca explicitamente "required"/"optional".
`risco`: overclaim pequeno, mas é a diferença entre "documentado" e "inferido"; `UN-002` cobre só a obrigatoriedade em runtime.
`ação`: reescrever como "presentes na estrutura básica; obrigatoriedade não declarada (UN-002)".

**A6 — `needs-decision` (alto). Contradição interna: `$serialMonitor` vs. R-08.**
`evidência`: `spec.md` R-08 exige erro quando "parte… inexistente"; C-11 exige aceitar `$serialMonitor:TX|RX`, que **não é parte** de `parts[]`.
`risco`: a spec, como escrita, rejeita o caso que ela própria manda suportar; dois implementadores produzem comportamentos opostos.
`ação`: reservar o namespace `$` (pseudo-partes), proibir `$` em `parts[].id`, e definir resolução/exportação dessas conexões; adicionar código de erro dedicado.

**A7 — `needs-decision` (médio). Round-trip não cobre o bloco `serialMonitor`.**
`evidência`: R-10 rastreia apenas C-01..C-07; nenhum R exige preservar/emitir `serialMonitor` (C-09/C-10) nem validar `display`/`newline`. `spec.md` §6 não tem fixture para isso.
`risco`: exportar um projeto importado apaga ou altera silenciosamente a configuração de serial — perda de dados do usuário.
`ação`: estender R-10 (ou criar R-14) + fixture `serial-monitor-roundtrip`.

**A8 — `needs-decision` (médio). Faltam requisitos de validação/segurança.**
`evidência`: `spec.md` §5.3 lista 6 códigos de erro; não há requisito para: limite de tamanho/profundidade/nº de partes e conexões (o documento é entrada de usuário e vai para browser/WASM); escape de `label` e demais strings na UI (XSS); chaves duplicadas e `__proto__`/prototype pollution; tipo de `version` (`1` vs `"1"`); valor desconhecido em `display`/`newline` (erro ou preservar?); regra concreta de ordenação do export (R-10 diz "determinística" sem definir critério); `parts[].id` ausente/`type` ausente (não há código para isso).
`risco`: superfície de robustez e de segurança não especificada; `docs/08` R-17 (firmware malicioso) e §6 não são endereçados nesta fronteira.
`ação`: adicionar R-15..R-19 e fixtures correspondentes.

**A9 — `needs-attribution` (baixo). Obrigação MIT ainda não registrada no projeto.**
`evidência`: `artifacts/LICENSE-wokwi-docs.txt` preservado e correto; `sources.md` §4 traz o bloco de atribuição. Mas não existe `THIRD_PARTY_NOTICES` nem `provenance/` no repositório (`docs/08` §8), e `wokwi/wokwi-docs` não consta em `docs/08` §2.
`risco`: se o adaptador ou esta ficha forem publicados sem o aviso, há descumprimento formal do MIT (o texto do aviso existe, mas fora do caminho de distribuição).
`ação`: incluir entrada MIT (copyright + link + commit + texto) em `THIRD_PARTY_NOTICES` antes de qualquer distribuição pública do adaptador/spec.

**A10 — `needs-decision` (baixo). Pendência A9 do próprio gate segue aberta.**
`evidência`: `gate.md` §6 A9 exige atualizar `docs/01` §5/§9 e `docs/08` R-01; `docs/01` §9 continua "Adotar condicionalmente" com a condição 2 "Revisão de ToS/EULA concluída" não marcada, e `notes.md` §6 confirma que nenhum doc foi alterado.
`risco`: documentação de arquitetura ainda autoriza condicionalmente o REA no alvo, contradizendo o `blocked`.
`ação`: nota/ADR do lead. Não bloqueia esta ficha (que não usa o alvo), mas é dívida registrada.

**A11 — `clean`, com ressalva (baixo). EV-013 não é usado por nenhum C.**
`evidência`: EV-013 (matriz de features do C3) não é rastreado em C-01..C-12 e descreve capacidades de emulação, fora do escopo declarado em `README.md` §4.
`ação`: marcar como contextual/não normativo para evitar leitura de que a spec cobre periféricos.

**A12 — `clean` (informativo). Nenhum hash, comando ou resultado inventado.**
`evidência`: os 5 hashes e tamanhos de `artifacts/raw-sha256.txt` reproduzem **exatamente** contra o commit pinado; o comando PowerShell de §6.1/`sources.md` §7 é sintaticamente válido e executável; as contagens de consistência batem. Isto é o oposto de fabricação — é o ponto mais forte da ficha.

**A13 — `clean` (informativo). Separação clean-room respeitada.**
Nenhuma captura do alvo, nenhum artefato de `wokwi.com/projects/*`, nenhum asset; `gate.md` §3-4, `notes.md` §6 e o conteúdo real do diretório são coerentes entre si. `README.md` §12 ("material proprietário: não") é verdadeiro para o que está no diretório. As citações do ToS em `gate.md` §2 são trechos curtos de análise de conformidade — aceitável, mas mantenha-as curtas e não as redistribua em material público.

**A14 — `unknown` (baixo). Metadados não reproduzíveis.**
`evidência`: `README.md` §2 cita "Node.js v22.23.4 (`fetch`)" mas nenhum comando em §6.1/§6.2 usa Node; o timestamp `2026-10-25T02:10:34-03:00` não é verificável a posteriori (os hashes são).
`ação`: remover o Node da lista de ferramentas ou documentar o comando; tratar o timestamp como declaração, não como fato verificado.

## Respostas diretas às 9 perguntas

1. **Compatível com MIT e com o gate `blocked`?** **Sim.** Fonte é repositório MIT, commit pinado, conteúdo verificado byte a byte; nenhuma captura, asset, projeto da comunidade, DOM, bundle ou dado pessoal. O gate é respeitado e citado coerentemente.
2. **C-01..C-12 rastreiam EV-001..EV-013?** **Parcialmente.** C-01..C-12 mapeiam 1:1 para EV-002..EV-012 (A2 quebra a resolução do ID; EV-001 e EV-013 não são usados por nenhum C). Há **overclaim real** em C-08 (A4) e em DV-002/`GPIO8`/GND (A3); C-01 tem overclaim menor (A5). Não há afirmação de que o runtime do alvo aceita o contrato — pelo contrário, isso está explicitado como `unknown` em `spec.md` cabeçalho, H6, §7 UN-001. Esse ponto está correto.
3. **UN-001..UN-008 permanecem desconhecidos?** **Sim.** Nenhum foi promovido; `spec.md` §7 repete a proibição de promoção. Falta acrescentar `UN-009` (nomes de pino da placa) — ver A3.
4. **R-01..R-13 marcados como decisões de engenharia?** **Sim**, com a ressalva de que R-08 conflita com C-11 (A6) e R-10 é incompleto (A7). R-12 cita `docs/08` §1.2 corretamente; R-09 cita "arquitetura §7", que trata do studio mas não enuncia "serial é projeção de eventos" — §7.1/§10 seriam citações mais precisas (cosmético).
5. **Atribuição MIT adequada?** **Sim em conteúdo** (copyright 2021-2024 CodeMagic LTD + link + commit + texto integral verificado). **Pendente em local**: não há `THIRD_PARTY_NOTICES`/`provenance/` (A9).
6. **Contradições / invenções?** Sem invenções de hash, comando, versão ou resultado (A12). Há: artefatos citados e ausentes e referência circular de resultado (A1), colisão de IDs (A2), contradição `$serialMonitor` vs R-08 (A6), e EV-013 órfão (A11). `README`, `gate`, `notes` e `handoff` são mutuamente coerentes quanto ao `blocked` e ao escopo.
7. **Implementável sem observar o alvo?** **Ainda não integralmente.** Parse/emit da estrutura geral: sim. A **fixture mínima de blink**: não, porque faltam os nomes de pino da placa (A3). Faltam requisitos de segurança/validação listados em A8 e o round-trip do serial (A7).
8. **Classificação final:** **`needs-decision`** — a licença e a proveniência estão `clean`; a spec não pode virar base de implementação antes de A1, A2, A3, A6 (e idealmente A7, A8).
9. Ver abaixo.

## Obrigações

- MIT: preservar copyright + permissão em toda distribuição de porções substanciais; incluir `wokwi/wokwi-docs` em `THIRD_PARTY_NOTICES` (A9).
- **Não copiar imagens, GIFs ou SVGs do `wokwi-docs`** — o MIT do repositório pode não cobrir obras de terceiros ali embutidas (ex.: o GIF de `diagram-format.md` é creditado a "Steve Sigma"). Fatos e nomes de campo: OK; mídia: não.
- Identificadores `wokwi-led` e `board-esp32-c3-devkitm-1` só como strings de interoperabilidade (nominativo); nunca como prefixo de IDs internos, nome de produto, logo ou domínio (`docs/08` §5).
- Disclaimer "Projeto independente, não afiliado à Espressif ou à Wokwi" em qualquer material público que mencione a Wokwi.
- Nenhum contato/feedback à Wokwi; nenhum firmware nosso criado no alvo; alvo permanece `blocked`.

## Bloqueios

1. **Não usar `spec.md` 0.1 como base de implementação** enquanto A1, A2, A3 e A6 não forem corrigidos — em especial A3, que é a lacuna com maior probabilidade de levar o implementador ao alvo.
2. **Não criar a fixture `blink-c3-minimal`** com nomes de pino inventados; sem fonte licenciada, ela é `unknown`.
3. **Nada aqui bloqueia manter esta evidência no repositório** (o material é lícito e limpo); o bloqueio é de *uso da spec*, não de permanência.

## Próximos passos

1. Lead: corrigir A1/A2/A5/A11 (edição documental simples) e publicar `MANIFEST.sha256` + `audit-spec.md` reais, ou remover as referências.
2. Lead/`rea-evidence`: abrir ficha de follow-up só para **nomes de pino e nomes de partes da placa C3**, com fonte licenciada: `wokwi-elements` (MIT, verificação **por asset**, `docs/08` §2) ou pinout do ESP32-C3-DevKitM-1 (Espressif, citar versão, não redistribuir PDF) + decisão explícita de naming. Registrar como `UN-009` até lá.
3. `web3d-engineer` + `verification-engineer`: incorporar A6, A7, A8 à spec antes de qualquer código.
4. `clean-room-auditor`: reauditar a spec 0.2 e, depois, o diff do adaptador e os assets 3D (`provenance/`).
5. Lead: fechar A9 do gate (`docs/01` §5/§9, `docs/08` R-01) e adicionar `wokwi/wokwi-docs` à tabela de `docs/08` §2.
6. **Não** adotar `wokwi-cli`/`wokwi-cli lint` (citado em `docs/diagram-format.md` como validador) sem revisão de licença da versão pinada — é ferramenta do fornecedor, não documentação MIT; se algum dia for usada como oráculo, exige `needs-decision`.

> Tratamento dos achados: [`response-to-audit.md`](response-to-audit.md).
> Reauditoria da spec v0.2: [`audit-spec-v0.2.md`](audit-spec-v0.2.md).

