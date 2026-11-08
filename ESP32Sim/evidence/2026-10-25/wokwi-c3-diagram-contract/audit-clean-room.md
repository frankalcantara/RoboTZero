# Parecer clean-room — gate do alvo Wokwi

- **Agente:** `clean-room-auditor` (subagente)
- **Sessão:** `ses_e8e8b2d50ffe3akKFu8KT7iz4r`
- **Data:** 2026-10-25
- **Modo:** somente leitura; nenhum arquivo criado, editado ou apagado
- **Objeto:** observação via REA/CDP de `https://wokwi.com/projects/new/esp32-c3`
- **Classificação:** **`blocked`**

> O lead verificou diretamente o ToS (`/legal/terms`) e a licença MIT do
> repositório `wokwi/wokwi-docs`. As demais verificações citadas abaixo são do
> auditor. Este é um parecer de engenharia, não jurídico.

---

# Parecer do `clean-room-auditor` — observação via REA/CDP de `wokwi.com/projects/new/esp32-c3`

> Avaliação de **engenharia**, não parecer jurídico. Decisão de distribuição exige revisão profissional. Nada foi coletado do alvo; apenas li páginas públicas de termos/documentação para verificar as citações do lead. Nenhum arquivo foi criado, editado ou apagado.

```text
Veredito: blocked
Escopo revisado:
  - Atividade proposta (não executada): observação passiva via REA/CDP de
    https://wokwi.com/projects/new/esp32-c3 (estrutura de projeto, fluxo de
    compilação/simulação, saída serial de blink), com artefatos redigidos em
    ESP32Sim/evidence/
  - ToS: https://wokwi.com/legal/terms (verificado por fetch em 2026-10-25;
    "Last updated: October 1st, 2026", "Copyright © 2019-2026 Wokwi B.V.")
  - https://wokwi.com/legal/licenses (fetch 2026-10-25) e
    https://wokwi.com/robots.txt (404)
  - https://docs.wokwi.com/ (fetch 2026-10-25; rodapé: CodeMagic LTD, build
    commit 5c90e53) e origem do conteúdo: github.com/wokwi/wokwi-docs (MIT,
    verificado via GitHub API)
  - docs/00, 01 (§5, §9), 03, 05 (§5-6), 08 (§1, §4, §4.2, §5, §7-8),
    evidence/README.md; estado do repo (apenas docs/ e evidence/README.md)
```

## 1. Classificação final: `blocked`

Quatro cláusulas do ToS, **cumulativas**, fecham o caso. Verifiquei as citações do lead e elas conferem **literalmente** (inclusive o erro de digitação "expressively" no original):

| # | Cláusula (verbatim, trecho) | Efeito sobre a atividade |
| --- | --- | --- |
| C1 | Licença de uso: "*…to use the Service … for your personal and non-commercial purposes only*" (também no resumo: "*You may use the Service for your personal non-commercial purposes only*") | O ESP32Sim é produto distribuído (docs/08 §3 prevê opção comercial). Não é uso pessoal não comercial. |
| C2 | Acceptable use: "*Using or launching any automated system, including robots, crawlers and similar applications to collect or compile content from the Service…*" | O REA **é** um sistema automatizado cuja finalidade declarada (docs/01 §4.1, §6.3) é coletar/compilar conteúdo do Service. |
| C3 | "*You may not access or use the Service in order to develop or create a similar or competitive product or service, other than as expressively permitted in these Terms.*" | Proibição **por finalidade**. Esp32Sim é, por definição do próprio projeto, produto similar/concorrente. Não há permissão expressa nos Termos. |
| C4 | IP: "*You may not … decompile, disassemble, reduce to human readable form, … process, compile, … reverse engineer, combine with other software, modify or create derivative works of the Service's software … without prior written authorization from us.*" | Captura de DOM/rede/WS/WASM, análise de bundle e exportação de scripts = condutas listadas, **sem autorização escrita registrada**. |

