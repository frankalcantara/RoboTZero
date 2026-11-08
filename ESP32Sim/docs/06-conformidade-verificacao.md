# 06 — Conformidade e verificação

## 1. Princípio

**Sem trace, não há compatibilidade.** Um periférico “que funciona no exemplo”
não está pronto. Ele precisa de:

1. fonte de verdade (TRM/datasheet/teste de referência);
2. teste unitário do modelo;
3. teste de integração com firmware real;
4. trace dourado reproduzível;
5. benchmark de custo.

## 2. Pirâmide de testes

```text
                 ┌──────────────────────────┐
                 │ Sistema / UX / deploy    │  poucos, caros, E2E
                 ├──────────────────────────┤
                 │ Firmware real (Arduino/  │  exemplos oficiais
                 │ ESP-IDF/FreeRTOS)        │
                 ├──────────────────────────┤
                 │ Integração SoC + devices │  barramentos, IRQ, DMA
                 ├──────────────────────────┤
                 │ Periféricos unitários    │  registradores, timing
                 ├──────────────────────────┤
                 │ CPU/ISA + scheduler      │  base, muitos e rápidos
                 └──────────────────────────┘
```

### 2.1 Testes de CPU/ISA

- Testes próprios por instrução (semântica, flags, exceções).
- `riscv-tests` para RV32IMC.
- Suítes Xtensa próprias + comparação com QEMU.
- Testes de interrupções aninhadas e prioridades.
- Testes de contadores (`mcycle`, `minstret`, CCOUNT).
- Fuzzing de sequências de instruções com comparação de estado final.

### 2.2 Testes de periféricos

Para cada periférico:

- reset e valores default;
- semântica de cada registrador (RO/RW/W1C/RC);
- IRQ por evento;
- FIFO, overflow, underflow;
- DMA e concorrência;
- clock enable/reset;
- comportamento com clock parado.

Ferramenta: testes C++ com acesso direto ao barramento + firmware mínimo.

### 2.3 Testes de integração

- UART loopback (TX → RX).
- I2C com sensor virtual.
- SPI com display virtual.
- ADC com fonte de tensão programável.
- LEDC com medidor de duty.
- RMT com decodificador de pulse train.
- DMA com memória e periférico.

### 2.4 Testes de firmware real

Matriz mínima:

| Firmware | Chip | Verifica |
| --- | --- | --- |
| `Blink` | C3/S3 | GPIO, timer, boot |
| `Serial` | C3/S3 | UART, clock, IRQ |
| `FreeRTOS` tasks | C3/S3 | scheduler, ticks |
| `Wire` scanner | C3/S3 | I2C |
| `SPI` display | S3 | SPI, DMA |
| `ADC` read | C3/S3 | ADC |
| `LEDC` fade | C3/S3 | PWM |
| `RMT` NeoPixel | S3 | RMT |
| `WiFi` HTTP | C6 | conectividade |
| `BLE` advertise | C6 | BLE |

Cada firmware tem:

- fonte e licença;
- comando de build;
- binário/ELF com hash;
- saída serial esperada;
- trace dourado;
- tolerâncias (se houver).

### 2.5 Testes do studio

O studio é apresentação: não tem comportamento de emulação para provar. Tem,
porém, contratos que impedem a UI de virar um segundo emulador
([ADR 0007](adr/0007-padroes-de-apresentacao-do-studio.md)). Cada padrão AP tem
verificação própria, e **nenhuma delas compara pixel**.

| AP | O que prova | Teste | Nível | Bloqueia merge |
| --- | --- | --- | --- | --- |
| AP-01 | IDs únicos e sem órfão; bundle inicial não importa three.js nem o analisador | `studio.catalog.ids`, `studio.bundle.initial` | unitário + build | Sim |
| AP-02 | N eventos de pino em um frame produzem uma única atualização de UI | `worker_smoke` (bridge), `studio.frame.coalesce`, `studio.frame.no-store-write` | unitário (bridge no E0-05; studio no E1-06) | Sim |
| AP-03 | Módulos puros sem DOM e sem three.js, com teste | `studio.pure.imports`, `studio.pure.coverage` | lint + unitário | Sim |
| AP-04 | Falha de painel ou de rota não derruba o restante da tela | `studio.error.panel`, `studio.error.route` | unitário + E2E | Sim |
| AP-05 | Nenhuma string visível inline; catálogo sem chave faltando ou órfã | `studio.i18n.inline`, `studio.i18n.catalog` | lint + unitário | Sim |
| AP-06 | Controles operáveis por teclado; sem violação crítica de acessibilidade | `studio.a11y.keyboard`, `studio.a11y.axe` | unitário + E2E | Sim |
| AP-07 | Nenhum literal de cor fora do arquivo de tokens; tema re-lê a paleta | `studio.theme.tokens`, `studio.theme.reload` | lint + unitário | Sim |
| AP-08 | O mesmo trace dourado produz o mesmo diagrama de timing | `studio.trace.golden` | unitário golden | Sim |

