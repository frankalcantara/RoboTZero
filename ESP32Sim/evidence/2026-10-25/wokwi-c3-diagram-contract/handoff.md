# Handoff — próxima etapa

Formato conforme `ESP32Sim/docs/05-agentes.md` §4.

## 1. Objetivo

Decidir se o ESP32Sim adota `diagram.json` v1 como **formato de
importação/exportação** do studio e, em caso positivo, especificar o adaptador
e suas fixtures de conformidade — sem observar o alvo e sem implementar
emulação.

## 2. Contexto

- Ficha: [`README.md`](README.md)
- Gate: [`gate.md`](gate.md) — alvo `blocked`
- Spec: [`spec.md`](spec.md) (v0.3, correções das revisões clean-room)
- Análise: [`notes.md`](notes.md)
- Fontes: [`sources.md`](sources.md)
- Pareceres: [`audit-clean-room.md`](audit-clean-room.md),
  [`audit-spec.md`](audit-spec.md) (v0.1),
  [`audit-spec-v0.2.md`](audit-spec-v0.2.md) (reauditoria) e
  [`response-to-audit.md`](response-to-audit.md) (tratamento)
- Arquitetura: `ESP32Sim/docs/03-arquitetura.md` §5, §7, §10
- Evidência: `ESP32Sim/evidence/README.md`

## 3. Escopo

**Entra:**

- decisão de produto sobre interoperabilidade de formato;
- desenho do adaptador (import/export) e da tabela de mapeamento;
- fixtures autorais e testes de round-trip;
- registro de proveniência dos assets 3D (placa/LED) quando existirem;
- resolução de UN-009 (nomes de pino da placa C3) com fonte licenciada.

**Não entra:**

- observar o simulador do alvo (permanece bloqueado);
- emular CPU, GPIO, UART, timers, boot ou flash;
- compilar firmware;
- copiar assets, código ou diagramas da comunidade;
- alterar a ABI do core por causa do formato.

## 4. Critério de aceite

- [ ] ADR decidindo adotar (ou não) `diagram.json` v1 como formato de
      importação/exportação.
- [ ] Tabela de mapeamento `board-esp32-c3-devkitm-1` → ID interno e
      `wokwi-led` → device interno, versionada.
- [ ] Adaptador com os códigos de erro de `spec.md` §5.3.
- [ ] Fixtures de `spec.md` §6 passando; `blink-c3-minimal` e
      `blink-c3-serial-explicit` só depois de resolver UN-009.
- [ ] Round-trip preservando campos de R-03/R-04 e o bloco `serialMonitor`
      (R-14).
- [ ] Limites de R-15, segurança de R-16/R-17 e validação de R-18/R-19
      implementados e testados.
- [ ] Nenhum asset do alvo no diff; `provenance/` atualizado.
- [ ] Entrada MIT de `wokwi/wokwi-docs` em `THIRD_PARTY_NOTICES` antes de
      distribuição pública do adaptador/spec.
- [ ] Revisão clean-room da implementação e dos assets.

## 5. Evidência esperada

- Relatório de testes com fixtures e hashes.
- Diff do adaptador e da tabela de mapeamento.
- `provenance/` para modelos 3D.
- Confirmação de que nenhuma captura do alvo foi usada.
- Resultado da verificação de `MANIFEST.sha256` desta evidência: **todos os
  arquivos listados verificaram OK** na execução de fechamento (procedimento
  abaixo). O comando e a saída são reproduzíveis; nenhum hash foi inventado.

### 5.1 Verificação dos hashes desta evidência

Executar em `ESP32Sim/evidence/2026-10-25/wokwi-c3-diagram-contract`:

```powershell
Get-Content MANIFEST.sha256 | ForEach-Object {
  $h, $p = $_ -split '  ', 2
  $actual = (Get-FileHash -Algorithm SHA256 -Path $p).Hash.ToLower()
  if ($actual -eq $h) { "OK   $p" } else { "FAIL $p" }
}
```

## 6. Restrições

- ToS do alvo: **`blocked`**; não abrir, capturar ou automatizar o serviço.
- Fonte permitida nesta etapa: documentação MIT `wokwi/wokwi-docs`, com commit
  pinado e atribuição.
- Nenhum código/asset proprietário.
- Nenhum commit/push sem pedido explícito.
- Core C++23/WASM; three.js/TS apenas apresentação.
- JSON só na fronteira; core recebe representação binária.
- `wokwi-cli` (citado na doc como validador) **não** é documentação MIT; se for
  usado como oráculo, exige revisão de licença e `needs-decision`.

## 7. Perguntas abertas

1. O produto deve aceitar `diagram.json` como formato de importação, de
   exportação ou ambos?
2. Como tratar partes desconhecidas: erro, quarentena ou preservação?
3. Qual é o nome/estrutura dos arquivos de código do projeto?
4. Qual é o contrato de compilação (bin/elf/uf2, offsets, partições)?
5. Qual é o comportamento de boot/serial de um blink C3 em hardware real?
6. Como versionar o adaptador quando o formato público mudar?
7. Quais assets 3D de placa/LED têm licença compatível?
8. Quais são os nomes de pino da placa C3 e qual fonte licenciada os define
   (pinout Espressif, `wokwi-elements` MIT com verificação por asset)?

## 8. Agente recomendado

| Ordem | Agente | Tarefa |
| --- | --- | --- |
| 1 | `esp32-lead` | ADR de adoção do formato, priorização e fechamento de A9 do gate (`docs/01` §5/§9, `docs/08` R-01) |
| 2 | `rea-evidence` | ficha de follow-up para UN-009: nomes de pino da placa C3 a partir de fonte licenciada, sem tocar no alvo |
| 3 | `web3d-engineer` | adaptador import/export no studio (após spec 0.3 revisada) |
| 4 | `verification-engineer` | fixtures, limites e round-trip |
| 5 | `firmware-runtime-engineer` | próxima investigação: arquivos de código, compilação e boot (fontes públicas) |
| 6 | `soc-peripherals-engineer` | GPIO/UART a partir de TRM |
| 7 | `clean-room-auditor` | revisão da spec 0.3, do diff do adaptador e dos assets |

Enquanto o gate do alvo estiver `blocked`, qualquer trabalho de
interoperabilidade deve continuar por documentação MIT, TRM/datasheet,
Arduino-ESP32/ESP-IDF open source, hardware próprio e traces próprios.

## 9. Dívidas registradas

- **A9 do gate:** `docs/01-avaliacao-rea.md` §5/§9 ainda diz "adotar
  condicionalmente" e `docs/08-riscos-e-licencas.md` R-01 ainda não foi
  atualizado com o `blocked`. Não foi alterado nesta etapa para respeitar o
  limite de escopo (somente evidência).
- **MIT notice:** `THIRD_PARTY_NOTICES` e `provenance/` ainda não existem no
  repositório; criar antes de distribuir o adaptador ou publicar a spec.
  `wokwi/wokwi-docs` também precisa ser incluído na tabela de `docs/08` §2.
- **UN-009:** sem nomes de pino licenciados, `blink-c3-minimal` e
  `blink-c3-serial-explicit` ficam bloqueadas. Não inventar pinos.
