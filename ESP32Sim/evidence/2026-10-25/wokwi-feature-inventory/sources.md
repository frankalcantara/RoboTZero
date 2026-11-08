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

A documentação foi usada para **inventariar funcionalidades e nomes de
atributos**. Nenhum código do simulador, asset, imagem ou modelo 3D foi
copiado.

## 2. Arquivos consultados

Prefixo das URLs:
`https://raw.githubusercontent.com/wokwi/wokwi-docs/5c90e532d24fb8ac4cd471e9568db03917011e91/`

| ID | Caminho no commit | SHA-256 | Bytes | Uso |
| --- | --- | --- | ---: | --- |
| SRC-001 | `LICENSE` | `4309562684d375e437100cb21a20d1278c7976ddf0869d7ed2946ad81fb0fc67` | 1.085 | licença |
| SRC-002 | `docs/guides/esp32.md` | `1fa2562ef3b7c162026a91d489b9639e2a2061467b6a2e95d2c165071436720b` | 18.236 | placas, linguagens, matriz de periféricos, atributos avançados |
| SRC-003 | `docs/guides/serial-monitor.md` | `433d712320da7e58574e1978442195743d65bd5155670d21f9dd22ea92de286b` | 6.420 | monitor serial e configuração |
| SRC-004 | `docs/guides/diagram-editor.md` | `277f87d5b38d9163e354dfd9550014c22874799190ce5a30100f71f621ee4a57` | 6.514 | edição de partes, fios, atalhos e grid |
| SRC-005 | `docs/guides/logic-analyzer.md` | `823f6c11891cccd1dde7e80781104bf7c2412b51534e41f0e781d815a1109c14` | 8.254 | canais, trigger, VCD |
| SRC-006 | `docs/guides/debugger.md` | `6c4c798db6930c303a9dfefd98ae8f85abef791be5f7519e71016fb993a11210` | 2.386 | debugger embutido (AVR) |
| SRC-007 | `docs/guides/esp32-wifi.md` | `94e21f703581f0c8fe76d91a2a54013ee96b935f2be3de0a824df5e675d23b49` | 11.526 | AP virtual, gateways, PCAP |
| SRC-008 | `docs/guides/libraries.md` | `80164510ef8657abdbed8c97025ad1c8937a80bf2d7f51e0afa206c2ea054225` | 2.874 | bibliotecas e `libraries.txt` |

Os IDs `SRC-*` identificam **arquivos**. O inventário cita página + seção, não
um ID de fato, para evitar colisão com a evidência
[`2026-10-25/wokwi-c3-diagram-contract`](../wokwi-c3-diagram-contract/README.md).

## 3. Atribuição

```text
Documentação de referência:
wokwi/wokwi-docs, commit 5c90e532d24fb8ac4cd471e9568db03917011e91
Copyright (c) 2021-2024 CodeMagic LTD
Licença MIT — texto completo em artifacts/LICENSE-wokwi-docs.txt
```

Se o projeto vier a copiar porções substanciais dessa documentação, o aviso de
copyright e a permissão MIT devem acompanhar a distribuição.

## 4. O que não foi usado

- `wokwi.com` (simulador) — **`blocked`** pelo gate
  [`../wokwi-c3-diagram-contract/gate.md`](../wokwi-c3-diagram-contract/gate.md).
- Projetos públicos da comunidade (`wokwi.com/projects/*`) — User Content, sem
  licença conferida.
- Imagens, GIFs, SVGs, screenshots, bundles, WASM ou modelos 3D.
- `docs.wokwi.com` renderizado — a rota preferida é o repositório MIT.

## 5. Reprodução

```powershell
$sha  = "5c90e532d24fb8ac4cd471e9568db03917011e91"
$base = "https://raw.githubusercontent.com/wokwi/wokwi-docs/$sha"
$files = @(
  "LICENSE",
  "docs/guides/esp32.md",
  "docs/guides/serial-monitor.md",
  "docs/guides/diagram-editor.md",
  "docs/guides/logic-analyzer.md",
  "docs/guides/debugger.md",
  "docs/guides/esp32-wifi.md",
  "docs/guides/libraries.md"
)
foreach ($f in $files) {
  $tmp = Join-Path $env:TEMP ("ev-" + [guid]::NewGuid().ToString("N") + ".bin")
  Invoke-WebRequest -Uri "$base/$f" -OutFile $tmp -UseBasicParsing
  "$((Get-FileHash -Algorithm SHA256 $tmp).Hash.ToLower())  $((Get-Item $tmp).Length)  $f"
  Remove-Item $tmp -Force
}
```

Compare com [`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt). A
verificação executada em 2026-10-25 resultou em **8/8 OK**.