Regras de execução:

- **AP-01 e AP-02 entram no CI no primeiro código de studio** (E0-05/E0-06);
  os oito são exigidos no G1.
- `worker_smoke` (`ctest --preset wasm`) já cobre o lado da bridge: N eventos
  emitidos pelo Worker saem em uma única drenagem, sem chamada JS por ciclo. O
  teste de coalescência na camada reativa nasce com o studio (E1-06).
- Testes de AP rodam no pipeline de §8.1, entre goldens e benchmark.
- E2E de AP-04 e AP-06 roda no preview deploy, não no build local.
- O golden de AP-08 é o **modelo** do diagrama (canais, slots, níveis), não o
  bitmap renderizado.

Limites: estes testes não provam qualidade visual, fps real nem usabilidade.
Isso fica em §7.3 (benchmark de studio) e na revisão manual do gate.

## 3. Trace dourado

### 3.1 Formato

JSONL ou binário compacto, com um evento por linha:

```json
{"seq":1,"cycle":0,"type":"reset","pc":"0x40000000"}
{"seq":2,"cycle":12,"type":"reg","pc":"0x40000004","reg":"a0","value":"0x1"}
{"seq":3,"cycle":20,"type":"mem","pc":"0x40000008","addr":"0x3fc80000","op":"w","size":4,"value":"0x1"}
{"seq":4,"cycle":30,"type":"pin","pin":8,"value":1}
{"seq":5,"cycle":40,"type":"serial","direction":"tx","bytes":"48 65 6c 6c 6f"}
{"seq":6,"cycle":50,"type":"irq","source":"uart0","level":1}
```

Regras:

- `cycle` é absoluto e monotônico.
- `seq` é global e determinístico.
- Campos numéricos usam hex estável.
- Eventos agregados podem ser emitidos a cada N ciclos para reduzir volume.
- O trace não pode alterar o timing quando desabilitado.
- O hash é SHA-256 da representação canônica escolhida: JSONL ordenado e sem
  espaços extras ou binário com layout versionado.

### 3.2 O que entra no trace

- reset, boot, panic, watchdog;
- instruções selecionadas (PC, opcode, registradores alterados);
- acessos a memória mapeada;
- acessos a registradores de periféricos;
- IRQs (raise/clear);
- mudanças de pino;
- serial TX/RX;
- DMA transfers;
- eventos de devices;
- breakpoints.

### 3.3 Comparação

- Nativo vs WASM: mesmo hash.
- Build `debug` vs `release`: mesmo hash de comportamento (exceto logs).
- Versão anterior vs atual: diff explicado.
- Emulador vs QEMU: diff classificado (`match`, `known-divergence`, `bug`).
- Emulador vs hardware: mesma classificação, com tolerância documentada.

## 4. Testes diferenciais

### 4.1 QEMU lcgamboa

- Usar como oráculo de CPU/periféricos, não como especificação cega.
- Rodar o mesmo firmware e comparar:
  - estado final de registradores;
  - memória;
  - saída serial;
  - contagem de ciclos (quando comparável);
  - IRQs.
- Divergências conhecidas ficam em `tests/conformance/known-divergences.md`.

### 4.2 Hardware real

- Placas C3/S3/C6 com firmware de teste.
- Captura de UART, lógica e consumo.
- Comparar timestamps com tolerância de clock.
- Hardware é a verdade final; QEMU é mais rápido para regressão.

### 4.3 Wokwi

- Usar apenas como **comportamento observável** e dentro dos termos aplicáveis.
- Nunca copiar implementação.
- Comparar UX e saídas de firmware, não código.
- Registrar limitações de versão/data.

## 5. Fuzzing

Alvos:

