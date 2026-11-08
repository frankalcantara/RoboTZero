# Tratamento dos achados de auditoria

- **Autor:** `esp32-lead`
- **Data:** 2026-10-25
- **Objeto:** evidência `2026-10-25/wokwi-c3-diagram-contract`
- **Pareceres tratados:**
  [`audit-spec.md`](audit-spec.md) (spec v0.1) e
  [`audit-spec-v0.2.md`](audit-spec-v0.2.md) (reauditoria da v0.2)

> Este arquivo é do lead. Ele não substitui os pareceres do auditor e não
> altera as conclusões deles. Cada linha registra o que foi efetivamente
> alterado no repositório; o que não foi feito permanece como pendência
> explícita.

## 1. Primeira rodada — spec v0.1 → v0.2

| Achado | Tratamento | Estado |
| --- | --- | --- |
| A1 | `MANIFEST.sha256` e `audit-spec.md` criados; referência de §6.2 corrigida; handoff §5 atualizado | corrigido |
| A2 | `sources.md` §2 renomeado para `SRC-001..SRC-005`; nota de separação SRC/EV adicionada | corrigido |
| A3 | `GPIO`/`GND`/`GPIO8` removidos de DV-002 e da spec; `UN-009` criado; `blink-c3-minimal` marcada como bloqueada | corrigido |
| A4 | C-08 qualificado (Uno/Mega; C3 em UN-007) | corrigido na spec; propagação para README/notes concluída na v0.3 |
| A5 | C-01 reescrito ("presentes na estrutura básica; obrigatoriedade não declarada") | corrigido na spec; resíduos em DV-002/notes §2.1 concluídos na v0.3 |
| A6 | Namespace `$` reservado; `$` proibido em `parts[].id`; pseudo-partes resolvidas; erro `unknown_pseudo_part` | corrigido |
| A7 | R-14 criado; fixture `serial-monitor-roundtrip` adicionada; R-10 estendido a C-09/C-10 | corrigido |
| A8 | R-15..R-19 criados (limites, XSS, prototype pollution, tipo de versão, campos ausentes); códigos de erro e fixtures adicionados | corrigido |
| A9 | Obrigação MIT registrada no handoff §4/§9 e em `sources.md` §4; `THIRD_PARTY_NOTICES` não criado (fora do escopo de evidência) | registrado, pendente |
| A10 | Registrado em `gate.md` §8 e `handoff.md` §9; docs de arquitetura não alterados | registrado, pendente |
| A11 | EV-013 marcado como contextual/não normativo na ficha e na spec | corrigido na ficha/spec; `notes.md` §3 concluído na v0.3 |
| A14 | Nota de ferramentas corrigida; timestamp tratado como declaração | corrigido |

## 2. Reauditoria — spec v0.2 → v0.3

| Achado | Tratamento | Estado |
| --- | --- | --- |
| A1a — referência a "§Reauditoria" inexistente | Reauditoria registrada em [`audit-spec-v0.2.md`](audit-spec-v0.2.md); referência substituída por links para esse arquivo e para este | corrigido |
| A1b — seção do lead dentro do parecer | Seção removida de `audit-spec.md`; tratamento movido para este arquivo | corrigido |
| A1c — `MANIFEST` não se auto-cobre | Aceito como limitação: o manifest cobre os entregues, não a si próprio; hash do manifest deve ser registrado no commit/ADR de fechamento | limitação registrada |
| A1d — edição posterior quebra a verificação | Procedimento documentado: regenerar o manifest no mesmo commit de qualquer edição | procedimento registrado |
| A2 — cosmético: README cita caminho em vez de `SRC-*` | Mantido: o caminho do arquivo é explícito e o mapeamento `SRC-*` está em `sources.md` §2 | aceito |
| A3 — `blink-c3-serial-explicit` sem bloqueio por UN-009 | Fixture marcada como **bloqueada** em `spec.md` §6; `handoff.md` §4/§9 atualizados | corrigido |
| A3 — DV-002 com "pino GPIO"/"pino de referência" | Reescrito para "pino de I/O" e "pino de referência", mantendo UN-009 explícito | corrigido |
| A4 — qualificação não propagada | H5, EV-008, DV-003 e `notes.md` §2.4 qualificados para Uno/Mega; C-08 passou a rastrear EV-008 + EV-009 | corrigido |
| A5 — resíduos "opcional" | DV-002 e `notes.md` §2.1 passaram a dizer "obrigatoriedade não declarada (UN-002)" | corrigido |
| A6a — sem código de erro para `$` em `parts[].id` | Adicionado `reserved_id` em `spec.md` §5.3 e no passo 5 da importação | corrigido |
| A6b — export de conexões com pseudo-parte não explícito | R-10 passou a exigir reemissão das conexões `$serialMonitor:*` | corrigido |
| A6c — sem fixture para `unknown_pseudo_part` | Fixture `unknown-pseudo-part` adicionada em `spec.md` §6 | corrigido |
| A6 — C-11 classificado como observado | Reclassificado como `derivado` | corrigido |
| A7 | Sem ação necessária | clean |
| A8 — R-17 ambíguo ("rejeitar ou parser seguro") | R-17 passou a exigir rejeição (`duplicate_key`), sem alternativa | corrigido |
| A8 — fixtures faltantes | Adicionadas: `invalid-version-type`, `duplicate-key`, `unknown-pseudo-part`, `invalid-serial-monitor`, `unknown-pin`, `xss-label` | corrigido |
| A9 — `docs/08` §2 sem `wokwi/wokwi-docs` | Registrado em `handoff.md` §9 como pendência específica; `docs/08` não alterado nesta etapa | registrado, pendente |
| A10 — dívida do gate | Mantida em `handoff.md` §9 e `gate.md` §8 | registrado, pendente |
| A11 — `notes.md` §3 contradiz EV-013 | Linha de EV-013 separada como "observado (contextual)" e "não pode virar requisito" | corrigido |
| Contradição 4 — C-07 ambíguo | Redação corrigida: só as instruções **depois** de `*` são revertidas | corrigido |
| Contradição 5 — C-03 com tradução entre aspas | Marcado explicitamente como tradução do original em inglês | corrigido |

## 3. O que permanece aberto

| Item | Motivo | Onde acompanhar |
| --- | --- | --- |
| Autorização escrita da Wokwi (A1–A8 do gate) | não existe; alvo segue `blocked` | `gate.md` §6 |
| A9 do gate (`docs/01` §5/§9, `docs/08` R-01) | fora do limite de escopo desta etapa (somente evidência) | `handoff.md` §9 |
| `THIRD_PARTY_NOTICES` e `provenance/` | fora do escopo de evidência; obrigatório antes de distribuir | `handoff.md` §4/§9 |
| `docs/08` §2 sem `wokwi/wokwi-docs` | idem | `handoff.md` §9 |
| UN-009 (nomes de pino da placa C3) | sem fonte licenciada; bloqueia `blink-c3-minimal` e `blink-c3-serial-explicit` | `spec.md` §6/§7, `handoff.md` §7 |
| Fixtures de `invalid_version_type`, `duplicate_key`, `unknown_pseudo_part`, `invalid_serial_monitor`, `unknown_pin`, `xss-label` | especificadas, mas a criação pertence à implementação | `spec.md` §6 |

Nenhuma implementação foi iniciada nesta etapa.
