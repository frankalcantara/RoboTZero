# Especificação comportamental — `diagram.json` v1 para blink ESP32-C3

- **Versão:** 0.3 (correções da reauditoria)
- **Data:** 2026-10-25
- **Fonte:** [`README.md`](README.md), evidências EV-001..EV-013
- **Status do alvo:** `blocked` para observação; contrato obtido de documentação MIT
- **Revisão clean-room:** [`audit-spec.md`](audit-spec.md) (v0.1) e
  [`audit-spec-v0.2.md`](audit-spec-v0.2.md) (reauditoria)

> Esta especificação descreve um **contrato de arquivo** para importação e
> exportação. Ela **não** descreve a emulação, o timing, o boot ou o runtime do
> alvo, e **não** afirma que o alvo aceita este contrato em execução — isso é
> `unknown` (UN-001).
>
> EV-013 (matriz de features do C3) é **contextual e não normativo**: não é
> usado por nenhum requisito desta spec.

## 1. Escopo

**Dentro:**

- ler e escrever `diagram.json` versão 1;
- representar placa ESP32-C3, LED e conexões;
- representar a configuração documentada do monitor serial;
- validar, preservar e reportar erros de forma explícita;
- limitar tamanho, profundidade e cardinalidade da entrada.

**Fora:**

- CPU, GPIO, UART, timers, boot, flash, FreeRTOS;
- compilação de firmware e formato de imagem;
- UI, câmera, editor visual e assets 3D;
- outros chips e outros componentes;
- comportamento não documentado do alvo;
- nomes de pino da placa C3 (UN-009).

## 2. Terminologia

| Termo | Significado |
| --- | --- |
| **Documento** | Um arquivo `diagram.json` versão 1. |
| **Parte** | Item do array `parts`. |
| **Conexão** | Item do array `connections`. |
| **Pino** | Nome de pino de uma parte (ex.: `A`, `C`). |
| **Pseudo-parte** | Identificador reservado com prefixo `$` (ex.: `$serialMonitor`), que não aparece em `parts`. |
| **Adaptador** | Camada do studio que converte documento ↔ modelo interno. |
| **Modelo interno** | Representação do projeto usada pelo ESP32Sim, fora do core. |

## 3. Contrato documentado (C)

Cada requisito abaixo é uma afirmação sobre o **formato documentado**, não
sobre o runtime do alvo.

| ID | Requisito | Fonte | Nível |
| --- | --- | --- | --- |
| **C-01** | O documento é um objeto JSON com `version` igual a `1`, `parts` como array e `connections` como array. `author`, `editor` e `serialMonitor` aparecem na estrutura básica; a obrigatoriedade deles não é declarada (UN-002). | EV-002 | observado |
| **C-02** | Cada parte tem `id` (string, único) e `type` (string) obrigatórios. `left`, `top`, `attrs`, `rotate` e `hide` são opcionais. | EV-003 | observado |
| **C-03** | `id` duplicado é inválido; a documentação alerta que a simulação "pode não funcionar corretamente" (tradução do original em inglês). | EV-003 | observado |
| **C-04** | A placa ESP32-C3 é identificada pelo tipo `board-esp32-c3-devkitm-1`. | EV-004 | observado |
| **C-05** | O LED é identificado pelo tipo `wokwi-led`, com pinos `A` (anodo) e `C` (catodo). O atributo `color` tem default `"red"`; `lightColor`, `label`, `gamma`, `flip` e `fps` são opcionais. | EV-005 | observado |
| **C-06** | Cada conexão é um array de quatro posições: origem `id:pino`, destino `id:pino`, cor (string; vazia oculta o fio) e lista opcional de instruções de fio. | EV-006 | observado |
| **C-07** | As instruções de fio usam `vN` (vertical) e `hN` (horizontal), com no máximo um `*`. As instruções **antes** de `*` seguem a ordem escrita; **apenas** as instruções **depois** de `*` são aplicadas em ordem reversa. O restante é completado automaticamente. | EV-007 | observado |
| **C-08** | Para Arduino Uno/Mega, o monitor serial anexa automaticamente à UART de hardware e detecta o baud rate; por padrão, só aparece quando há saída (`display: "auto"`). Para o C3, o auto-attach e o auto-baud não estão declarados nas páginas coletadas (UN-007). | EV-008, EV-009 | observado com qualificação |
| **C-09** | O bloco `serialMonitor` tem defaults `collapse=false`, `convertEol=false`, `display="auto"` e `newline="lf"`. | EV-009 | observado |
| **C-10** | `display` aceita `auto`, `always`, `never`, `plotter` e `terminal`. `newline` aceita `lf`, `cr`, `crlf` e `none`. `collapse` e `convertEol` são booleanos. | EV-009, EV-010 | observado |
| **C-11** | O monitor serial pode ser roteado explicitamente por conexões `$serialMonitor:TX` e `$serialMonitor:RX`, ligadas aos pinos RX/TX da UART. `$serialMonitor` é tratado como identificador reservado (pseudo-parte), não como parte de `parts`. | EV-011 | derivado |
| **C-12** | Para USB CDC, o chip aceita `serialInterface: "USB_SERIAL_JTAG"`; nesse caso, as conexões com `$serialMonitor` devem ser removidas. | EV-012 | observado |

