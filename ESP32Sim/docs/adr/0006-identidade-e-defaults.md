# ADR 0006 — Identidade e defaults de interoperabilidade

- **Status:** Aceito
- **Data:** 2026-10-25
- **Data de aceite:** 2026-10-25
- **Decisores:** lead, produto, clean-room
- **Evidência de entrada:** EV-2026-10-25-02 —
  [`evidence/2026-10-25/wokwi-feature-inventory/`](../../evidence/2026-10-25/wokwi-feature-inventory/README.md)
- **Relaciona-se com:** ADR 0005 (escopo de paridade),
  [`08-riscos-e-licencas.md`](../08-riscos-e-licencas.md) §5 (política de marcas)

## Contexto

O inventário documental cita identificadores públicos do Wokwi que carregam
marca ou convenção do terceiro: `Wokwi-GUEST`, `Wokwi IoT Gateway`,
`$serialMonitor`, `wokwi-logic.vcd`, `host.wokwi.internal` e `@wokwi:`. Também
cita defaults de rede (`24:0a:c4:00:01:10`, `42:13:37:55:aa:01`, `10.10.0.2`,
`10.13.37.2`).

A licença MIT cobre a **documentação**, não o software do simulador, a marca ou
eventuais imagens/assets. O parecer clean-room recomendou registrar em ADR a
decisão de **não reutilizar identificadores de marca** no produto, e a política
de marcas de `08` §5 já veda sugerir afiliação ou endosso.

O uso nominativo de "Wokwi" para citar proveniência em evidências e ADRs é
legítimo. O que esta ADR decide é o que **não** entra na identidade, no schema e
nos defaults do produto.

> **Atribuição:** `wokwi/wokwi-docs` @ `5c90e532d24fb8ac4cd471e9568db03917011e91`,
> MIT, Copyright (c) 2021-2024 CodeMagic LTD. Nenhum código, asset ou imagem do
> alvo foi usado.

## Decisão proposta

### 1. Identidade do produto

- "Wokwi" **não** entra em nome de produto, logo, domínio, nome de pacote ou
  identificador de API.
- O produto não sugere afiliação, patrocínio ou endosso.
- O disclaimer de `08` §5 é obrigatório em README, site e materiais públicos:
  "Projeto independente, não afiliado à Espressif ou à Wokwi."

### 2. Identificadores de configuração e de UI

Não adotar os identificadores do terceiro como contrato do produto. Usar nomes
neutros e próprios:

| Referência documental | Não usar como identidade | Uso interno proposto |
| --- | --- | --- |
| `Wokwi-GUEST` | SSID padrão do AP virtual | SSID próprio, configurável |
| `Wokwi IoT Gateway` | nome de gateway/rede | "gateway virtual", com nome próprio |
| `$serialMonitor` | id de conexão do diagrama | `serial` (nosso id) |
| `wokwi-logic.vcd`, `wokwi.vcd` | nome de arquivo exportado | `logic.vcd` (nosso) |
| `wokwi.pcap` | captura de rede | `capture.pcap` (nosso) |
| `host.wokwi.internal` | hostname de acesso ao host | hostname próprio, configurável |
| `@wokwi:` | prefixo de biblioteca custom | id próprio, sem prefixo de terceiro |
| `diagram.json` | formato de projeto do terceiro | `circuit.json` (nosso); `diagram.json` só no adaptador de importação |
| `wokwi-led`, `board-esp32-c3-devkitm-1` | id de parte/placa | id interno próprio e versionado |
| `wokwi.toml` | arquivo de configuração do VS Code | `esp32sim.toml` (nosso) |

Nomes genéricos de função (`serialMonitor`, `logicAnalyzer`) podem ser usados se
forem o nosso contrato; o que se evita é o identificador **específico** do
terceiro e a aparência de marca.

### 2.1 Nomes de placas e partes

- Nomes de placas da Espressif são descritivos: "compatível com
  ESP32-C3-DevKitM-1", nunca "oficial" ou "homologado".
- O **id interno** de placa/parte é próprio e versionado; não herda
  `board-esp32-c3-devkitm-1` nem `wokwi-led`.
- A definição de placa de `02` D-007 usa o id próprio; a correspondência com
  nomes de terceiros, se necessária, fica no adaptador.

### 3. Importação de projetos do terceiro

Se houver suporte a importar projetos do Wokwi:

- a tradução vive em um **adaptador**, não no schema do core;
- o adaptador tem tabela de mapeamento explícita e testada;
- a atribuição e a origem ficam registradas;
- o schema interno continua com os nomes do produto;
- a importação é interoperabilidade, não adoção de identidade.

Sem essa decisão de produto, **não** há compromisso de importação.

> **Cross-reference:** esta seção fecha, para o produto, o item aberto de
> [`handoff.md`](../../evidence/2026-10-25/wokwi-c3-diagram-contract/handoff.md) §4
> ("ADR decidindo adotar (ou não) `diagram.json` v1"). A decisão é: `diagram.json`
> v1 é adotado **apenas como formato de importação/exportação no adaptador**;
> `circuit.json` continua sendo o schema interno.

