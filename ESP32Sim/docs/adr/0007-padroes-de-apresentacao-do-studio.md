# ADR 0007 — Padrões de apresentação do studio

- **Status:** Aceito
- **Data:** 2026-10-25
- **Data de aceite:** 2026-10-27
- **Decisores:** lead, web3d, produto
- **Referências consultadas (sem reutilização):** `agharsallah/esp-labs-web` e
  `agharsallah/esp-labs` — ambos sem licença (todos os direitos reservados);
  ver [`08-riscos-e-licencas.md`](../08-riscos-e-licencas.md) §2
- **Relaciona-se com:** ADR 0001 (core C++23/WASM), ADR 0006 (identidade e
  defaults), [`03-arquitetura.md`](../03-arquitetura.md) §7,
  [`02-escopo-requisitos.md`](../02-escopo-requisitos.md) §2.6

## 1. Contexto

`03-arquitetura.md` §7 descreve a stack, as camadas, a cena 3D, o orçamento de
render e os requisitos de acessibilidade do studio. Não fixa, porém, os
contratos que impedem a camada de apresentação de virar um segundo emulador.

A revisão de dois projetos externos do mesmo autor — um guia interativo em React
e um conjunto de labs de firmware em PlatformIO — mostrou que o valor deles está
quase todo na camada de apresentação, e que essa camada é reimplementável sem
código. Nenhum dos dois tem licença: são todos os direitos reservados. Este ADR
descreve **comportamento e contrato**, no nível de especificação. Nenhum código,
nome interno, estrutura de arquivos, texto, ícone ou modelo foi copiado.

O risco endereçado aqui é específico e previsível: um studio de simulador tende a
acumular lógica de emulação na UI por conveniência — cálculo de tempo, estado de
pino, interpolação de PWM, fila de eventos. Quando isso entra no TypeScript, o
determinismo do core deixa de valer, o build nativo perde utilidade e o trace
dourado deixa de explicar o que a tela mostra.

## 2. Decisão

O studio segue oito padrões normativos. Cada um tem uma regra verificável.

### AP-01 — Catálogo de painéis com ID tipado e carga sob demanda

**Regra.** Todo painel, ferramenta e visualizador é registrado em um catálogo por
ID. Conteúdo (lição, fixture, projeto) referencia o ID e nunca importa o
componente. O módulo é carregado no primeiro uso. ID duplicado falha em teste.

**Motivo.** three.js, gráficos e analisador lógico não podem entrar no bundle
inicial; a meta é < 3 s de carregamento do studio.

**Verificação.** Teste que falha em ID duplicado e em ID órfão; teste que falha
se o grafo do bundle inicial importar three.js ou o analisador.

### AP-02 — Estado de simulação fora da camada reativa

**Regra.** O estado de simulação vive na bridge e é lido por frame. A camada
reativa recebe apenas mudanças discretas (comando aceito, conexão alterada,
falha). É proibido atualizar store ou estado de componente por evento de pino,
por ciclo ou por amostra.

**Motivo.** 60 fps e determinismo. Eventos de pino são agregados no core
(`03` §11); transformá-los em atualizações de UI reintroduz o acoplamento que a
ABI C existe para evitar.

**Verificação.** `worker_smoke` (E0-05) emite N eventos no Worker e exige que a
main thread os receba em uma única drenagem do ring, sem chamada JS por ciclo;
teste de coalescência injeta N eventos de pino em um frame e exige uma única
atualização de UI; perfil de render sem re-render por evento.

### AP-03 — Lógica de apresentação pura e testável

**Regra.** Projeção, formatação, geometria de fios, parsing de trace, conversão
de unidade e layout de painel ficam em módulos puros, sem DOM, sem three.js e
sem acesso ao worker. Esses módulos têm teste unitário.

**Motivo.** É a parte da UI que pode ser verificada sem navegador — e a que mais
regride.

**Verificação.** Cobertura dos módulos puros; lint de imports que rejeita
three.js e APIs de DOM nesses módulos.

### AP-04 — Contenção de erro em três níveis

**Regra.** Falha de rota, falha de painel e falha de desenho são contidas
separadamente. Um painel quebrado não derruba a lição nem o editor. Falha do
core é evento, não exceção (`03` §12).

**Motivo.** Um simulador em desenvolvimento falha; a tela inteira não pode
falhar junto.

**Verificação.** Teste que injeta falha em um painel e verifica que o restante da
tela continua operável e que o erro é registrado.

### AP-05 — i18n e RTL desde o primeiro commit

**Regra.** Nenhuma string visível inline: catálogos por namespace. CSS lógico
(`margin-inline`, `inset-inline`, `padding-block`). Números, unidades e datas
formatados por locale. Texto de conteúdo separado do texto de interface.

