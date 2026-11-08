# Evidência — contrato de `diagram.json` para blink ESP32-C3

- **ID da evidência:** EV-2026-10-25-01
- **Data da coleta:** 2026-10-25 (America/Sao_Paulo)
- **Autor:** `esp32-lead`
- **Status:** concluída para o contrato documentado; **alvo original bloqueado**
- **Parecer de gate:** [`audit-clean-room.md`](audit-clean-room.md)
- **Parecer da spec (v0.1):** [`audit-spec.md`](audit-spec.md)
- **Reauditoria (v0.2):** [`audit-spec-v0.2.md`](audit-spec-v0.2.md)
- **Tratamento dos achados:** [`response-to-audit.md`](response-to-audit.md)

## 1. Pergunta

Qual é o contrato mínimo, **documentado publicamente**, do arquivo
`diagram.json` de um projeto de *blink* em ESP32-C3 que o ESP32Sim precisa
aceitar e emitir para interoperar com projetos existentes, sem reutilizar
código, assets ou implementação proprietária do alvo?

Perguntas secundárias, dentro do mesmo contrato:

- Como a placa ESP32-C3 e um LED são identificados no diagrama?
- Como as conexões elétricas são representadas?
- Como o monitor serial é anexado e configurado?
- O que permanece `unknown` e não pode virar requisito?

## 2. Alvo e origem

| Item | Valor |
| --- | --- |
| Alvo original (observação) | `https://wokwi.com/projects/new/esp32-c3` |
| Situação do alvo original | **`blocked`** pelo gate legal/clean-room |
| Fonte efetiva desta evidência | `github.com/wokwi/wokwi-docs` |
| Commit pinado | `5c90e532d24fb8ac4cd471e9568db03917011e91` (branch `main`) |
| Licença da fonte | MIT — Copyright (c) 2021-2024 CodeMagic LTD |
| Data/hora da coleta | 2026-10-25T02:10:34-03:00 |
| Ferramentas | PowerShell 5.1 (`Invoke-WebRequest`, `Get-FileHash`); Node.js v22.23.4 usado apenas para consultas auxiliares à API GitHub e contagens de ocorrências — não é necessário para reproduzir a coleta |
| Base legal | Licença MIT do repositório de documentação; ToS do simulador **não** autoriza observação |

## 3. Decisão de gate (resumo)

