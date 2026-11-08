# Inventário de funcionalidades documentadas — recorte ESP32-C3/MVP

- **Versão:** 1.0
- **Data:** 2026-10-25
- **Fonte:** `wokwi/wokwi-docs` @ `5c90e532d24fb8ac4cd471e9568db03917011e91` (MIT)
- **Método:** leitura das 7 páginas listadas em [`sources.md`](sources.md); cada
  linha deriva de título/seção do documento. **Nenhum trecho substancial
  reproduzido**: há notas técnicas curtas parafraseadas/traduzidas e literais
  de configuração, sob MIT e com atribuição. Sem código, imagem ou asset.
- **Classificação:** todas as linhas de funcionalidade são **observado
  (documental)** — o documento afirma a existência da funcionalidade. Nenhuma
  foi verificada em execução; o alvo segue `blocked`.
- **Relevância:** coluna de **avaliação do lead** (não é fato do documento):
  `MVP` = blink C3 + GPIO/UART/timer + serial; `Fase 2` = periféricos e
  ferramentas; `Fase 3` = Wi-Fi; `Fora` = não está no roteiro atual;
  `n/a` = não aplicável (linha de agregado, não é funcionalidade).

## Legenda de superfície

| Sigla | Significado |
| --- | --- |
| **Web** | Studio no navegador (`wokwi.com`) |
| **VS Code** | Extensão Wokwi for VS Code |
| **CLI** | Wokwi CLI / CI |
| **Pago** | Exige plano pago (Hobby+/Pro ou equivalente) |
| **Externo** | Ferramenta de terceiro, fora do produto |

## 1. Firmware e placa

| ID | Funcionalidade | O que é | Superfície | Relevância | Fonte |
| --- | --- | --- | --- | --- | --- |
| F-001 | Arduino-ESP32 / ESP-IDF no navegador | Compila e executa sketches Arduino e projetos ESP-IDF (compilados via Arduino core) no studio | Web | MVP | `esp32.md` §Getting Started |
| F-002 | MicroPython / CircuitPython | Executa projetos MicroPython e CircuitPython; REPL pelo terminal serial | Web | Fase 2 | `esp32.md` §Getting Started |
| F-003 | Rust (std) | Executa firmware Rust com `std` (exemplo de conexão usa `esp-idf-svc`) | Web | Fora | `esp32.md` §Getting Started; `esp32-wifi.md` §Connecting from Rust (std) |
| F-004 | Firmware custom | Upload de `.bin`, `.elf` ou `.uf2`, inclusive imagem mesclada (bootloader + partições + app) | Web | Fase 2 | `esp32.md` §Custom Application Firmware |
| F-005 | Placa ESP32-C3-DevKitM-1 | Placa C3 de entrada suportada | Web | MVP | `esp32.md` §ESP32 boards |
| F-006 | Outras placas C3 | XIAO ESP32-C3 e Rust Board ESP32-C3 | Web | Fora | `esp32.md` §ESP32 boards |
| F-007 | Flash configurável | `flashSize` em MB (2/4/8/16/32), default 4 | Web/VS Code/CLI | Fase 2 | `esp32.md` §Flash and memory size |
| F-008 | PSRAM configurável | `psramSize`/`psramType`; **o C3 não tem PSRAM** na matriz | Web/VS Code/CLI | Fora | `esp32.md` §Flash and memory size |
| F-009 | Offset de firmware | `firmwareOffset` manual; default é detecção automática, com fallback em `0x10000` | Web | Fase 2 | `esp32.md` §Custom firmware offset |
| F-010 | Tabela de partições custom | `partitions.csv` no projeto; **só no studio web** (VS Code/CLI usam a do firmware) | Web | Fase 2 | `esp32.md` §Custom Partition Table |
| F-011 | MAC address | `macAddress` configurável; default `24:0a:c4:00:01:10` | Web | Fase 2 | `esp32.md` §Changing the MAC address |
| F-012 | Limite de frequência de CPU | `cpuFrequency` (`"16"`, `"max"` ou `"auto"`); default limita a ~8 MHz para acelerar a simulação | Web | Fase 2 | `esp32.md` §CPU frequency limit |
| F-013 | Chip revision | `chipRevision` `"1.0"`/`"3.1"`; documentado para o P4, não para o C3 | Web | Fora | `esp32.md` §Choosing a different chip revision |
| F-014 | Heap monitoring no CI | CLI acompanha o heap livre e falha o run abaixo de um limite (ESP-IDF) | CLI | Fora | `esp32.md` §Heap monitoring in CI |