Isso aciona diretamente duas regras internas já escritas:
- `evidence/README.md` §1: "*Só colete evidência de alvos que você está autorizado a examinar*" + §2 ("leia ToS antes; registre a conclusão").
- `docs/08` §4.2: "*Termos proíbem observação → bloquear `rea-evidence` no alvo*" (R-01) e `docs/01` §5.2 ("se houver cláusula proibitiva, **não use o REA** no alvo").

Não há no repositório: autorização escrita, contrato, ADR jurídico, parecer profissional, conta/perfil dedicado documentado (`ESP32Sim` contém apenas `docs/` e `evidence/README.md`; `evidence/` só tem o README). Portanto **não existe base registrada** para desbloquear.

**Agravantes específicos desta atividade** (não citados pelo lead):
- **`/projects/new/esp32-c3` é fluxo de criação.** Ao criar/rodar o blink, o conteúdo vira **User Content** e, pela cláusula de User Content, você concede à Wokwi licença "*worldwide, royalty-free, … perpetual and irrevocable … to copy, reproduce, create derivative works of, distribute, display, sublicense*". Ou seja: até o firmware de teste que escrevermos no alvo fica licenciado a eles e pode tornar-se público.
- **Cláusula de feedback:** "*you assign all right, title and interest in and to that feedback to us*". Se qualquer achado for enviado como bug report/sugestão à Wokwi, os direitos sobre o achado são **cedidos**. Política: nenhum contato de feedback durante/após a coleta.
- **Cláusula de telemetria:** "*you agree for us to analyse the Service and Extension you use, your User Content … and to collect telemetry data*" — a coleta gera telemetria vinculada à conta usada.
- **Lei/jurisdição:** Holanda, foro exclusivo de Amsterdã. Remédios disponíveis incluem suspensão/terminação e "*technical and legal measures*".

## 2. Pode ocorrer sem autorização escrita? **Não.**

O conflito é insolúvel por três caminhos que costumam ser tentados:

1. **"É só uma conta gratuita / sessão anônima."** Irrelevante. C1 limita a licença a uso *pessoal e não comercial*; C3 proíbe a **finalidade**, não o acesso pago. Uma sessão anônima não cria permissão.
2. **"O produto será gratuito/MIT, então não é comercial."** C3 não fala de monetização: veda "*similar or competitive product or service*". Um simulador de ESP32 gratuito é, ainda assim, similar/concorrente. C1 já cai por "personal"; C3 cai independentemente de preço.
3. **"O observador age como pessoa física, não pela empresa."** C2/C3 e a cláusula de acceptable use ("*all acts or omissions … and the access and use of the Service by anyone on your behalf*") alcançam quem age **em nome** de outrem. O parecer é emitido para o projeto, portanto a finalidade é do projeto.

Ponto que o lead deve registrar: **`personal non-commercial use` e "construir produto similar" são incompatíveis por desenho**, não por interpretação estrita. A observação só se torna lícita com **permissão expressa por escrito que derrogue C3** (C3 admite exceção apenas "*as expressively permitted in these Terms*", ou seja, é preciso instrumento próprio — não basta silêncio, não basta licença comercial de assinatura).

Consequência de engenharia, mais importante que o risco de conta: **a evidência seria contaminada de forma irreparável**. Redaction remove artefatos, não remove conhecimento. Um implementador que lê uma spec derivada de captura do Service está implementando a partir do Service — o que C3 veda e o que o clean-room de `docs/00` §4.4 e `docs/08` §1.1 existe para impedir. Não existe "spec suficientemente abstrata" que cure isso retroativamente.

## 3. Leitura de documentação pública: **admissível por uma rota, `needs-decision`/`unknown` pelas outras**

Distinção necessária, porque "Service" no ToS é definido restritivamente como o simulador web "*which is also available as an extension*" (VS Code):

