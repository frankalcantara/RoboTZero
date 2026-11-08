# Evidência — inventário de funcionalidades documentadas (recorte ESP32-C3/MVP)

- **ID da evidência:** EV-2026-10-25-02
- **Data da coleta:** 2026-10-25 (America/Sao_Paulo)
- **Autor:** `esp32-lead`
- **Status:** concluída para o recorte documental; **alvo original bloqueado**
- **Parecer de gate:** [`../wokwi-c3-diagram-contract/gate.md`](../wokwi-c3-diagram-contract/gate.md)
- **Parecer clean-room desta ficha:** [`audit-clean-room.md`](audit-clean-room.md)

## 1. Pergunta

Quais funcionalidades do simulador Wokwi estão **documentadas publicamente** no
recorte relevante ao MVP do ESP32Sim (ESP32-C3, GPIO/UART/timer, serial,
diagrama, logic analyzer, debugger, Wi-Fi e bibliotecas) — sem observar o
serviço, sem copiar código/assets e sem transformar documentação em requisito?

A pergunta não é "como o Wokwi implementa", e sim "o que ele declara oferecer",
para servir de base a uma decisão de paridade e priorização.

## 2. Alvo e origem

| Item | Valor |
| --- | --- |
| Alvo original (observação) | `https://wokwi.com/projects/new/esp32-c3` |
| Situação do alvo original | **`blocked`** pelo gate legal/clean-room |
| Fonte efetiva | `github.com/wokwi/wokwi-docs` |
| Commit pinado | `5c90e532d24fb8ac4cd471e9568db03917011e91` (branch `main`) |
| Licença da fonte | MIT — Copyright (c) 2021-2024 CodeMagic LTD |
| Páginas lidas | 7 guias + `LICENSE` (ver [`sources.md`](sources.md)) |
| Ferramentas | PowerShell 5.1 (`Invoke-WebRequest`, `Get-FileHash`); consultas auxiliares à API do GitHub (não necessárias para reproduzir) |
| Base legal | Licença MIT do repositório de documentação; ToS do simulador **não** autoriza observação |

### 2.1 Avisos de uso

- **Escopo da licença:** a MIT cobre a **documentação** do repositório
  `wokwi/wokwi-docs`. Não cobre o software do simulador, o serviço, a marca nem
  eventuais imagens/assets do produto.
- **Nomes de marca:** identificadores como `Wokwi-GUEST`, `Wokwi IoT Gateway`,
  `$serialMonitor`, `wokwi-logic.vcd`, `host.wokwi.internal` e `@wokwi:`
  aparecem aqui como **fato documental**, para interoperabilidade e comparação.
  Não devem ser replicados como identidade do ESP32Sim; a decisão de naming
  exige ADR próprio.
- **MAC padrão:** `24:0a:c4:…` é um OUI da Espressif. Replicá-lo em dispositivo
  virtual é decisão de produto; a recomendação é usar MAC localmente
  administrado.

## 3. Decisão de gate (resumo)