## 4. Requisitos do ESP32Sim (R)

Estes requisitos são **decisões de engenharia** derivadas do contrato. Eles não
são fatos sobre o alvo.

| ID | Requisito | Rastreia | Justificativa |
| --- | --- | --- | --- |
| **R-01** | O adaptador deve aceitar um documento bem-formado e produzir o modelo interno do projeto; JSON inválido deve retornar erro estável, sem exceção não tratada. | C-01, C-02 | robustez de fronteira |
| **R-02** | `id` duplicado deve ser rejeitado com erro explícito, não "corrigido" em silêncio. | C-03 | evita ambiguidade de conexões |
| **R-03** | Campos de parte não interpretados devem ser preservados para round-trip. | C-02 | interoperabilidade sem perda |
| **R-04** | Chaves de topo não interpretadas devem ser preservadas para round-trip. | C-01 | evita perda de metadados |
| **R-05** | `type` de parte desconhecido deve produzir resultado explícito `unsupported_part` (erro ou quarentena), nunca descarte silencioso. | C-02 | segurança e diagnóstico |
| **R-06** | O adaptador deve mapear `board-esp32-c3-devkitm-1` para um ID interno de placa por tabela versionada. | C-04 | desacoplar formato externo do modelo interno |
| **R-07** | O adaptador deve mapear `wokwi-led` para o device interno de LED, preservando `A`, `C`, `color` (default `"red"`) e demais atributos. | C-05 | interoperabilidade |
| **R-08** | Conexões devem ser resolvidas por `id:pino`. Parte ou pino inexistente deve gerar erro explícito, **exceto** pseudo-partes reservadas `$serialMonitor:TX`/`RX`, que são resolvidas pelo adaptador e não precisam existir em `parts`. `$` é proibido em `parts[].id` (`reserved_id`). Cor vazia deve ocultar o fio; instruções de fio devem ser preservadas. | C-06, C-07, C-11 | integridade do diagrama |
| **R-09** | O monitor serial deve ser uma projeção de eventos do core; a configuração `serialMonitor` deve afetar apenas a apresentação, nunca o timing ou o estado do core. | C-08..C-12 | arquitetura §7, §10 |
| **R-10** | A exportação deve ser determinística: emitir `version: 1`, preservar a ordem de `parts`/`connections` da importação (itens novos são anexados), **reemitir conexões com pseudo-partes `$serialMonitor:*`**, emitir chaves de topo em ordem fixa (`version`, `author`, `editor`, `parts`, `connections`, `serialMonitor`, depois chaves preservadas em ordem lexicográfica) e preservar os campos de R-03/R-04. | C-01..C-11 | reprodutibilidade |
| **R-11** | O adaptador deve viver na fronteira do studio (ou pacote compartilhado fora do core); a ABI do core não muda por causa deste formato. | C-01..C-12 | arquitetura §5, §10 |
| **R-12** | Modelos 3D de placa e LED não podem vir do alvo; precisam de licença própria e registro em `provenance/`. | — | docs/08 §1.2 |
| **R-13** | A suíte de conformidade deve ter fixtures autorais para os casos de §6, sem copiar projetos da comunidade. | C-01..C-12 | docs/06 |
| **R-14** | O bloco `serialMonitor` deve ser preservado no round-trip e validado contra os enums de C-10; valor desconhecido gera `invalid_serial_monitor`. | C-09, C-10 | evita perda silenciosa de configuração |
| **R-15** | O adaptador deve impor limites de entrada: tamanho máximo de 1 MiB, 256 partes, 1.024 conexões, profundidade máxima de 32 níveis e strings de até 4 KiB. Excesso gera `limit_exceeded`. | C-01, C-02 | entrada de usuário vai para browser/WASM |
| **R-16** | Strings de `label` e `attrs` devem ser tratadas como texto na UI; nunca interpretadas como HTML. | C-02, C-05 | XSS |
| **R-17** | Chaves `__proto__`, `constructor` e `prototype` devem ser rejeitadas (`unsafe_key`); chaves duplicadas no JSON devem ser rejeitadas (`duplicate_key`). O parser não pode usar último-valor-vence. | C-01 | prototype pollution / ambiguidade |
| **R-18** | `version` deve ser o número `1`; a string `"1"` é erro (`invalid_version_type`). | C-01 | tipagem estrita |
| **R-19** | `parts[].id` ausente, `type` ausente ou tipo errado gera `missing_part_field`; `connections` fora do formato gera `invalid_connection`. | C-02, C-06 | validação explícita |

## 5. Comportamento esperado do adaptador

### 5.1 Importação

1. Ler o documento como JSON, com parser que rejeita chaves duplicadas.
2. Validar limites de R-15.
3. Validar `version` como número `1` (`unsupported_version` ou
   `invalid_version_type`).
4. Validar `parts` e `connections` como arrays.
5. Validar `id` único, `type` presente e `$` proibido em `parts[].id`
   (`reserved_id`).