## 2. Periféricos do ESP32-C3 (matriz documentada)

A matriz marca cada periférico para o C3. `✔` = simulado; `🟡` = parcial;
`❌` = não implementado; `—` = não existe no chip; `n/a` = não aplicável
(linha de agregado, não é funcionalidade).

| ID | Funcionalidade | Estado no C3 | Observação documentada | Relevância | Fonte |
| --- | --- | --- | --- | --- | --- |
| H-001 | Núcleo do processador | ✔ | — | MVP | `esp32.md` §Simulation Features |
| H-002 | GPIO | ✔ | Interrupções suportadas | MVP | idem |
| H-003 | IOMUX | ✔ | — | MVP | idem |
| H-004 | UART | ✔ | — | MVP | idem |
| H-005 | USB Serial + JTAG | ✔ | Serial suportado; **JTAG não** | MVP | idem |
| H-006 | I2C | ✔ | Apenas master; endereçamento de 10 bits não suportado | Fase 2 | idem |
| H-007 | SPI | ✔ | — | Fase 2 | idem |
| H-008 | RMT | ✔ | — | Fase 2 | idem |
| H-009 | LEDC PWM | ✔ | Base de `analogWrite()`, Servo, Buzzer | Fase 2 | idem |
| H-010 | DMA | ✔ | — | Fase 2 | idem |
| H-011 | Wi-Fi | ✔ | Ver §7 | Fase 3 | idem |
| H-012 | Timers | ✔ | — | MVP | idem |
| H-013 | Watchdog | ✔ | — | MVP | idem |
| H-014 | Stack Guard | ✔ | Trap de stack overflow via Debug Assist (RISC-V) | Fase 2 | idem |
| H-015 | ADC | ✔ | — | Fase 2 | idem |
| H-016 | RNG | ✔ | Gerador de números aleatórios | Fase 2 | idem |
| H-017 | Acelerador AES | ✔ | — | Fase 2 | idem |
| H-018 | Acelerador SHA | ✔ | — | Fase 2 | idem |
| H-019 | Acelerador RSA | ✔ | — | Fase 2 | idem |
| H-020 | TWAI | 🟡 | Implementação parcial | Fase 3 | idem |
| H-021 | RTC | 🟡 | Apenas resistores de pull-up/pull-down | Fase 3 | idem |
| H-022 | GDB debugging | ✔ | Funciona com Wokwi for VS Code | Fase 2 | idem |
| H-023 | Itens ausentes no C3 (agregado) | n/a | PSRAM, USB (CDC genérico), I2S ❌, Bluetooth ❌, MCPWM, PCNT, ECC, Hall, ULP. **Agregado de ausências, não uma funcionalidade** | n/a | idem |

## 3. Editor de diagrama