| Superfície | Situação | Ação |
| --- | --- | --- |
| **github.com/wokwi/wokwi-docs** (fonte do docs.wokwi.com; rodapé "Edit this page" aponta para lá) | **`clean`** — MIT, verificado hoje via GitHub API (`spdx_id: MIT`, push 2026-10-23). MIT concede uso "*without restriction*", o que prevalece para esse material. | **Rota preferencial.** Ler via GitHub/raw, não via site. Citar `commit sha` (build atual: `5c90e53`) para pin. Obrigação: incluir copyright + permission notice se copiarmos porções substanciais de texto. |
| **docs.wokwi.com** (renderizado) | **`unknown`/`needs-decision`** — conteúdo é MIT, mas a entrega é feita por infraestrutura da Wokwi/CodeMagic; leitura ampla de C3 poderia alcançar "access … in order to develop". | Evitável: mesmo conteúdo, sem a dúvida. Se usar, ler sob a égide do MIT e registrar a proveniência do commit. |
| **blog.wokwi.com, posts e páginas institucionais** | **`needs-decision`** — não localizei licença explícita (`unknown`). Fatos são livres; o texto é protegido. | Citar fatos com URL+data; **não** reproduzir trechos, imagens ou diagramas. |
| **wokwi.com/legal/licenses** | **`clean` como leitura**, mas **não é permissão de nada** — lista apenas licenças de terceiros embarcadas na extensão VS Code (@iarna/toml ISC, cross-fetch MIT, node-fetch MIT, tr46 MIT, uf2 MIT, webidl-conversions BSD-2, whatwg-url MIT, ws MIT). | **Não citar como base legal.** Não há licença aberta do simulador, do WASM ou dos assets nessa página. |
| **`/robots.txt`** | **404** → `unknown`. Ausência de robots.txt não autoriza nada; o ToS prevalece. | Registrar como `unknown`; não inferir permissão. |

**Armadilha a bloquear explicitamente:** projetos públicos da comunidade em `wokwi.com/projects/...` (inclusive os exemplos linkados nos docs) **não** são fonte utilizável como evidência — são User Content sob os Termos, sem licença MIT conferida, e acessá-los para desenvolver concorrente é o próprio C3. A lista de exemplos no docs.wokwi.com pode ser lida como *índice*, nunca como *fonte de arquivos*.

## 4. Conteúdo mínimo de uma autorização escrita que desbloqueie a coleta

Instrumento próprio, assinado por representante com poderes da Wokwi B.V., cobrindo no mínimo:

1. **Partes e alcance subjetivo:** o contratante, seus empregados, prestadores e **agentes automatizados/ferramentas de IA** usados na coleta (nomear que o REA/CDP é ferramenta).
2. **Derrogação expressa e nominal** de C3 ("*similar or competitive product*") e de C4 (reverse engineering/derivative works), C2 (automated systems) e C1 (restrição a uso pessoal não comercial) — para a **finalidade declarada**: projetar e distribuir um simulador de ESP32, inclusive comercialmente.
3. **Objeto técnico delimitado:** observar/capturar `/projects/new/esp32-c3` e projetos criados por nós; DOM, rede, WebSocket, console, storage keys, bundle estático, artefatos WASM; **janela de tempo e rate limit**; autorização explícita para conexão CDP.
4. **Escopo dos artefatos permitidos:** screenshots, traces, hashes, estruturas de dados observadas; limites de retenção e proibição de redistribuir conteúdo capturado.
5. **Titularidade do resultado:** specs e código derivados pertencem ao contratante; nenhuma licença reversa; nenhuma cessão de feedback (derrogar a cláusula de feedback para os nossos achados).
6. **User Content:** que firmware de teste que criarmos no alvo **não** seja licenciado à Wokwi de forma perpétua/irrevocável, nem publicado.
7. **Marcas:** regras de uso de "Wokwi"/"ESP32" nos nossos materiais e no texto de compatibilidade; autorização (ou proibição) de mencionar interoperabilidade.
8. **Confidencialidade e publicação:** se a evidência pode ou não ser publicada; se há NDA sobre achados.
9. **Vigência, revogação e wind-down:** prazo, aviso prévio, obrigação de destruir capturas na revogação, sobrevivência do direito de usar specs já escritas.
10. **Foro/lei aplicável** e contato formal (o endereço de e-mail aparece ofuscado no HTML que li — obter o canal oficial na página viva, sem inventar).
11. **Declaração de que a autorização não constitui endosso** e não cria afiliação.