**Motivo.** Retrofitar i18n é caro; layout com propriedades físicas quebra em
RTL e obriga reescrita.

**Verificação.** Lint que rejeita texto literal em componente; teste de catálogo
que falha em chave ausente ou órfã.

### AP-06 — Acessibilidade operacional

**Regra.** Todo controle do bench (botão, chave, potenciômetro, sensor) é
operável por teclado e por leitor de tela. `prefers-reduced-motion` desliga
animação não essencial. O monitor serial usa `aria-live` controlado. Pinos e
conexões têm alternativa 2D/tabular.

**Motivo.** O bench virtual é a interface principal; um botão que só responde ao
mouse exclui parte dos usuários do produto.

**Verificação.** Teste de operação por teclado nos controles; auditoria
automática sem violações críticas.

### AP-07 — Cor vem de token

**Regra.** Nenhuma cor literal em canvas, three.js ou CSS de componente. A
paleta é lida dos tokens de tema e re-lida quando o tema muda.

**Motivo.** Tema claro/escuro e contraste acessível sem duplicar constantes.

**Verificação.** Teste que rejeita valor de cor literal fora do arquivo de
tokens.

### AP-08 — Analisador lógico consome trace, não animação

**Regra.** Diagramas de timing e o analisador lógico são renderizações de trace
do core (canal, ciclo, nível), reproduzíveis e navegáveis passo a passo. Nenhuma
sequência de timing é escrita na UI.

**Motivo.** O analisador é ferramenta de verificação e de ensino; se a fonte for
sintética, ele prova apenas que a UI sabe desenhar.

**Verificação.** O mesmo trace dourado produz o mesmo diagrama; teste que rejeita
dado de timing constante no widget.

### Rastreabilidade

Cada padrão serve a pelo menos um requisito de `02` §2.6. Se um requisito sair do
escopo, o padrão correspondente pode sair junto; o contrário não vale.

| Padrão | Requisitos ancorados |
| --- | --- |
| AP-01 | U-001, U-002 |
| AP-02 | U-001, U-006 |
| AP-03 | U-002, U-007 |
| AP-04 | U-005, U-009 |
| AP-05 | U-011 |
| AP-06 | U-010 |
| AP-07 | U-001, U-010 |
| AP-08 | U-007 |

## 3. Alternativas rejeitadas

1. **Deixar os padrões implícitos em `03` §7.** Sem regra verificável, a UI
   regride na primeira entrega com prazo curto.
2. **Adotar a stack do projeto de referência (React + Compiler + Vite).**
   Exigiria ADR próprio; `03` §7.1 já fixa Svelte 5 + Vite.
3. **Reaproveitar código ou assets das referências.** Bloqueado por licença
   (`08` §2).
4. **Modelar o firmware em TypeScript para "simular" mais rápido.** Contradiz o
   ADR 0001 e destrói o determinismo.
5. **Adiar i18n e acessibilidade para o V1.** Retrofit caro e risco de reescrita
   de layout.

## 4. Consequências

**Positivas.** Bundle inicial pequeno; a UI não pode virar emulador; falhas
contidas; i18n e acessibilidade baratas por terem nascido junto.

**Custos.** Catálogo e teste de IDs; lint de i18n e de cor; disciplina de frame
loop; mais testes de UI do que um protótipo exigiria.

**Riscos.** AP-02 está verificado no lado da bridge desde o E0-05; a coalescência
na camada reativa só é verificável quando o studio existir (E1-06). AP-06 pode
consumir tempo de design além do previsto.

## 5. Não decide

- Framework e versões — `03` §7.1.
- Formato de projeto e defaults — ADR 0006.
- Topologia de deploy e headers — ADR 0003.
- Escopo de features do studio — `02` §2.6.

## 6. Condições de aceite

- [x] AP-01 a AP-08 refletidos em
  [`03-arquitetura.md`](../03-arquitetura.md) §7.6.
- [x] Cada AP com verificação associada no plano de testes
  ([`06`](../06-conformidade-verificacao.md) §2.5).
- [x] `08` §2 registra as duas referências como "sem reutilização".
- [x] AP-02 verificado no E0-05 no lado da bridge (`worker_smoke`); a
  coalescência na camada reativa fica para o E1-06, quando o studio existir.

## 7. Critérios de revisão

- Novo framework ou salto de versão → ADR próprio.
- AP-02 não segurar 60 fps na prática → revisar a bridge, não afrouxar a regra.
- Nova referência externa sem licença → apenas padrões, registrada em `08` §2.
- Custo de AP-05/AP-06 ameaçar G1 → cortar escopo de feature, nunca o padrão.
