# Fontes, licença e atribuição

## 1. Fonte primária

| Item | Valor |
| --- | --- |
| Repositório | `https://github.com/wokwi/wokwi-docs` |
| Commit | `5c90e532d24fb8ac4cd471e9568db03917011e91` |
| Branch de referência | `main` |
| Licença | MIT |
| Copyright | Copyright (c) 2021-2024 CodeMagic LTD |
| Texto da licença | [`artifacts/LICENSE-wokwi-docs.txt`](artifacts/LICENSE-wokwi-docs.txt) |
| Data da coleta | 2026-10-25 |
| Método | HTTP GET em `raw.githubusercontent.com`, somente leitura |

A documentação foi usada para extrair **nomes de campos, identificadores
públicos e comportamento documentado**. Nenhum código do simulador, asset,
imagem ou modelo 3D foi copiado.

## 2. Arquivos consultados

| ID (SRC) | Caminho no commit | URL raw | SHA-256 | Tamanho | Uso |
| --- | --- | --- | --- | ---: | --- |
| SRC-001 | `LICENSE` | `…/5c90e53…/LICENSE` | `4309562684d375e437100cb21a20d1278c7976ddf0869d7ed2946ad81fb0fc67` | 1.085 | licença |
| SRC-002 | `docs/diagram-format.md` | `…/5c90e53…/docs/diagram-format.md` | `7c8def287d78f1f8daa96643a0fa1ab8a0d586e64bcde7b556d4db2ad06ac711` | 6.541 | estrutura, partes, conexões, fios |
| SRC-003 | `docs/guides/esp32.md` | `…/5c90e53…/docs/guides/esp32.md` | `1fa2562ef3b7c162026a91d489b9639e2a2061467b6a2e95d2c165071436720b` | 18.236 | USB CDC, `$serialMonitor`, features |
| SRC-004 | `docs/parts/wokwi-led.md` | `…/5c90e53…/docs/parts/wokwi-led.md` | `55b0d58aa823571aaab418ab68480f84483bb41d458eb9d5f3041b6037898aa6` | 3.919 | pinos e atributos do LED |
| SRC-005 | `docs/guides/serial-monitor.md` | `…/5c90e53…/docs/guides/serial-monitor.md` | `433d712320da7e58574e1978442195743d65bd5155670d21f9dd22ea92de286b` | 6.420 | monitor serial e configuração |

Os IDs `SRC-*` identificam **arquivos**. Os IDs `EV-*` usados em
[`README.md`](README.md) e [`spec.md`](spec.md) identificam **fatos**
extraídos desses arquivos. A separação evita colisão de rastreabilidade.

O prefixo completo das URLs é
`https://raw.githubusercontent.com/wokwi/wokwi-docs/5c90e532d24fb8ac4cd471e9568db03917011e91/`.

## 3. O que foi citado

Apenas trechos curtos e nomes de campos, por exemplo:

- `version`, `author`, `editor`, `parts`, `connections`, `serialMonitor`;
- `board-esp32-c3-devkitm-1`, `wokwi-led`;
- `id`, `type`, `left`, `top`, `attrs`, `rotate`, `hide`;
- `A`, `C`, `color`, `lightColor`, `label`, `gamma`, `flip`, `fps`;
- `display`, `collapse`, `newline`, `convertEol`;
- `$serialMonitor:TX`, `$serialMonitor:RX`, `serialInterface`,
  `USB_SERIAL_JTAG`.

Não foram reproduzidas páginas inteiras, código de exemplo, imagens, GIFs ou
diagramas.

## 4. Atribuição

```text
Documentação de referência:
wokwi/wokwi-docs, commit 5c90e532d24fb8ac4cd471e9568db03917011e91
Copyright (c) 2021-2024 CodeMagic LTD
Licença MIT — texto completo em artifacts/LICENSE-wokwi-docs.txt
```

Se no futuro o projeto copiar porções substanciais dessa documentação, o aviso
de copyright e a permissão MIT devem acompanhar a distribuição, conforme a
licença.

## 5. Fontes consultadas apenas para o gate (não usadas como evidência)

| Fonte | Uso | Situação |
| --- | --- | --- |
| `https://wokwi.com/legal/terms` | verificar cláusulas C1–C6 | leitura de conformidade; versão 2026-10-01 |
| `https://docs.wokwi.com/` | confirmar que o conteúdo vem do repo MIT | `unknown`/`needs-decision`; não usado como fonte |
| `https://wokwi.com/legal/licenses` | verificar se havia licença do simulador | não é licença do simulador; não usado |
| `https://wokwi.com/robots.txt` | verificar política de crawling | 404; `unknown`; não usado |

Projetos públicos em `wokwi.com/projects/*` **não** foram acessados. Eles são
User Content sob os Termos e não são fonte de evidência.

## 6. Fontes planejadas para a próxima etapa (não usadas aqui)

| Fonte | Uso previsto | Licença/observação |
| --- | --- | --- |
| TRM/datasheet Espressif ESP32-C3 | CPU, memória, GPIO, UART, timers, boot | copyright Espressif; usar e citar, não redistribuir PDF |
| Arduino-ESP32 | firmware de teste, boot, compilação | LGPL-2.1 (confirmar na versão pinada) |
| ESP-IDF | boot, partições, imagem | Apache-2.0 |
| QEMU / fork lcgamboa | oráculo diferencial em CI | GPLv2; revisar antes de distribuir |
| Hardware real ESP32-C3 | trace e serial de referência | próprio |

## 7. Reprodução

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
  "$((Get-FileHash -Algorithm SHA256 $tmp).Hash.ToLower())  $((Get-Item $tmp).Length)  $f"
  Remove-Item $tmp -Force
}
```

Compare a saída com [`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt).