Sem os itens 2, 5 e 6, a autorização **não desbloqueia** — apenas troca "violação de ToS" por "violação de contrato".

## 5. Alternativas lícitas concretas (priorizadas)

O achado mais útil deste parecer: **quase tudo que o lead quer da captura já existe em fonte licenciada ou em hardware próprio.**

| Prioridade | Fonte | Responde o quê | Situação |
| --- | --- | --- | --- |
| 1 | **docs.wokwi.com via repo MIT `wokwi/wokwi-docs`** (pin `5c90e53`) — inclui `/diagram-format` e `Supported Hardware` | **Estrutura do projeto**: `diagram.json` (board, parts, connections), lista de arquivos, nomes de placas/partes — exatamente o item 1 da coleta proposta | `clean` (MIT; aviso se copiar texto) |
| 2 | **Hardware real** (ESP32-C3-DevKitM-1) + firmware próprio + analisador lógico/UART | **Saída serial observável de blink** e **goldens próprios** — substitui o item 3 da coleta com autoridade maior | `clean`; vira `provenance/` |
| 3 | **TRM/datasheet Espressif ESP32-C3** (citar versão e seção) | CPU RISC-V, mapeamento de memória, GPIO/UART/timers, boot | Copyright Espressif: **usar, não redistribuir PDF** |
| 4 | **ESP-IDF (Apache-2.0)** e **arduino-esp32 (LGPL-2.1 — confirmar na versão pinada)** | Fluxo de compilação/partições/boot, firmware de teste, mensagens de log do próprio firmware | `needs-attribution`/`needs-decision` (LGPL: revisar linkagem antes de distribuir) |
| 5 | **QEMU (GPLv2)** e fork lcgamboa (verificar termos do fork) | Oráculo diferencial de CPU/periféricos em **CI**, nunca no WASM distribuído | `needs-decision` (R-03) |
| 6 | **avr8js (MIT)** | Referência de design de emulação (AVR, não ESP32) | `clean` com aviso |
| 7 | **wokwi-elements (MIT)** | Referência *visual* de componentes | `needs-attribution` + verificação **por asset** (artes de placas podem embutir marcas de terceiros); **não** serve como modelo 3D |
| 8 | **Normas ISA RISC-V e headers de ROM API** | ISA, ABI, exceções | Verificar licença de cada documento antes de citar |
| 9 | **REA retargetado para nossos próprios artefatos** (nosso bundle, nosso WASM, nosso dev server) | Auditoria de supply chain e de superfície de imports/exports | `clean` — mantém o valor da ferramenta (docs/01 §4.3) sem tocar no alvo |

Nota de escopo: a pergunta "como o Wokwi serializa o projeto" **não exige observação** — o formato está documentado em repositório MIT. A pergunta "como o serial se comporta em blink" **não exige o alvo** — hardware real ou QEMU respondem com mais fidelidade. Sobra como estritamente dependente do alvo apenas *detalhes de implementação do concorrente*, que são precisamente o que C3/C4 vedam e que o projeto declarou não querer copiar (`docs/00` §7: "*não ser compatível byte a byte … interoperabilidade documentada, não clone*").

## 6. Obrigações e riscos residuais; o que NÃO pode entrar no repositório

**Não pode entrar (bloqueio duro):** HTML/CSS/DOM dumps do alvo; bundles JS, source maps, scripts exportados; artefatos WASM do alvo; screenshots da UI do alvo; SVGs/modelos/texturas de partes obtidos do alvo; `diagram.json` ou projetos copiados de `wokwi.com/projects/*`; logs de serial capturados do Service; catálogo interno de placas/partes, endpoints ou nomes internos de API; credenciais, cookies, headers `Authorization`, IDs de conta, telemetria; PDFs de TRM/datasheet Espressif; firmware de usuário sem consentimento (docs/08 §6).