O ToS do simulador (`https://wokwi.com/legal/terms`, "Last updated:
October 1st, 2026") proíbe, de forma cumulativa:

1. uso que não seja **pessoal e não comercial**;
2. **sistemas automatizados** de coleta;
3. acesso para **desenvolver produto similar ou concorrente**;
4. **engenharia reversa** do software sem autorização escrita.

O ESP32Sim é, por definição, um produto similar/concorrente. Não há
autorização escrita da Wokwi, parecer jurídico profissional ou conta/perfil
dedicado registrados no repositório. Conclusão: **`blocked`** para observar o
simulador. Nenhuma captura, screenshot, bundle, DOM, WebSocket ou artefato do
alvo foi coletado.

O parecer do `clean-room-auditor` indicou uma rota lícita: a documentação do
projeto é publicada em repositório **MIT** (`wokwi/wokwi-docs`). A coleta foi
redirecionada para essa fonte, com commit pinado. O escopo foi reduzido ao
**contrato de arquivo de diagrama**, não ao comportamento interno do
simulador. Detalhes em [`gate.md`](gate.md).

## 4. Escopo

**Dentro do escopo:**

- estrutura de `diagram.json` (versão, partes, conexões);
- identificadores públicos de placa ESP32-C3 e de LED;
- semântica documentada de conexões e do mini-language de fios;
- configuração documentada do monitor serial;
- requisitos de importação/exportação derivados desse contrato.

**Fora do escopo (não investigado):**

- runtime, timing, CPU, GPIO, UART ou boot do simulador do alvo;
- fluxo de compilação, endpoints, WebSocket, UI, DOM ou bundles;
- assets 3D, CSS, imagens, fontes ou modelos do alvo;
- fidelidade elétrica ou ciclo a ciclo;
- outros chips e outros componentes;
- nome/estrutura dos arquivos de código do projeto (ex.: `sketch.ino`) —
  não documentado nas páginas coletadas; permanece `unknown`.

## 5. Hipóteses e critérios observáveis

| ID | Hipótese | Critério observável | Resultado |
| --- | --- | --- | --- |
| H1 | O formato de diagrama é versionado e tem partes/conexões como arrays | Documento público com `version`, `parts`, `connections` | confirmada (documental) |
| H2 | A placa ESP32-C3 tem um identificador público estável | Identificador aparece na lista de microcontroladores | confirmada (documental) |
| H3 | O LED tem pinos e atributos públicos | Pinos A/C e atributo `color` documentados | confirmada (documental) |
| H4 | Conexões usam `id:pino` e uma tupla posicional | Exemplo público de conexão | confirmada (documental) |
| H5 | O monitor serial é anexado por padrão à UART de hardware | Texto público sobre auto-attach e auto-baud | confirmada **apenas para Arduino Uno/Mega** (documental); para o C3, `unknown` (UN-007) |
| H6 | O runtime do alvo aceita exatamente esse contrato | Execução observada no alvo | **não testada** — `unknown` |

## 6. Procedimento reproduzível

### 6.1 Coleta (somente leitura)

```powershell
$sha  = "5c90e532d24fb8ac4cd471e9568db03917011e91"
$base = "https://raw.githubusercontent.com/wokwi/wokwi-docs/$sha"
$files = @(
  "LICENSE",
  "docs/diagram-format.md",
  "docs/guides/esp32.md",
  "docs/parts/wokwi-led.md",
  "docs/guides/serial-monitor.md"
)

foreach ($f in $files) {
  $tmp = Join-Path $env:TEMP ("ev-" + [guid]::NewGuid().ToString("N") + ".bin")
  Invoke-WebRequest -Uri "$base/$f" -OutFile $tmp -UseBasicParsing
  $h   = (Get-FileHash -Algorithm SHA256 -Path $tmp).Hash.ToLower()
  $len = (Get-Item $tmp).Length
  "$h  $len  $f"
  Remove-Item $tmp -Force
}
```

Saída esperada (registrada em
[`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt)):

```text
4309562684d375e437100cb21a20d1278c7976ddf0869d7ed2946ad81fb0fc67  1085  LICENSE
7c8def287d78f1f8daa96643a0fa1ab8a0d586e64bcde7b556d4db2ad06ac711  6541  docs/diagram-format.md
1fa2562ef3b7c162026a91d489b9639e2a2061467b6a2e95d2c165071436720b  18236  docs/guides/esp32.md
55b0d58aa823571aaab418ab68480f84483bb41d458eb9d5f3041b6037898aa6  3919  docs/parts/wokwi-led.md
433d712320da7e58574e1978442195743d65bd5155670d21f9dd22ea92de286b  6420  docs/guides/serial-monitor.md
```

### 6.2 Verificação dos artefatos entregues

```powershell
Set-Location ESP32Sim/evidence/2026-10-25/wokwi-c3-diagram-contract
Get-Content MANIFEST.sha256 | ForEach-Object {
  $h, $p = $_ -split '  ', 2
  $actual = (Get-FileHash -Algorithm SHA256 -Path $p).Hash.ToLower()
  if ($actual -eq $h) { "OK   $p" } else { "FAIL $p" }
}
```

Resultado da última execução: **todos os arquivos `OK`** (ver §13).

### 6.3 Revisão de consistência

- O identificador `board-esp32-c3-devkitm-1` foi encontrado em
  `docs/diagram-format.md` (1 ocorrência).
- `wokwi-led` aparece em `docs/diagram-format.md` (5) e
  `docs/parts/wokwi-led.md` (9).
- `$serialMonitor` aparece em `docs/guides/serial-monitor.md` (8) e
  `docs/guides/esp32.md` (1).
- `serialInterface` / `USB_SERIAL_JTAG` aparecem em `docs/guides/esp32.md`
  (1 cada).
- Não há segunda fonte independente para o identificador da placa C3 nas
  páginas coletadas; isso é uma limitação registrada em §9.

## 7. Observado (fatos registrados)

| ID | Fato | Fonte | Nível |
| --- | --- | --- | --- |
| EV-001 | O repositório `wokwi/wokwi-docs` é MIT, Copyright (c) 2021-2024 CodeMagic LTD | `LICENSE` @ commit | observado |
| EV-002 | `diagram.json` tem `version` (sempre 1), `author`, `editor`, `parts` e `connections`; pode ter `serialMonitor` | `docs/diagram-format.md` | observado |
| EV-003 | Cada parte tem `id` e `type` obrigatórios; `left`, `top`, `attrs`, `rotate`, `hide` opcionais; `id` deve ser único | `docs/diagram-format.md` | observado |
| EV-004 | A placa ESP32-C3 é identificada como `board-esp32-c3-devkitm-1` | `docs/diagram-format.md` | observado |
| EV-005 | O LED é `wokwi-led`, com pinos `A` (anodo) e `C` (catodo); `color` tem default `"red"`; há `lightColor`, `label`, `gamma`, `flip`, `fps` | `docs/parts/wokwi-led.md` | observado |
| EV-006 | Cada conexão é um array de 4 itens: `id:pino` origem, `id:pino` destino, cor (string vazia oculta o fio) e lista opcional de instruções de fio | `docs/diagram-format.md` | observado |
| EV-007 | O mini-language de fios usa `vN`/`hN`, um único `*`, e as instruções após `*` são aplicadas em ordem reversa | `docs/diagram-format.md` | observado |
| EV-008 | Para Arduino Uno/Mega, o monitor serial anexa automaticamente à UART de hardware e detecta o baud rate; globalmente, o default `display: "auto"` só mostra o monitor quando há saída. O auto-attach/auto-baud do C3 não está declarado (UN-007) | `docs/guides/serial-monitor.md` | observado com qualificação |
| EV-009 | O bloco `serialMonitor` tem defaults `collapse=false`, `convertEol=false`, `display="auto"`, `newline="lf"`; `display` aceita `auto`, `always`, `never`, `plotter`, `terminal` | `docs/guides/serial-monitor.md` | observado |
| EV-010 | `newline` aceita `lf`, `cr`, `crlf`, `none`; o default é `lf` | `docs/guides/serial-monitor.md` | observado |
| EV-011 | É possível rotear o monitor serial por conexões explícitas `$serialMonitor:TX` / `$serialMonitor:RX` | `docs/guides/serial-monitor.md` e `docs/guides/esp32.md` | observado |
| EV-012 | Para USB CDC, o chip aceita `serialInterface: "USB_SERIAL_JTAG"` e as conexões com `$serialMonitor` devem ser removidas | `docs/guides/esp32.md` | observado |
| EV-013 | O ESP32-C3 aparece na matriz de features com GPIO, UART, USB Serial/JTAG (serial, sem JTAG), timers, watchdog, etc. | `docs/guides/esp32.md` | observado (contextual, não normativo) |

## 8. Derivado

| ID | Conclusão | Base | Nível |
| --- | --- | --- | --- |
| DV-001 | É possível importar/exportar um blink C3 sem observar o simulador: o contrato está descrito em fonte MIT | EV-001..EV-012 | derivado |
| DV-002 | O bundle mínimo de diagrama para blink é: uma parte de placa C3, uma parte `wokwi-led`, duas conexões elétricas (um pino de I/O da placa ao anodo do LED; o catodo a um pino de referência) e o bloco `serialMonitor` (obrigatoriedade não declarada — UN-002). **Os nomes exatos dos pinos da placa não estão documentados nas páginas coletadas (UN-009).** | EV-002..EV-007, EV-009 | derivado |
| DV-003 | Para Arduino Uno/Mega, o monitor serial não exige conexão explícita no caso padrão; a conexão explícita é usada para portas alternativas ou USB CDC. Para o C3, o comportamento padrão é `unknown` (UN-007) | EV-008, EV-011, EV-012 | derivado |
| DV-004 | O contrato pode ser tratado como formato de **importação/exportação** no studio, sem entrar no core | EV-002..EV-012 + `docs/03-arquitetura.md` §10 | derivado |
| DV-005 | O nome do arquivo de código (`sketch.ino`) e o fluxo de compilação não estão cobertos pelas páginas coletadas | ausência de evidência | derivado (lacuna) |

## 9. Inferido e desconhecido

| ID | Item | Classificação | Por quê |
| --- | --- | --- | --- |
| UN-001 | O runtime do alvo aceita exatamente este contrato e produz o comportamento descrito | **desconhecido** | não houve execução no alvo (bloqueado) |
| UN-002 | O editor exige ou preserva `author` e `editor` | **desconhecido** | a doc os descreve, mas não diz se são obrigatórios em runtime |
| UN-003 | `left`/`top` são obrigatórios na prática | **desconhecido** | a doc diz "opcional"; não testado |
| UN-004 | Nome e organização dos arquivos de código do projeto | **desconhecido** | não documentado nas páginas coletadas |
| UN-005 | Comportamento elétrico/timing do GPIO e do LED | **desconhecido** | exige TRM/hardware; fora do escopo |
| UN-006 | Mensagens de erro e validação do editor | **desconhecido** | não documentadas |
| UN-007 | Se o C3 usa USB CDC por padrão no alvo | **desconhecido** | a doc descreve a opção, não o default por placa |
| UN-008 | Compatibilidade futura: o formato pode mudar sem aviso | **desconhecido** | a doc é versionada, mas não há compromisso de estabilidade |
| UN-009 | Nomes de pino da placa `board-esp32-c3-devkitm-1` (GPIO, GND, 3V3 etc.) | **desconhecido** | as páginas coletadas não listam os pinos da placa; a doc sugere descobri-los na UI do alvo, que está `blocked` |

Nenhum item acima foi promovido a requisito.

## 10. Limitações

- A evidência é **documental**, não comportamental. Não houve observação do
  simulador, do runtime, do serial real ou de qualquer artefato do alvo.
- A documentação pode estar desatualizada em relação ao serviço; o commit
  pinado é a verdade desta ficha.
- Foram lidas apenas 5 páginas. A lista de partes e placas é maior e não foi
  varrida por completo.
- Não há segunda fonte independente para o identificador
  `board-esp32-c3-devkitm-1` nas páginas coletadas.
- O ToS do alvo continua **`blocked`**; qualquer mudança exige o checklist de
  desbloqueio em [`gate.md`](gate.md).
- Nenhum teste de hardware ou de firmware foi executado nesta etapa.

## 11. Implicações para o projeto

1. O studio pode oferecer importação/exportação de `diagram.json` v1 como
   **adaptador de interoperabilidade**, mantendo o modelo interno do ESP32Sim
   (JSON só na fronteira; o core recebe representação binária — arquitetura
   §10).
2. O mapeamento de tipos públicos (`board-esp32-c3-devkitm-1`, `wokwi-led`)
   para IDs internos deve ser uma tabela versionada, não uma dependência do
   core.
3. O monitor serial é uma projeção de eventos do core na UI; não pode virar
   estado no TypeScript.
4. O que falta para um blink ponta a ponta (código, compilação, boot, UART
   física, timing) exige investigação separada com fontes públicas
   (Arduino-ESP32, TRM Espressif) e hardware próprio.
5. Nenhum asset, código ou dado proprietário foi copiado.

## 12. Licença e clean-room

- Material de origem: documentação MIT (`wokwi/wokwi-docs`), com aviso de
  copyright preservado em [`artifacts/LICENSE-wokwi-docs.txt`](artifacts/LICENSE-wokwi-docs.txt)
  e em [`sources.md`](sources.md).
- Material proprietário no repositório: **não**.
- Código do alvo copiado: **não**.
- Capturas do alvo: **não**.
- Revisão: gate pelo `clean-room-auditor` (sessão `ses_e8e8b2d50ffe3akKFu8KT7iz4r`);
  revisão da spec v0.1 em [`audit-spec.md`](audit-spec.md); reauditoria da
  v0.2 em [`audit-spec-v0.2.md`](audit-spec-v0.2.md); tratamento dos achados em
  [`response-to-audit.md`](response-to-audit.md).

## 13. Hashes e verificação

- Hashes dos arquivos-fonte pinados: [`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt).
- Hashes dos arquivos entregues: [`MANIFEST.sha256`](MANIFEST.sha256).
- Procedimento de verificação: §6.2.
- Resultado da última verificação (2026-10-25): **12/12 arquivos `OK`**
  (README, gate, sources, notes, spec, handoff, audit-clean-room, audit-spec,
  audit-spec-v0.2, response-to-audit, LICENSE da fonte e raw-sha256).

## 14. Artefatos

```text
evidence/2026-10-25/wokwi-c3-diagram-contract/
  README.md                  esta ficha
  gate.md                    gate legal/clean-room e decisão
  sources.md                 fontes, licença e atribuição
  notes.md                   análise e classificação
  spec.md                    especificação comportamental
  handoff.md                 handoff para a próxima etapa
  audit-clean-room.md        parecer de gate
  audit-spec.md              parecer da spec v0.1
  audit-spec-v0.2.md         reauditoria da spec v0.2
  response-to-audit.md       tratamento dos achados pelo lead
  MANIFEST.sha256            hashes dos arquivos entregues
  artifacts/
    LICENSE-wokwi-docs.txt   licença MIT da fonte
    raw-sha256.txt           hashes dos arquivos-fonte pinados
```