- decoder de ISA (bytes aleatórios e válidos);
- mapa de memória (endereços limítrofes);
- registradores de periféricos (sequências de read/write);
- imagens de flash malformadas;
- arquivos de projeto (schema, limites, paths);
- protocolo de eventos worker↔UI.

Ferramentas:

- libFuzzer (nativo) para decoder/bus;
- AFL++ opcional;
- fuzzing de schema no TypeScript (fast-check);
- corpus versionado e minimizado.

Critérios:

- sem crash, sem UB, sem leitura fora de limites;
- divergência de comportamento vira bug ou known-divergence;
- execução determinística.

## 6. Sanitizers e análise estática

- ASan/UBSan no build nativo `debug`.
- TSan apenas em código com threads; não no core single-thread.
- `clang-tidy` com checks acordados.
- `-Wall -Wextra -Werror` no CI.
- Sem `reinterpret_cast` inseguro sem justificativa.
- Sem `memcpy` fora de limites; usar `std::span` e `std::copy_n`.

## 7. Benchmarks

### 7.1 Core

- ciclos simulados por segundo;
- instruções por segundo;
- custo por acesso a memória/registrador;
- custo de IRQ;
- custo de DMA;
- memória pico.

### 7.2 WASM

- tamanho do `.wasm` bruto/gzip;
- tempo de instanciação;
- tempo de carga no worker;
- memória inicial e crescimento;
- custo de `SharedArrayBuffer`/ring buffer.

### 7.3 Studio

- fps, frame time, draw calls, triângulos;
- tempo de picking;
- tempo de atualização de fios;
- memória do renderer.

### 7.4 Metodologia

- Hardware de referência documentado.
- Browser/versão documentados.
- 10 execuções, mediana e p95.
- Comparar antes/depois no mesmo commit base.
- Publicar resultados no PR.

## 8. CI

### 8.1 Pipeline

```text
lint → build nativo → testes unitários → build WASM → testes WASM
     → goldens → studio (catálogo, i18n, a11y, tokens) → benchmark
     → deploy preview
```

### 8.2 Matriz

- Windows (MSVC/Clang), Linux (GCC/Clang), macOS (Apple Clang).
- Emscripten pinado.
- Node pinado.
- Browser: Chromium headless para testes de bridge; Playwright para E2E.

### 8.3 Artefatos

- `esp32sim.wasm` + loader;
- binários de teste;
- traces dourados;
- relatório de benchmark;
- SBOM e licenças;
- screenshots/vídeo do studio.

### 8.4 Regras de merge

- Nenhum merge com golden quebrado.
- Nenhum merge com licença desconhecida.
- Nenhum merge com regressão de performance > 10% sem justificativa.
- Nenhum merge com `TODO` de comportamento não documentado.

## 9. Gates de qualidade

| Gate | Critério |
| --- | --- |
| G0 | Spikes, ADRs, licenças e benchmark inicial |
| G1 | Blink/Serial + goldens nativo/WASM + deploy + AP-01..AP-08 |
| G2 | S3 + periféricos + GDB + diferencial QEMU |
| G3 | Wi-Fi/BLE + C6 + estresse |
| Release | SBOM, licenças, limitações, rollback |

## 10. Relatório de conformidade

Cada release publica:

```markdown
# Conformidade vX.Y.Z

## Escopo
Chips, periféricos e firmwares cobertos.

## Resultados
- Goldens: N/N
- Diferencial QEMU: N match, M known-divergence, K bugs
- Hardware: N/N
- Performance: X ciclos/s, Y ms/frame

## Limitações
O que não foi testado e por quê.

## Evidências
Links para traces, hashes e relatórios.
```

## 11. Orçamento de testes

- Testes unitários: < 2 min no CI.
- Goldens: < 10 min.
- Diferencial QEMU: noturno.
- Hardware: semanal/manual.
- Fuzzing: noturno com corpus acumulado.

## 12. Anti-padrões

- Golden gerado pelo próprio emulador sem revisão.
- “Passa no Blink” como prova de SoC.
- Tolerância ampla para esconder divergência.
- Trace que muda o timing.
- Teste que depende de relógio de parede.
- Screenshot como prova de contrato de UI: é flaky e não prova o contrato.
- Evento de pino virando atualização de estado na UI (quebra AP-02).
- Comparar com Wokwi e tratar como especificação.