**Riscos residuais que permanecem mesmo sem coleta:**
- **Marca:** "Wokwi" fora de nome/logo/domínio; nada "confusingly similar" (cláusula de marcas); disclaimer "Projeto independente, não afiliado à Espressif ou à Wokwi" (docs/08 §5). "ESP32" apenas descritivo — confirmar *trademark guidelines* da Espressif antes do release.
- **Feedback:** nenhum bug report/sugestão à Wokwi (cessão automática).
- **User Content:** nenhum firmware nosso criado dentro do alvo.
- **Termos mutáveis:** "*We may amend the Terms … continued use … constitutes your consent*". Qualquer conclusão vale para a versão lida (2026-10-01); **re-verificar a cada release** e registrar data + hash do texto.
- **Foro holandês** e remédios unilaterais (suspensão, "*technical and legal measures*").
- **Inconsistência documental já existente:** `docs/08` §2 descreve `wokwi-elements` como "Referência visual" sem exigir proveniência por asset; `docs/01` §9 autoriza o REA "condicionalmente" sem que a condição 2 (revisão de ToS) esteja satisfeita — **estado atual é não autorizado**. Exige ADR/nota do lead atualizando 01 §5/§9 e 08 R-01 com esta conclusão.
- **Itens de release não atendidos** (docs/08 §7-8): sem `provenance/`, sem `THIRD_PARTY_NOTICES`, sem SBOM no repositório; e a decisão de licenciamento do produto (§3) segue "pendente".
- **Transparência:** para verificar as citações, acessei `wokwi.com/legal/terms`, `/legal/licenses`, `/robots.txt` e `docs.wokwi.com`. Sob leitura maximalista de C3 isso também é acesso ao Service; risco prático baixo (páginas legais/públicas, finalidade de conformidade), mas fica registrado como `unknown` para não criar precedente de "acesso incidental autorizado".

## 7. Checklist objetivo de desbloqueio

- [ ] **A1.** Autorização escrita da Wokwi B.V. assinada, cobrindo os itens 1-11 do §4 — **com derrogação nominal de C3 e C4**.
- [ ] **A2.** Cópia da autorização arquivada fora do repo público (referência/hash em `docs/`), com identificação do signatário e poderes.
- [ ] **A3.** Parecer de profissional jurídico habilitado sobre a autorização **e** sobre a distribuição do produto; registro em ADR (`docs/adr/0005-...`). **Errata 2026-10-25:** o slot 0005 foi ocupado por `ADR 0005 — Escopo de paridade do MVP`; o registro jurídico deve usar o próximo número livre (0007+).
- [ ] **A4.** ToS re-verificado na data do desbloqueio, com versão ("Last updated") e hash do texto registrados.
- [ ] **A5.** Conta/perfil dedicado e isolado documentado (sem dados pessoais, sem credenciais de terceiros) + plano de redaction e retenção.
- [ ] **A6.** Escopo da coleta escrito e aprovado **antes** da execução: URL, janela, rate limit, artefatos permitidos, proibição de contato/feedback e de upload de firmware nosso.
- [ ] **A7.** Ficha de evidência pré-preenchida com "Base legal/termos" e "Limitações".
- [ ] **A8.** Separação confirmada: `rea-evidence` escreve spec de comportamento; implementadores **não** leem capturas brutas; `clean-room-auditor` revisa a spec antes de qualquer código.
- [ ] **A9.** `docs/01` §5/§9 e `docs/08` R-01 atualizados com a decisão e a data.
- [ ] **A10.** Nenhum artefato da lista de bloqueio do §6 no diff; `THIRD_PARTY_NOTICES` + `provenance/` criados quando entrarem dependências/assets.

**Enquanto A1-A3 não existirem, a atividade permanece `blocked`** — e o caminho 1 da §5 (wokwi-docs MIT) entrega o item de maior valor da coleta sem depender de nada disso. Recomendo que o lead trate o REA como **ferramenta interna** (nosso bundle/WASM) e substitua a coleta no alvo por: docs MIT + hardware real + QEMU em CI.