### 4. Defaults de rede

- **MAC padrão:** localmente administrado (**LAA**) e **unicast** (primeiro
  octeto com o bit local setado e o bit multicast zerado), configurável.
  Derivação **determinística** a partir da seed do projeto, para respeitar o
  determinismo. Não usar `24:0a:c4:…`, que é OUI da Espressif.
- **Base MAC do eFuse stub:** própria e determinística; é dela que
  `esp_read_mac` deriva o MAC do firmware. A regra vale para a base, não só para
  o MAC exposto na UI.
- **BSSID do AP virtual:** próprio e LAA; não copiar `42:13:37:55:aa:01`.
- **Endereços IP:** default próprio e determinístico em faixa privada, por
  exemplo `192.168.4.0/24`; configurável. Não copiar `10.10.0.2`/`10.13.37.2`.
  IP privado não é marca; a regra é evitar herdar o default do terceiro.
- **Nomes de arquivo:** próprios (`logic.vcd`, `capture.pcap`), não
  `wokwi-logic.vcd`/`wokwi.pcap`.
- **Acesso ao host:** hostname próprio, desabilitado por padrão, com opt-in
  explícito e aviso de exposição de rede local. Revisar sob `08` §6 antes de
  habilitar.

### 5. Onde os nomes do terceiro podem aparecer

- Em `evidence/`, em ADRs e em documentação de proveniência, como **citação**.
- Em documentação de migração/importação, com atribuição.
- No **adaptador de importação** e nas fixtures dele. O adaptador é um módulo
  isolado, com diretório nomeado (`apps/import/`, nome provisório) e fixtures em
  `tests/fixtures/import/`.
- Em nenhum outro diretório de produto: `core/`, `include/`, `wasm/`, `apps/`
  (exceto `apps/import/`), `services/`, `tests/` (exceto
  `tests/fixtures/import/`), `tools/`.

### 6. Verificação

Propor um lint de CI que falhe se os identificadores proibidos aparecerem nos
diretórios de produto. A regra primária é simples: **o token `wokwi`
(case-insensitive), o literal `diagram.json` e o prefixo `board-esp32-` são
proibidos** fora da allowlist. Isso cobre todas as variantes (`Wokwi-GUEST`,
`@wokwi:`, `wokwi-led`, `host.wokwi.internal`, `wokwi.toml`, …) sem enumerar o
catálogo inteiro de partes e placas.

Lista de referência (documentação e fixtures de autoteste):

```text
Wokwi-GUEST
Wokwi IoT Gateway
$serialMonitor
wokwi-logic.vcd
wokwi.vcd
wokwi.pcap
host.wokwi.internal
@wokwi:
diagram.json
wokwi-led
board-esp32-c3-devkitm-1
wokwi.toml
24:0a:c4:
42:13:37:55:aa:01
```

Regras do lint:

- **case-insensitive** para `wokwi` e para o prefixo `board-esp32-`;
- `$` e `.` escapados na regex quando a lista literal for usada;
- MACs e IPs **não** entram no lint (não são marca); as regras de §4 são
  verificadas em revisão de código;
- allowlist: `evidence/`, `docs/`, `apps/import/` e `tests/fixtures/import/`;
- **autoteste:** o lint roda contra um fixture com `Wokwi-GUEST` em `core/`
  (deve falhar) e contra `docs/adr/0006` (deve passar, pois `docs/` está fora do
  escopo).

O lint é uma tarefa de Fase 1; até existir, a revisão de código cobre a regra.

## Motivações

- Evitar confusão de marca e sugerir afiliação indevida.
- Evitar que a compatibilidade de configuração vire cópia de identidade.
- Separar interoperabilidade (adaptador) de contrato interno (schema próprio).
- Evitar defaults que carreguem OUI ou valores específicos de terceiro.

## Consequências

### Positivas

- Identidade própria e sem ambiguidade legal.
- Schema interno livre para evoluir sem herdar convenções do terceiro.
- Importação, se vier, fica isolada e testável.

### Negativas

- Usuários que esperam os mesmos nomes verão diferenças; exige documentação.
- O adaptador de importação, se existir, tem custo de manutenção.
- Um lint adicional precisa ser mantido.

### Mitigações

- Publicar a tabela de mapeamento quando houver importação.
- Explicar as diferenças de nome na documentação de migração.
- Manter a lista de identificadores proibidos em um único arquivo de configuração
  do lint.

## Critérios de revisão

- Decisão de suportar importação de projetos do Wokwi → revisar §3.
- Revisão jurídica de marcas antes de lançamento público → revisar §1.
- Mudança na política de marcas de `08` §5 → revisar esta ADR.
- Se um identificador deixar de ser marca ou se tornar genérico → reavaliar a
  lista de §6 com parecer clean-room.
