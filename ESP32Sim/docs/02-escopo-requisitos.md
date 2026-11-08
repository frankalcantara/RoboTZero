# 02 — Escopo e requisitos

## 1. Estratégia de chips

Não é viável entregar todos os ESP32 com a mesma profundidade no primeiro
release. A ordem proposta equilibra dificuldade de ISA, demanda e cobertura de
periféricos.

| Ordem | Chip | CPU | Por que | Fase |
| --- | --- | --- | --- | --- |
| 1 | **ESP32-C3** | RISC-V RV32IMC, single-core, 160 MHz | ISA menor, boa documentação, Wi-Fi/BLE, barato | MVP |
| 2 | **ESP32-S3** | Xtensa LX7 dual-core, 240 MHz | Muito usado, USB, PSRAM, LCD/CAM, IA | V1 |
| 3 | **ESP32-C6** | RISC-V + LP core, Wi-Fi 6, BLE 5.3, 802.15.4 | Geração nova, Thread/Zigbee | V2 |
| 4 | **ESP32 clássico** | Xtensa LX6 dual-core, 240 MHz | Compatibilidade com projetos existentes | V2 |
| 5 | **ESP32-H2** | RISC-V, 802.15.4/BLE | Nicho de conectividade | V3 |
| 6 | **ESP32-P4** | RISC-V dual-core high-end | MIPI, câmera, display, PSRAM | V3+ |

**Decisão pendente:** validar a ordem com dados de uso e com a revisão de
licenças. A decisão final está em `adr/0002-prioridade-de-chips.md`.

### 1.1 Por que C3 primeiro

- RISC-V RV32IMC é menor e mais fácil de validar que Xtensa LX7.
- Permite construir o pipeline C++23→WASM, scheduler, boot, flash e UART sem
  lidar com dual-core e ISA proprietária.
- Serve de base para C6 (mesma família) e para o núcleo RISC-V do P4.
- O S3 entra depois, reutilizando todo o SoC, devices e UI.

### 1.2 Por que não começar pelo ESP32 clássico

- LX6 dual-core e periféricos legados aumentam o risco do MVP.
- O objetivo declarado é “versões mais novas e mais completas”; o clássico
  entra por compatibilidade, não como fundação.

## 2. Requisitos funcionais

### 2.1 Execução de firmware

| ID | Requisito | Prioridade | Critério de aceitação |
| --- | --- | --- | --- |
| F-001 | Carregar imagem de flash (`.bin`) e ELF com símbolos | Must | Firmware boota e atinge `app_main`/`setup()` |
| F-002 | Emular boot ROM/flash boot mínimo do chip alvo | Must | Vetor de reset, imagem e partições corretos |
| F-003 | Executar firmware Arduino | Must | `Blink` e `Serial` no MVP; `Wire`/`SPI` no V1 |
| F-004 | Executar firmware ESP-IDF | Must (V1) | `hello_world`, `blink`, `uart` passam no V1 |
| F-005 | Suportar FreeRTOS e troca de contexto | Must | Tarefas, filas e semáforos funcionam |
| F-006 | Suportar dual-core e SMP (S3/clássico) | Should | Duas tarefas rodam e sincronizam |
| F-007 | Depuração GDB remota | Should | Breakpoint, step, backtrace e watchpoint |
| F-008 | Upload de firmware pré-compilado | Must | Usuário envia `.bin`/ELF e executa |
| F-009 | Compilação de sketch no serviço | Should | `.ino` → `.bin` com logs reproduzíveis |

### 2.2 CPU e memória

| ID | Requisito | Prioridade | Critério de aceitação |
| --- | --- | --- | --- |
| C-001 | Núcleo RISC-V RV32IMC | Must | Testes de ISA e firmware real |
| C-002 | Núcleo Xtensa LX7 | Must (V1) | Testes de ISA e firmware real |
| C-003 | Núcleo Xtensa LX6 | Should (V2) | Testes de ISA e firmware real |
| C-004 | Exceções, interrupções e prioridades | Must | Testes de IRQ e nested interrupts |
| C-005 | MMU/MPU e cache conforme o chip | Must | Mapeamento flash/PSRAM validado |
| C-006 | Memória SRAM/ROM/PSRAM com latência configurável | Must | Teste de memória e desempenho |
| C-007 | Contador de ciclos determinístico | Must | Trace idêntico entre execuções |

### 2.3 Periféricos

