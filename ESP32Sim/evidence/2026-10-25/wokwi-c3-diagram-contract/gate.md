# Gate legal e clean-room — alvo `wokwi.com/projects/new/esp32-c3`

- **Data:** 2026-10-25
- **Responsável:** `esp32-lead`
- **Parecer:** [`audit-clean-room.md`](audit-clean-room.md)
- **Classificação:** **`blocked`** para observação do simulador
- **Rota alternativa:** **`clean`** para a documentação MIT `wokwi/wokwi-docs`

> Avaliação de engenharia, não parecer jurídico. Decisão de distribuição exige
> revisão profissional.

## 1. Termos aplicáveis

| Item | Valor |
| --- | --- |
| Documento | Wokwi Terms of Service |
| URL | `https://wokwi.com/legal/terms` |
| Versão | "Last updated: October 1st, 2026" |
| Leitura | 2026-10-25 |
| Foro | Holanda; tribunais de Amsterdã |
| Autorização escrita da Wokwi | **não localizada** |
| Revisão jurídica profissional | **não realizada** |
| Conta/perfil dedicado documentado | **não localizado** |

## 2. Cláusulas relevantes

Citações curtas, para análise de conformidade:

| ID | Cláusula (trecho) | Efeito sobre a atividade |
| --- | --- | --- |
| C1 | "*…to use the Service … for your personal and non-commercial purposes only*" | O ESP32Sim é produto distribuído; não é uso pessoal não comercial. |
| C2 | "*Using or launching any automated system, including robots, crawlers and similar applications to collect or compile content from the Service…*" | O REA/CDP é sistema automatizado de coleta. |
| C3 | "*You may not access or use the Service in order to develop or create a similar or competitive product or service, other than as expressively permitted in these Terms.*" | Proibição por **finalidade**: o ESP32Sim é simulador concorrente. |
| C4 | "*You may not … reverse engineer … the Service's software … without prior written authorization from us.*" | Captura/análise de DOM, rede, WebSocket, bundle ou WASM sem autorização escrita. |
| C5 | "*you grant us a worldwide, royalty-free, … perpetual and irrevocable license to copy, reproduce, create derivative works of, distribute, display…*" (User Content) | Firmware de teste criado no alvo viraria User Content licenciado à Wokwi. |
| C6 | "*you assign all right, title and interest in and to that feedback to us*" | Qualquer feedback/bug report enviado à Wokwi é cedido. |

C1–C4 são **cumulativas**. Mesmo uma sessão anônima, um produto gratuito ou um
observador pessoa física não removem C3: a vedação é pela finalidade de
construir produto similar/concorrente, não pelo preço ou pelo tipo de conta.

## 3. Decisão

**`blocked`.** Não foi realizada nenhuma das seguintes ações no alvo:

- abrir `wokwi.com/projects/new/esp32-c3`;
- conectar CDP/REA;
- capturar DOM, acessibilidade, rede, WebSocket, console, storage ou scripts;
- tirar screenshot;
- baixar bundle, WASM ou asset;
- criar projeto, rodar firmware ou ler serial;
- enviar feedback, bug report ou sugestão.

A única leitura feita em domínio `wokwi.com` foi de páginas legais públicas
(`/legal/terms`), para verificar a base do gate. O parecer do auditor registra
essa leitura incidental como `unknown` sob leitura maximalista de C3, com risco
prático baixo e finalidade de conformidade; não há intenção de criar precedente
de "acesso incidental autorizado".

## 4. Rota alternativa aprovada

O parecer indicou que a documentação do projeto é publicada em repositório
**MIT**: `github.com/wokwi/wokwi-docs` (Copyright (c) 2021-2024 CodeMagic LTD).
Essa rota é `clean` e foi usada com commit pinado
`5c90e532d24fb8ac4cd471e9568db03917011e91`.

O que foi lido nessa rota:

- `LICENSE` (MIT);
- `docs/diagram-format.md`;
- `docs/guides/esp32.md`;
- `docs/parts/wokwi-led.md`;
- `docs/guides/serial-monitor.md`.

O que **não** foi lido/coletado: projetos públicos da comunidade
(`wokwi.com/projects/*`), exemplos linkados na documentação, imagens, GIFs,
SVGs, modelos 3D, bundles, WASM, endpoints ou qualquer artefato do simulador.

## 5. Limites que permanecem

- A rota MIT cobre **documentação**, não o comportamento do runtime. Tudo que
  depende de execução no alvo permanece `unknown`.
- Projetos públicos da comunidade são User Content sob os Termos, sem licença
  MIT conferida; **não** são fonte de evidência.
- `docs.wokwi.com` renderizado é `unknown`/`needs-decision`; a rota preferida é
  o repositório MIT (raw, commit pinado).
- `wokwi.com/legal/licenses` lista licenças de terceiros da extensão; **não**
  é licença do simulador e não autoriza nada.
- `robots.txt` retornou 404 (`unknown`); ausência de robots.txt não autoriza.
- O ToS pode mudar; a conclusão vale para a versão de 2026-10-01 e deve ser
  re-verificada a cada release.

## 6. Checklist de desbloqueio (se o alvo voltar ao escopo)

- [ ] A1. Autorização escrita da Wokwi B.V., assinada por representante com
      poderes, derrogando nominalmente C1–C4 para a finalidade de construir e
      distribuir um simulador de ESP32 (inclusive comercialmente).
- [ ] A2. Cópia da autorização arquivada fora do repositório público, com
      referência/hash em `docs/`.
- [ ] A3. Parecer de profissional jurídico habilitado sobre a autorização e
      sobre a distribuição do produto; registrar em ADR.
- [ ] A4. ToS re-verificado na data do desbloqueio, com versão e hash do texto.
- [ ] A5. Conta/perfil dedicado e isolado, sem dados pessoais ou credenciais de
      terceiros; plano de redaction e retenção.
- [ ] A6. Escopo da coleta aprovado antes da execução: URL, janela, rate limit,
      artefatos permitidos, proibição de feedback e de upload de firmware nosso.
- [ ] A7. Ficha de evidência pré-preenchida com base legal e limitações.
- [ ] A8. Separação clean-room confirmada: quem coleta não implementa; quem
      implementa não lê capturas brutas; auditor revisa a spec.
- [ ] A9. `docs/01-avaliacao-rea.md` §5/§9 e `docs/08-riscos-e-licencas.md`
      R-01 atualizados com esta decisão.
- [ ] A10. Nenhum artefato da lista de bloqueio no diff; `THIRD_PARTY_NOTICES`
      e `provenance/` criados quando entrarem dependências/assets.

Enquanto A1–A3 não existirem, a atividade permanece **`blocked`**.

## 7. Consequência para o plano

O caminho lícito para a Fase 0/1 é:

1. documentação MIT para o **formato de projeto**;
2. TRM/datasheet Espressif para **CPU, memória e periféricos**;
3. Arduino-ESP32/ESP-IDF (open source, com revisão de licença) para
   **firmware, boot e compilação**;
4. hardware real e traces próprios para **verdade de campo**;
5. QEMU apenas como oráculo em CI, com revisão de GPL.

O REA continua útil, mas retargetado para **nossos próprios artefatos**
(bundle, WASM, dev server), não para o alvo.

## 8. Estado das ações

| Ação | Estado |
| --- | --- |
| A1–A8 | pendentes (sem autorização escrita / sem revisão jurídica) |
| A9 (atualizar `docs/01` §5/§9 e `docs/08` R-01) | **pendente** — não alterado nesta etapa para respeitar o limite de escopo (somente evidência); registrado em [`handoff.md`](handoff.md) §9 |
| A10 | pendente |

Nenhuma implementação foi iniciada.
