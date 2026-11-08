# Notas de análise — inventário de funcionalidades

Estas notas acompanham [`inventory.md`](inventory.md). Elas separam o que a
documentação **declara** do que o lead **avalia** para o MVP. Não substituem a
ficha [`README.md`](README.md).

## 1. O que a lista é — e o que não é

- É um **inventário documental**: cada linha existe porque uma página MIT do
  commit pinado a descreve.
- Não é uma promessa de que a funcionalidade funciona hoje, nem de que funciona
  no C3. O alvo segue `blocked`; nada foi executado.
- Não é um requisito do ESP32Sim. A coluna de relevância é avaliação do lead.
- Não é um clone: nomes de atributos e de funcionalidades são usados para
  interoperabilidade e comparação, não copiados como implementação.

## 2. Números

| Corte | Quantidade |
| --- | ---: |
| Linhas inventariadas | 85 (84 funcionalidades + 1 agregado) |
| Firmware/placa (F) | 14 |
| Periféricos do C3 (H) | 23 |
| Editor de diagrama (E) | 12 |
| Monitor serial (S) | 10 |
| Logic analyzer (L) | 8 |
| Debugger (D) | 4 |
| Wi-Fi (W) | 9 |
| Bibliotecas (B) | 5 |

Relevância (avaliação do lead): **30 MVP**, **32 Fase 2**, **12 Fase 3**,
**10 Fora** e **1 `n/a`** (H-023, agregado de ausências).

## 3. Leituras que mudam decisão

### 3.1 Debugger no navegador não cobre o C3

`debugger.md` diz que o debugger embutido é beta e disponível para AVR (Uno,
Nano, Mega, ATtiny85). Para ESP32/C3, a própria página remete ao Wokwi for VS
Code. A matriz de `esp32.md` confirma GDB para o C3, mas "works with Wokwi for
VS Code". **Consequência:** não prometer debugger de C3 no studio como paridade
do MVP; se entrar, é uma fase própria e via GDB.

### 3.2 Serial do C3 não tem auto-attach declarado

O texto de auto-attach/auto-baud está na seção "Arduino Uno and Mega". Para o
C3, a doc descreve a opção USB CDC (`serialInterface: "USB_SERIAL_JTAG"`) e
manda remover as conexões `$serialMonitor`. **Consequência:** o ESP32Sim precisa
decidir explicitamente o default do C3; não herdar o comportamento de Uno/Mega
como se fosse fato.

No inventário, `S-001` registra **apenas** o fato documentado para Uno/Mega; o
comportamento do C3 fica em **U-001** (`unknown`). O ID **UN-007** pertence à
ficha irmã [`../wokwi-c3-diagram-contract/spec.md`](../wokwi-c3-diagram-contract/spec.md)
§7 e cobre a pergunta estreita ("o C3 usa USB CDC por padrão?"). São
identificadores distintos, com escopos distintos.

### 3.3 Superfície importa

Boa parte do que parece "do produto" é de outra superfície:

- VS Code: GDB, `wokwi.toml`, VCD automático, PlatformIO/ESP-IDF;
- CLI: heap monitoring, CI;
- plano pago: APs customizados, Private Gateway, upload de biblioteca.

**Consequência:** uma lista única "Wokwi tem X" mistura promessas de produto,
extensão e plano. O inventário separa a coluna `Superfície` justamente para
evitar essa confusão.

### 3.4 Lacunas do C3 são declaradas

A matriz marca como ausentes/não implementados no C3: PSRAM, USB CDC genérico,
I2S, Bluetooth, MCPWM, PCNT, ECC, Hall, ULP; TWAI e RTC são parciais.
**Consequência:** paridade nesses itens não é critério de MVP; alguns nem
existem no chip.

### 3.5 `partitions.csv` é específico do studio web

A doc diz que a tabela custom só é usada no Wokwi.com; VS Code/CLI usam a tabela
do firmware. **Consequência:** o ESP32Sim precisa decidir se a tabela é
artefato de projeto ou do binário — decisão de arquitetura, não de UI.

## 4. O que a lista sugere para o MVP

Ordem defensável, considerando o objetivo "blink C3 ponta a ponta":

1. **F-001/F-005** — rodar Arduino-ESP32 na placa C3.
2. **H-001..H-005, H-012, H-013** — núcleo, GPIO, IOMUX, UART, USB serial,
   timers e watchdog.
3. **E-001..E-012** — editor suficiente para montar o diagrama do blink.
4. **S-001..S-009** — serial com decisão explícita UART x USB CDC.
5. **B-001/B-002** — includes e bibliotecas padrão.

Fica para depois: I2C/SPI/ADC/PWM/RMT/DMA (Fase 2), logic analyzer (Fase 2),
GDB (Fase 2), Wi-Fi (Fase 3), Rust/upload de lib/CI (Fora).

Isso é coerência com o plano de fases do projeto, não uma prescrição do
fornecedor.

## 5. O que não pode ser inferido

- Que o C3 real se comporta como a doc diz — não houve execução.
- Que a matriz de periféricos está atualizada.
- Que a lista está completa: 7 páginas não cobrem o produto.
- Que "documentado" implica licença para copiar implementação; a licença MIT
  cobre a documentação, não o software do simulador.
- Que funcionalidades de plano pago podem ser replicadas sem decisão de produto.

## 6. Riscos residuais

| Risco | Mitigação |
| --- | --- |
| Tratar a lista como spec | cada item exige spec própria e fonte de implementação |
| Prometer paridade de debugger no navegador | marcar D-001..D-004 como AVR-only; GDB é H-022 |
| Herdar auto-attach de Uno/Mega para o C3 | manter U-001 como `unknown` até evidência própria |
| Confundir documentação MIT com licença do simulador | atribuição MIT só para o texto; nada de asset/código |
| Envelhecimento da doc | commit pinado + data; revalidar antes de release |

## 7. Próximos passos sugeridos

1. `esp32-lead`: usar a lista para um ADR de **escopo de paridade do MVP**,
   marcando o que é paridade, o que é diferencial e o que é não objetivo.
2. `rea-evidence`: se o escopo exigir o catálogo de partes, abrir ficha
   separada para as 70 páginas — mesmo commit, mesma licença.
3. `firmware-runtime-engineer`: investigar boot/serial do C3 em hardware real
   ou QEMU, para substituir U-001 por evidência própria.
4. `clean-room-auditor`: revisar o ADR e qualquer spec derivada desta lista.
