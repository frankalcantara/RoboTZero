# ESP32Sim

Simulador web de microcontroladores ESP32 modernos, com execução de firmware
real, visualização 3D em three.js e hospedagem na Cloudflare.

> **Status:** Fase 0 — descoberta, decisões e protótipos. Ainda não há
> emulador funcional neste diretório.
>
> **Nome:** `ESP32Sim` é um codinome de trabalho. O nome público deve ser
> definido antes do lançamento, com revisão de marca.
>
> **Independência:** projeto independente, não afiliado à Espressif ou à Wokwi.

## Visão

Construir um simulador determinístico, verificável e extensível para a família
ESP32, com foco nas gerações mais recentes (C3, S3, C6, H2/P4), capaz de rodar
firmware Arduino e ESP-IDF no navegador com desempenho próximo ao tempo real.

## Decisão arquitetural central

O simulador é um **aplicativo C++23**. CPU, SoC, periféricos, dispositivos
virtuais, scheduler, conectividade e tracing vivem no core. O core é compilado
para **WebAssembly** com Emscripten e executado em um **Web Worker**; o mesmo
código é compilado nativamente para testes, CI e ferramentas de verificação.

**Three.js e TypeScript são somente interface:** renderização, editor de
circuito, painéis, monitor serial e interação. Nenhuma regra de emulação,
timing ou estado de periférico pode viver no frontend.

## Princípios

1. **Fidelidade com evidência.** Cada comportamento implementado tem uma fonte:
   datasheet, TRM, teste de firmware ou trace de referência.
2. **Determinismo.** Mesma imagem, mesmo input, mesmo trace — independente da
   máquina e do relógio de parede.
3. **Desempenho mensurável.** Orçamentos de ciclos, memória e frame definidos e
   medidos em CI.
4. **Interface separada.** O core não conhece three.js, DOM, JSON ou rede.
5. **Clean-room.** Comportamento pode ser estudado; código, assets e dados
   proprietários não podem ser copiados.
6. **Aberto para extensão.** Novos chips, periféricos, placas e componentes
   entram por interfaces versionadas e testes de conformidade.

## Escopo inicial

- **MVP:** ESP32-C3 (RISC-V RV32IMC), boot de firmware Arduino, GPIO, UART,
  timer, interrupções, flash e um board 3D no navegador.
- **V1:** ESP32-S3 (Xtensa LX7 dual-core), suporte a ESP-IDF, I2C, SPI, ADC,
  LEDC/PWM, RMT, DMA, PSRAM, USB Serial/JTAG e depuração GDB.
- **V2:** ESP32-C6 (Wi-Fi 6, BLE 5.3, 802.15.4), Wi-Fi/BLE simulados e
  ESP32 clássico (LX6 dual-core) para compatibilidade.
- **Fora do MVP:** câmera, LCD, análise analógica SPICE, custom chips,
  colaboração em tempo real e marketplace.

O detalhamento está em [`docs/02-escopo-requisitos.md`](docs/02-escopo-requisitos.md).

## Documentação

| Documento | Conteúdo |
| --- | --- |
| [`docs/00-visao-geral.md`](docs/00-visao-geral.md) | Visão, público, métricas e índice |
| [`docs/01-avaliacao-rea.md`](docs/01-avaliacao-rea.md) | Avaliação do REA como ferramenta de evidência |
| [`docs/02-escopo-requisitos.md`](docs/02-escopo-requisitos.md) | Escopo, requisitos e critérios de aceitação |
| [`docs/03-arquitetura.md`](docs/03-arquitetura.md) | Arquitetura C++23/WASM/three.js/Cloudflare |
| [`docs/04-plano-de-trabalho.md`](docs/04-plano-de-trabalho.md) | Fases, milestones, backlog e primeiras tarefas |
| [`docs/05-agentes.md`](docs/05-agentes.md) | Papéis, handoffs e protocolo dos agentes |
| [`docs/06-conformidade-verificacao.md`](docs/06-conformidade-verificacao.md) | Traces dourados, testes diferenciais e CI |
| [`docs/07-plataforma-cloudflare.md`](docs/07-plataforma-cloudflare.md) | Topologia, limites, custos e deploy |
| [`docs/08-riscos-e-licencas.md`](docs/08-riscos-e-licencas.md) | Riscos, licenças e política clean-room |
| [`evidence/README.md`](evidence/README.md) | Protocolo de registro de evidências |

## Estrutura planejada do repositório

```text
ESP32Sim/
  CMakeLists.txt
  cmake/
  core/                 # C++23: cpu, soc, peripherals, devices, scheduler
  include/esp32sim/     # headers públicos e ABI
  wasm/                 # bindings C, loader, worker
  apps/studio/          # TypeScript + three.js (somente UI)
  services/compiler/    # arduino-cli/ESP-IDF (container)
  tests/                # unitários, conformidade e traces
  tools/                # geradores, benchmarks, scripts
  evidence/             # fichas de evidência de interoperabilidade
  provenance/           # manifesto de licenças de assets/dependências
  docs/
```

A estrutura de produto só deve ser criada quando a Fase 0 aprovar a arquitetura.
O esqueleto atual (`CMakeLists.txt`, `cmake/`, `core/`, `include/`, `wasm/`,
`tests/`) é o spike E0-03/E0-04 e não implica arquitetura aceita.

## Build

Pré-requisitos e comandos estão em [`BUILD.md`](BUILD.md).

```bash
cmake --preset native
cmake --build --preset native
ctest --preset native
```

## Como começar

1. Leia [`docs/00-visao-geral.md`](docs/00-visao-geral.md) e
   [`docs/04-plano-de-trabalho.md`](docs/04-plano-de-trabalho.md).
2. Execute a avaliação do REA e a revisão de licenças antes de observar o Wokwi.
3. Use o agente `esp32-lead` para orquestrar; ele delega para os especialistas.
4. Toda decisão relevante vira ADR; toda alegação de comportamento vira
   evidência ou teste.

## Licença

A licença do projeto ainda não foi decidida. Não distribua binários ou assets
antes da revisão de [`docs/08-riscos-e-licencas.md`](docs/08-riscos-e-licencas.md).