| ID | Funcionalidade | O que é | Relevância | Fonte |
| --- | --- | --- | --- | --- |
| E-001 | Adicionar parte | Menu `+` ou tecla `A`; nem todas as partes aparecem no menu — MCUs podem exigir editar `diagram.json` | MVP | `diagram-editor.md` §Adding a part |
| E-002 | Mover parte | Arrastar ou setas; `Shift` move em passos maiores | MVP | §Moving a part |
| E-003 | Rotacionar parte | `R` gira 90°; outros ângulos via `diagram.json` | MVP | §Rotating a part |
| E-004 | Duplicar parte | `D`, inclusive várias vezes | MVP | §Duplicating a part |
| E-005 | Excluir parte | `Delete` | MVP | §Deleting a part |
| E-006 | Seleção múltipla | `Shift` + clique; mover/duplicar/excluir em grupo | MVP | §Selecting multiple parts |
| E-007 | Copiar/colar | `Ctrl+C`/`Ctrl+V`; copia também os fios internos; funciona entre projetos | MVP | §Copying and pasting parts |
| E-008 | Criar fio | Clique no pino de origem e no de destino; é possível guiar o caminho; `Esc`/botão direito cancela | MVP | §Creating a wire |
| E-009 | Cor do fio | Automática por função (GND preto, 5 V vermelho, demais verde); paleta por atalho; cor via `diagram.json` | MVP | §Changing the color of a wire |
| E-010 | Excluir fio | Selecionar + lixeira/`Delete`, ou duplo clique | MVP | §Deleting a wire |
| E-011 | Atalhos de teclado | Zoom (`-`/`+`), fit (`F`), doc da parte (`?`), grid (`G`), undo/redo (`Ctrl+Z`/`Y`) | MVP | §Keyboard shortcuts |
| E-012 | Grid e régua | Snap de 2,54 mm (0,1"); `Shift` desativa; `Alt`/`Ctrl` usa grid fino de 1,27 mm; régua em mm/pol | MVP | §Grid snapping |

## 4. Monitor serial

| ID | Funcionalidade | O que é | Relevância | Fonte |
| --- | --- | --- | --- | --- |
| S-001 | Anexo automático à UART (Uno/Mega) | O monitor anexa ao serial de hardware e detecta o baud automaticamente — **documentado para Arduino Uno/Mega**. Para o C3 não há declaração (ver **U-001**) | MVP | `serial-monitor.md` §Arduino Uno and Mega |
| S-002 | Modos de exibição | `display`: `auto`, `always`, `never`, `plotter`, `terminal` | MVP | §Display |
| S-003 | Terminal com cores | Modo `terminal` com XTerm.js e suporte a cores/ANSI | Fase 2 | §Display |
| S-004 | Collapse | `collapse: true` abre o monitor recolhido | MVP | §Collapse |
| S-005 | Newline de entrada | `newline`: `lf`, `cr`, `crlf`, `none` | MVP | §Newline |
| S-006 | Convert EOL | `convertEol` converte `\n` em `\r\n` no modo terminal | Fase 2 | §Convert EOL |
| S-007 | Entrada para o firmware | Texto digitado é enviado ao programa (`Serial.read()` e afins) | MVP | §Newline |
| S-008 | Configuração no diagrama | Bloco `serialMonitor` no `diagram.json` | MVP | §Configuring the Serial Monitor |
| S-009 | USB CDC no C3 | `serialInterface: "USB_SERIAL_JTAG"` e remoção das conexões `$serialMonitor` | MVP | `esp32.md` §USB CDC |
| S-010 | Serial Plotter | Modo `plotter` exibe os dados como gráfico | Fase 2 | §Display |

> **S-001 × U-001:** S-001 registra apenas o fato documentado para Uno/Mega.
> O comportamento do C3 (auto-attach/auto-baud) permanece `unknown` e está
> registrado como **U-001** (§10). O ID **UN-007** pertence à ficha irmã
> [`../wokwi-c3-diagram-contract/spec.md`](../wokwi-c3-diagram-contract/spec.md)
> §7 e cobre a pergunta estreita ("o C3 usa USB CDC por padrão?"); não é o
> mesmo identificador.

## 5. Logic Analyzer

| ID | Funcionalidade | O que é | Superfície | Relevância | Fonte |
| --- | --- | --- | --- | --- | --- |
| L-001 | 8 canais digitais | Canais D0–D7 | Web | Fase 2 | `logic-analyzer.md` §Features |
| L-002 | Taxa de amostragem | 1 GHz (valor declarado na documentação) | Web | Fase 2 | idem |
| L-003 | LEDs de atividade | Um LED verde por canal com atividade | Web | Fase 2 | idem |
| L-004 | Contador de amostras | Mostra quantas amostras foram capturadas | Web | Fase 2 | idem |
| L-005 | Trigger configurável | `triggerMode` (`edge`/`level`), `triggerPin` (D7 por default) e `triggerLevel` | Web | Fase 2 | §Using the Trigger |
| L-006 | Export VCD | No studio web, parar a simulação baixa `wokwi-logic.vcd`; no VS Code, o arquivo é salvo automaticamente (`wokwi.vcd` ou o caminho de `vcdFile`) | Web + VS Code | Fase 2 | §Using the Logic Analyzer; §Exporting VCD in VS Code |
| L-007 | Nomes de canais | `channelNames` renomeia D0…D7 | Web | Fase 2 | §Customizing the Channel Names |
| L-008 | Visualização externa | Abrir o VCD no PulseView ou GTKWave | Externo | Fase 2 | §Viewing the data in PulseView |

## 6. Debugger

| ID | Funcionalidade | O que é | Relevância | Fonte |
| --- | --- | --- | --- | --- |
| D-001 | Debugger web interativo | Beta; disponível **apenas para AVR** (Uno, Nano, Mega, ATtiny85) — **não para o C3** | Fora | `debugger.md` §Interactive debugger |
| D-002 | Breakpoints e stepping | Breakpoint na margem do editor; continuar, step over, step into, step out | Fora | §Using the debugger |
| D-003 | Call stack | Pilha de chamadas até a função atual | Fora | §Call stack |
| D-004 | Variáveis | Valores por escopo: locais, globais e registradores | Fora | §Variables |

> Para o C3, o debug documentado é **GDB via Wokwi for VS Code** (H-022), não o
> debugger embutido do studio.

## 7. Wi-Fi

| ID | Funcionalidade | O que é | Superfície | Relevância | Fonte |
| --- | --- | --- | --- | --- | --- |
| W-001 | Access point virtual | `Wokwi-GUEST`, aberto, canal 6, BSSID `42:13:37:55:aa:01` | Web | Fase 3 | `esp32-wifi.md` §Connecting to the WiFi |
| W-002 | Conexão por linguagem | Exemplos de conexão em Arduino, MicroPython e Rust | Web | Fase 3 | idem |
| W-003 | APs customizados | SSID, senha WPA2-PSK, múltiplas redes, canal e BSSID; substitui o `Wokwi-GUEST` | Web + Pago | Fase 3 | §Custom WiFi Access Points |
| W-004 | Public Gateway | Acesso à internet padrão; monitorado; sem conexões de entrada | Web | Fase 3 | §Internet Access |
| W-005 | Private Gateway | Gateway local; TCP/UDP; conexões de entrada; sem monitoramento | Web + Pago | Fase 3 | §The Private Gateway |
| W-006 | Acesso ao host local | `host.wokwi.internal` e encaminhamento de portas (`--forward`) | Web + Pago | Fase 3 | §Connecting to your local machine |
| W-007 | Endereços de rede | IP público `10.10.0.2`, privado `10.13.37.2`, MAC padrão `24:0a:c4:00:01:10` | Web | Fase 3 | §Network addresses |
| W-008 | Captura PCAP | Download do `wokwi.pcap`; pilha 802.11 → IP/TCP/UDP → DNS/HTTP/MQTT/CoAP; abrir no Wireshark | Web | Fase 3 | §Viewing WiFi traffic with Wireshark |
| W-009 | Limitações | Sem ICMP (ping); Private Gateway não funciona no Safari | Web | Fase 3 | §Limitations |

## 8. Bibliotecas Arduino

| ID | Funcionalidade | O que é | Superfície | Relevância | Fonte |
| --- | --- | --- | --- | --- | --- |
| B-001 | Autocomplete de include | Digitar `#` em linha vazia sugere `#include` de bibliotecas populares | Web | MVP | `libraries.md` intro |
| B-002 | Bibliotecas embutidas | Compilação inclui as bibliotecas padrão do Arduino (ex.: `Wire.h`, `SPI.h`) | Web | MVP | idem |
| B-003 | Library Manager | Busca e instala bibliotecas do Arduino Library Manager | Web | Fase 2 | §Adding third party libraries |
| B-004 | Upload de biblioteca custom | Envia uma pasta `.h`/`.c`/`.cpp` compactada ao build server | Web + Pago | Fora | §Uploading custom libraries |
| B-005 | `libraries.txt` | Uma biblioteca por linha; `#` comenta; `nome@versão`; custom com `@wokwi:<id>` | Web | Fase 2 | §The libraries.txt file |

## 9. O que este recorte não cobre

Por decisão de escopo (subconjunto ESP32-C3/MVP), **não** foram inventariados:

- as 70 páginas de `docs/parts/` (catálogo de placas e componentes);
- `docs/vscode/` (10 páginas), `docs/wokwi-ci/` (8), `docs/chips-api/` (11);
- guias de MicroPython, CircuitPython, custom chips, GDB, diagram-format,
  getting-started e FAQ;
- funcionalidades do produto que não estão documentadas nessas 7 páginas.

## 10. O que permanece desconhecido

| ID | Item | Status |
| --- | --- | --- |
| U-001 | O comportamento real do C3 no studio (auto-attach serial, timing, periféricos) | desconhecido — alvo `blocked` |
| U-002 | Se a matriz de periféricos está atualizada na data da leitura | desconhecido |
| U-003 | Funcionalidades do produto não documentadas | desconhecido |
| U-004 | Detalhes de implementação (como cada funcionalidade é feita) | fora do escopo e não investigado |
| U-005 | Limites de plano/preço além do que a doc menciona | desconhecido |