| ID | Periférico | MVP | V1 | V2+ |
| --- | --- | --- | --- | --- |
| P-001 | GPIO + IO MUX + GPIO matrix | Sim | Sim | Sim |
| P-002 | UART (TX/RX, FIFO, IRQ, DMA) | Sim | Sim | Sim |
| P-003 | Timers (system, TG0/TG1) | Sim | Sim | Sim |
| P-004 | Interrupções (PLIC/INTERRUPT) | Sim | Sim | Sim |
| P-005 | Flash SPI + cache | Sim | Sim | Sim |
| P-006 | I2C | — | Sim | Sim |
| P-007 | SPI mestre/escravo | — | Sim | Sim |
| P-008 | ADC (oneshot/contínuo) | — | Sim | Sim |
| P-009 | LEDC/PWM | — | Sim | Sim |
| P-010 | RMT (IR/NeoPixel) | — | Sim | Sim |
| P-011 | PCNT | — | Sim | Sim |
| P-012 | GDMA | — | Sim | Sim |
| P-013 | RTC, watchdog, brownout | Parcial (watchdog básico) | Parcial | Sim |
| P-014 | eFuse | — | Parcial | Sim |
| P-015 | USB Serial/JTAG | — | Sim (S3) | Sim |
| P-016 | USB OTG | — | — | Sim |
| P-017 | LCD/CAM (S3/P4) | — | — | Sim |
| P-018 | TWAI/CAN | — | — | Sim |
| P-019 | Temperatura/sensores internos | — | — | Sim |
| P-020 | I2S (áudio: ADC/DAC, PDM) | — | — | Sim |

> **P-013:** o watchdog básico entra no MVP por `04` E1-02; RTC e brownout
> ficam para V1/V2. Este alinhamento resolve o conflito registrado no
> [ADR 0005](adr/0005-escopo-de-paridade-do-mvp.md) §6.1.
>
> **P-020:** I2S não constava desta tabela até 2026-10-25. A ausência no MVP e
> no V1 é decisão registrada, não omissão: áudio exige DMA contínuo e
> temporização de amostra que competem com o orçamento de ciclos do G1. Entra no
> V2 junto com os devices de microfone e amplificador.

### 2.4 Conectividade

| ID | Requisito | Prioridade | Observação |
| --- | --- | --- | --- |
| N-001 | Wi-Fi station simulada | Should (V2) | MAC/PHY + rede virtual determinística |
| N-002 | TCP/UDP/IP via lwIP ou pilha própria | Should (V2) | SLIRP ou rede virtual |
| N-003 | BLE advertising/GATT básico | Could (V2) | HCI virtual |
| N-004 | 802.15.4/Thread/Zigbee (C6/H2) | Could (V3) | Requer análise de escopo |
| N-005 | Captura de pacotes no studio | Could (V2) | PCAP para depuração |
| N-006 | Servidor HTTP local no firmware | Should (V2) | Teste ponta a ponta |

### 2.5 Devices e placas

| ID | Requisito | Prioridade | Critério |
| --- | --- | --- | --- |
| D-001 | Modelo de device C++ com interface estável | Must | Novo device sem alterar o core |
| D-002 | LED, botão, buzzer | Must | Funcionam no MVP; potenciômetro entra no V1 com ADC |
| D-003 | Sensor I2C (ex.: BME280) | Should | Leitura consistente |
| D-004 | Display SPI (ex.: ILI9341) | Should | Framebuffer e refresh |
| D-005 | NeoPixel/RMT | Should | Cores e timing |
| D-006 | Cartão SD virtual | Could | FATFS |
| D-007 | Definição de placa declarativa | Must | Pinout, periféricos e modelo 3D |
| D-008 | Modelos 3D em glTF/GLB | Must | Carregam e mapeiam pinos |

### 2.6 Studio (interface)

| ID | Requisito | Prioridade | Critério |
| --- | --- | --- | --- |
| U-001 | Cena 3D com placa e componentes | Must | 60 fps em cena média |
| U-002 | Editor de circuito e fios | Must | Adicionar, mover e excluir parte; criar e excluir fio; conectar/desconectar pinos |
| U-003 | Editor de código com destaque | Must | TypeScript/Arduino/C++ |
| U-004 | Monitor serial | Must | TX/RX com timestamps; entrada do usuário para o firmware; modos `auto`/`always`/`never` |
| U-005 | Console de build e erros | Must | Logs do compilador |
| U-006 | Controles de execução | Must | Run/pause/step/reset/velocidade |
| U-007 | Analisador lógico | Should | Waveform de pinos |
| U-008 | Depurador | Should | Breakpoints e variáveis |
| U-009 | Salvar/abrir/compartilhar projeto | Must | URL + arquivo local |
| U-010 | Layout responsivo e acessível | Should | Teclado e leitores de tela |
| U-011 | i18n pt-BR/en | Should | Textos externalizados |
| U-012 | Modo offline após carregar | Should | Service Worker/cache |

## 3. Requisitos não funcionais