O ToS do simulador (`https://wokwi.com/legal/terms`, "Last updated:
October 1st, 2026") veda uso não pessoal/não comercial, coleta automatizada,
acesso para desenvolver produto similar/concorrente e engenharia reversa sem
autorização escrita. O ESP32Sim é, por definição, um produto
similar/concorrente; não há autorização registrada.

**Conclusão:** observar o site para listar funcionalidades continua
**`blocked`** — inclusive sem copiar texto, porque a vedação é pela finalidade
do acesso (C3), não pelo ato de copiar. A rota usada é a documentação **MIT**,
com commit pinado. Detalhes e checklist de desbloqueio em
[`gate.md`](../wokwi-c3-diagram-contract/gate.md).

> As cláusulas do ToS acima estão registradas em `gate.md`; o texto do ToS
> **não foi revalidado nesta ficha** (o alvo não foi acessado). Trate a citação
> do ToS como `unknown` nesta evidência e consulte `gate.md` como fonte.

## 4. Escopo

**Dentro:**

- funcionalidades descritas nas 7 páginas de `docs/guides/` selecionadas;
- estado documentado de cada periférico **no ESP32-C3**;
- superfície em que a funcionalidade é oferecida (Web, VS Code, CLI, plano pago);
- avaliação do lead sobre relevância para o MVP (marcada como avaliação, não
  como fato).

**Fora:**

- observar o simulador, o runtime, o serial real ou qualquer artefato do alvo;
- catálogo de 70 partes/placas, `docs/vscode/`, `docs/wokwi-ci/`,
  `docs/chips-api/`;
- detalhes de implementação, preços, limites de plano além do que a doc cita;
- requisitos de produto derivados da lista.

## 5. Procedimento

1. Fixar o commit `5c90e53…` e baixar apenas as 7 páginas + `LICENSE` via
   `raw.githubusercontent.com`.
2. Calcular SHA-256 e tamanho de cada arquivo; registrar em
   [`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt).
3. Ler títulos, seções e tabelas; extrair **nomes de funcionalidades e
   atributos**, sem reproduzir o texto.
4. Classificar cada linha como `observado (documental)` e separar o que é
   avaliação do lead.
5. Verificar os hashes por re-download (8/8 OK).
6. Submeter a ficha ao `clean-room-auditor` (somente leitura).

Comando reproduzível em [`sources.md`](sources.md) §5.

## 6. Resultado

- **85 linhas** inventariadas em 8 áreas (F, H, E, S, L, D, W, B) — 84
  funcionalidades + 1 agregado de ausências (H-023).
- **30 linhas** marcadas como `MVP`, **32** como `Fase 2`, **12** como
  `Fase 3`, **10** como `Fora` e **1** como `n/a` (H-023) — avaliação do lead.
- A lista completa, com fonte por linha, está em [`inventory.md`](inventory.md).

### 6.1 Achados que mudam a expectativa

| Achado | Consequência |
| --- | --- |
| O debugger embutido do studio é **beta e só para AVR** (Uno, Nano, Mega, ATtiny85) | O C3 **não** tem debugger no navegador; o caminho documentado é GDB via VS Code (H-022) |
| Auto-attach e auto-baud do serial estão documentados **para Uno/Mega** | Para o C3, o comportamento padrão é `unknown` (U-001); a opção USB CDC é explícita (S-009) |
| Várias funcionalidades são **de plano pago** (APs customizados, Private Gateway, upload de biblioteca) | Paridade de produto não é só técnica; envolve decisão de plano |
| Parte das funcionalidades é **VS Code/CLI**, não Web | O inventário separa superfície para não prometer no studio o que é de outra superfície |
| A matriz marca lacunas no C3 (I2S, Bluetooth, MCPWM, PCNT, ECC, Hall, ULP; sem PSRAM) | Não faz sentido prometer paridade nesses itens no MVP |
| `partitions.csv` só vale no studio web | O contrato de build do ESP32Sim precisa decidir onde a tabela vive |

## 7. Classificação das conclusões

| Nível | Itens |
| --- | --- |
| **Observado (documental)** | as 84 linhas de funcionalidade — o documento afirma a funcionalidade. `H-023` é agregado de ausências (`n/a`), não uma funcionalidade |
| **Derivado** | os agrupamentos por área, a contagem e a separação por superfície |
| **Inferido** | nenhum item promovido |
| **Desconhecido** | U-001..U-005 em [`inventory.md`](inventory.md) §10 |

Nenhuma linha foi verificada em execução. "Documentado" não significa
"funciona hoje" nem "funciona no C3".

## 8. Limitações

- Recorte de 7 páginas; o restante da documentação não foi inventariado.
- A documentação pode estar desatualizada ou incompleta em relação ao serviço.
- A coluna de relevância é julgamento do lead, não fato do documento.
- A matriz de periféricos é uma declaração do fornecedor; não foi testada.
- O alvo segue `blocked`; qualquer verificação de comportamento exige o
  checklist de desbloqueio.
- Nenhum firmware, hardware ou teste de timing foi executado.

## 9. Implicações para o projeto

1. A lista serve para **priorizar paridade**, não para copiar produto: cada
   item precisa de decisão de escopo e de uma fonte própria de implementação
   (TRM, Arduino-ESP32, ESP-IDF, hardware).
2. O MVP do ESP32Sim deve assumir que **debugger no navegador para C3** não é
   paridade exigida pelo que está documentado; GDB pode ser fase posterior.
3. O serial do C3 precisa de decisão explícita (USB CDC x UART), porque a doc
   não declara auto-attach para essa placa.
4. Logic analyzer, Wi-Fi e bibliotecas são fases distintas; misturá-los no MVP
   aumenta risco sem entregar o blink ponta a ponta.
5. Nenhum item desta lista vira requisito sem spec própria e revisão
   clean-room.

> **Disposição vigente (2026-10-25):** a decisão de escopo sobre estas 85 linhas
> está no [`ADR 0005 — Escopo de paridade do MVP`](../../../docs/adr/0005-escopo-de-paridade-do-mvp.md);
> a decisão de identidade e defaults está no
> [`ADR 0006`](../../../docs/adr/0006-identidade-e-defaults.md). Este inventário
> continua sendo **insumo**, não requisito.

## 10. Artefatos

```text
evidence/2026-10-25/wokwi-feature-inventory/
  README.md                  esta ficha
  inventory.md               lista das 85 linhas (84 funcionalidades + H-023)
  sources.md                 fontes, licença e reprodução
  notes.md                   análise e lacunas
  audit-clean-room.md        parecer do auditor
  MANIFEST.sha256            hashes dos arquivos entregues
  artifacts/
    LICENSE-wokwi-docs.txt   licença MIT da fonte
    raw-sha256.txt           hashes dos 8 arquivos-fonte pinados
```

## 11. Hashes e verificação

- Fontes: [`artifacts/raw-sha256.txt`](artifacts/raw-sha256.txt) — **8/8 OK**
  por re-download em 2026-10-25.
- Entregues: [`MANIFEST.sha256`](MANIFEST.sha256).
- Procedimento:

```powershell
Set-Location ESP32Sim/evidence/2026-10-25/wokwi-feature-inventory
Get-Content MANIFEST.sha256 -Encoding UTF8 | ForEach-Object {
  $h, $p = $_ -split '  ', 2
  $actual = (Get-FileHash -Algorithm SHA256 -Path $p).Hash.ToLower()
  if ($actual -eq $h) { "OK   $p" } else { "FAIL $p" }
}
```

Resultado registrado no fechamento desta ficha (ver §12).

## 12. Fechamento

- Verificação do `MANIFEST.sha256`: **7/7 OK** (7 arquivos entregues, execução
  de 2026-10-25).
- Fontes pinadas: **8/8 OK** por re-download em 2026-10-25.
- Nenhuma implementação foi iniciada.
- Nenhum commit/push foi feito.
