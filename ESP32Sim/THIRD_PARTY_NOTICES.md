# Third-party notices

Este arquivo registra licenças e obrigações de materiais de terceiros usados no
repositório ESP32Sim. Ele complementa — e não substitui — a revisão de
[`docs/08-riscos-e-licencas.md`](docs/08-riscos-e-licencas.md).

## Documentação de referência

### `wokwi/wokwi-docs`

| Item | Valor |
| --- | --- |
| Origem | `https://github.com/wokwi/wokwi-docs` |
| Commit usado | `5c90e532d24fb8ac4cd471e9568db03917011e91` |
| Uso | Inventário documental de funcionalidades em [`evidence/2026-10-25/wokwi-feature-inventory/`](evidence/2026-10-25/wokwi-feature-inventory/README.md) |
| Material usado | Texto da documentação (consulta, paráfrase e notas técnicas curtas). Nenhum código, asset, imagem ou modelo |
| Licença | MIT |
| Copyright | Copyright (c) 2021-2024 CodeMagic LTD |
| Cópia da licença | [`evidence/2026-10-25/wokwi-feature-inventory/artifacts/LICENSE-wokwi-docs.txt`](evidence/2026-10-25/wokwi-feature-inventory/artifacts/LICENSE-wokwi-docs.txt) |

Texto da licença:

```text
The MIT License (MIT)

Copyright (c) 2021-2024 CodeMagic LTD

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

## Ferramentas de desenvolvimento

### `DietrichGebert/ponytail`

| Item | Valor |
| --- | --- |
| Origem | `https://github.com/DietrichGebert/ponytail` |
| Uso | Skill de método de desenvolvimento em [`.opencode/skills/ponytail/`](../.opencode/skills/ponytail/SKILL.md) |
| Material usado | Texto das instruções da skill |
| Licença | MIT |
| Copyright | Copyright (c) 2026 DietrichGebert |
| Cópia da licença | [`.opencode/skills/ponytail/LICENSE`](../.opencode/skills/ponytail/LICENSE) |

A skill orienta o processo (menor mudança que resolve, teste junto, sem
abstração especulativa). Não contém código, asset ou dado do produto e não é
embarcada no `.wasm` nem no studio.

## Escopo e marcas

- A licença MIT cobre a **documentação**. Não cobre o software do simulador, a
  marca, eventuais imagens/assets do produto de origem nem o serviço.
- "Wokwi" é usado apenas para citar proveniência. O produto não usa a marca em
  nome, logo, domínio ou identificador; ver
  [`docs/adr/0006-identidade-e-defaults.md`](docs/adr/0006-identidade-e-defaults.md).
- Nenhum código, asset, imagem, modelo 3D ou dado proprietário do alvo foi
  copiado.

## Manutenção

- Atualizar este arquivo quando uma nova dependência ou material de terceiros
  entrar no repositório.
- Registrar a obrigação (aviso, fonte, patente, marca) e o modo de uso.
- Seguir o checklist de release de `docs/08-riscos-e-licencas.md` §8.