| ID | Requisito | Meta |
| --- | --- | --- |
| NF-001 | Determinismo | Trace idêntico para mesmo firmware/input/seed |
| NF-002 | Desempenho MVP | ≥ 50% do tempo real em notebook médio |
| NF-003 | Desempenho V1 | ≥ 100% do tempo real em firmware de teste |
| NF-004 | Frame rate | 60 fps cena média; 30 fps mínima |
| NF-005 | Latência de input | < 50 ms para botões |
| NF-006 | Memória WASM | ≤ 256 MB no MVP; crescer com justificativa |
| NF-007 | Tempo de carga | < 3 s em conexão média |
| NF-008 | Tamanho do WASM | ≤ 8 MB gzip no MVP; ≤ 15 MB no V1 |
| NF-009 | Portabilidade | Chrome/Edge/Firefox/Safari recentes |
| NF-010 | Isolamento | COOP/COEP para threads; fallback single-thread |
| NF-011 | Segurança | CSP estrita, sem eval, sem segredos no cliente |
| NF-012 | Acessibilidade | WCAG 2.2 AA nas telas principais |
| NF-013 | Observabilidade | Métricas de ciclos, frame, memória e erros |
| NF-014 | Reprodutibilidade | Builds herméticos e versões pinadas |

## 4. Critérios de aceitação por fase

### Fase 0 — Descoberta e fundação

- [ ] Avaliação do REA concluída e revisão de licenças registrada.
- [ ] Spike C++23 + Emscripten compila e roda em Web Worker.
- [ ] Benchmark de ciclos e memória documentado.
- [ ] Spike three.js carrega um board glTF e recebe estado do core.
- [ ] Spike Cloudflare Pages/Workers com COOP/COEP publicado.
- [ ] ADRs 0001–0004 aceitos.
- [ ] Backlog da Fase 1 priorizado.

### Fase 1 — MVP ESP32-C3

- [ ] Firmware Arduino `Blink` roda e o LED 3D pisca.
- [ ] `Serial.println` aparece no monitor serial.
- [ ] Timer e interrupções funcionam.
- [ ] Boot de imagem real e partições.
- [ ] Trace dourado de `Blink` e `Serial` passa em nativo e WASM.
- [ ] Projeto salva/abre/compartilha.
- [ ] Deploy público na Cloudflare.
- [ ] Documentação de uso e limitações.

### Fase 2 — S3 e periféricos

- [ ] Dual-core e FreeRTOS SMP.
- [ ] I2C, SPI, ADC, LEDC, RMT, DMA.
- [ ] GDB stub com breakpoints e backtrace.
- [ ] Analisador lógico e traces de pinos.
- [ ] Testes diferenciais contra QEMU em pelo menos 10 firmwares.
- [ ] Exemplos oficiais Arduino/IDF no escopo passam.

### Fase 3 — Conectividade e C6

- [ ] Wi-Fi station + TCP/UDP + HTTP local.
- [ ] BLE advertising/GATT básico.
- [ ] 802.15.4/Thread para C6/H2.
- [ ] Rede virtual determinística e captura de pacotes.
- [ ] Testes de estresse de conectividade.

## 5. Fora de escopo (explícito)

- Certificação elétrica ou de radiofrequência.
- Emulação analógica SPICE no MVP.
- Compatibilidade binária com o formato de projeto do Wokwi.
- Execução de firmware proprietário sem licença.
- Emulação de todos os periféricos de todos os chips.
- Multiusuário/colaboração em tempo real antes da Fase 4.
- Marketplace de componentes antes da Fase 4.
- Suporte a placas customizadas por usuário antes da V1.

## 6. Premissas

1. O usuário tem acesso a firmware compilado e/ou toolchain local.
2. Datasheets/TRMs dos chips alvo são públicos.
3. O navegador suporta WebAssembly, Web Workers e, para threads,
   `SharedArrayBuffer` com cross-origin isolation.
4. A Cloudflare é usada para hospedagem estática e APIs; compilação pesada
   roda em serviço separado.
5. O projeto pode usar componentes MIT/Apache; GPL/AGPL exigem revisão.
6. O time aceita que fidelidade total é um processo incremental e medido.

## 7. Dependências externas

| Dependência | Uso | Licença | Status |
| --- | --- | --- | --- |
| Emscripten/LLVM | Build WASM | MIT/UIUC | A definir versão |
| CMake/Ninja | Build | BSD/Apache | OK |
| three.js | Render 3D | MIT | OK |
| Vite/TypeScript | Studio | MIT/Apache | OK |
| Catch2/GoogleTest | Testes C++ | BSL/Apache | A definir |
| arduino-cli | Compilação de sketches | GPLv3? | Revisar |
| ESP-IDF | Compilação de firmware | Apache-2.0 | Revisar componentes |
| QEMU lcgamboa | Oráculo de testes | GPLv2 | Revisar distribuição |
| WABT | Inspeção WASM | Apache-2.0 | OK como ferramenta |
| Cloudflare | Deploy | Termos comerciais | OK |

> Licenças marcadas com “revisar” não podem entrar em distribuição sem parecer
> ou decisão registrada em `08-riscos-e-licencas.md`.