6. Validar `serialMonitor` contra C-09/C-10.
7. Resolver tipos conhecidos pela tabela de mapeamento.
8. Resolver conexões; erro se parte/pino não existir, exceto pseudo-partes
   `$serialMonitor:TX`/`RX` (pseudo-parte desconhecida gera
   `unknown_pseudo_part`).
9. Preservar campos não interpretados.
10. Entregar ao modelo interno uma representação sem dependência de JSON.

### 5.2 Exportação

1. Partir do modelo interno.
2. Emitir `version: 1`.
3. Emitir partes e conexões na ordem definida em R-10, incluindo as conexões
   com `$serialMonitor:*`.
4. Reemitir `serialMonitor` e campos preservados.
5. Não emitir campos que o modelo interno não conhece e que não foram
   preservados na importação (evitar invenção).

### 5.3 Erros

Códigos estáveis mínimos:

| Código | Condição |
| --- | --- |
| `invalid_json` | JSON malformado |
| `duplicate_key` | chave duplicada no objeto |
| `unsafe_key` | `__proto__`, `constructor` ou `prototype` |
| `unsupported_version` | `version` diferente de 1 |
| `invalid_version_type` | `version` não é número |
| `limit_exceeded` | tamanho/profundidade/cardinalidade acima de R-15 |
| `duplicate_part_id` | `id` repetido |
| `missing_part_field` | `id`/`type` ausente ou com tipo errado |
| `reserved_id` | `$` em `parts[].id` |
| `unsupported_part` | `type` desconhecido |
| `unknown_pin` | pino inexistente na parte |
| `unknown_pseudo_part` | pseudo-parte `$...` não reconhecida |
| `invalid_connection` | conexão fora do formato de 4 posições |
| `invalid_serial_monitor` | `display`/`newline`/booleanos inválidos |

## 6. Fixtures de conformidade (a criar na implementação)

As fixtures devem ser **autorais**, construídas a partir desta spec e de
nomes públicos, sem copiar projetos da comunidade. Conteúdo mínimo:

| Fixture | Cobre | Observação |
| --- | --- | --- |
| `blink-c3-minimal` | C-01..C-06, R-01..R-08 | **bloqueada** até resolver UN-009 (nomes de pino da placa) |
| `blink-c3-serial-explicit` | C-11, R-08 | **bloqueada** até resolver UN-009 (pinos RX/TX da placa) |
| `blink-c3-usb-cdc` | C-12 | usa `serialInterface`; verificar UN-009 antes de criar |
| `serial-monitor-roundtrip` | C-09, C-10, R-14 | preserva `serialMonitor` |
| `unsupported-part` | R-05 | parte desconhecida |
| `duplicate-id` | R-02 | `id` repetido |
| `missing-field` | R-19 | `id`/`type` ausente |
| `malformed-json` | R-01 | JSON inválido |
| `duplicate-key` | R-17 | chave duplicada |
| `unsafe-key` | R-17 | `__proto__` |
| `reserved-id` | R-08 | `$` em `parts[].id` |
| `unknown-pseudo-part` | R-08 | `$foo` |
| `unknown-pin` | R-08 | pino inexistente |
| `invalid-version-type` | R-18 | `"1"` |
| `invalid-serial-monitor` | R-14 | enum inválido |
| `limit-exceeded` | R-15 | cardinalidade/tamanho |
| `xss-label` | R-16 | `label` com HTML |
| `round-trip-preserve` | R-03, R-04, R-10 | campos preservados |

Nenhuma fixture deve conter asset, código ou diagrama copiado do alvo.

## 7. O que esta spec não afirma

| ID | Item | Status |
| --- | --- | --- |
| UN-001 | O runtime do alvo aceita exatamente este contrato | desconhecido |
| UN-002 | `author`/`editor` são obrigatórios em runtime | desconhecido |
| UN-003 | `left`/`top` são obrigatórios na prática | desconhecido |
| UN-004 | Nome/estrutura dos arquivos de código | desconhecido |
| UN-005 | Comportamento elétrico/timing de GPIO e LED | desconhecido |
| UN-006 | Mensagens de erro e validação do editor | desconhecido |
| UN-007 | C3 usa USB CDC por padrão; auto-attach/auto-baud no C3 | desconhecido |
| UN-008 | Estabilidade futura do formato | desconhecido |
| UN-009 | Nomes de pino da placa `board-esp32-c3-devkitm-1` | desconhecido |

Nenhum item desta tabela pode ser convertido em requisito sem nova evidência.

## 8. Critérios de aceite desta spec

- [ ] Todo C tem fonte EV identificável.
- [ ] Todo R tem rastreabilidade e justificativa.
- [ ] Nenhum `unknown` foi promovido a requisito.
- [ ] Nenhum código, asset ou dado proprietário foi incluído.
- [ ] Revisão clean-room registrada em [`audit-spec-v0.2.md`](audit-spec-v0.2.md).
- [ ] A implementação futura não lê capturas do alvo (não existem).
- [ ] Fixtures dependentes de pino (`blink-c3-minimal`,
      `blink-c3-serial-explicit`) só são criadas depois de resolver UN-009 com
      fonte licenciada.
