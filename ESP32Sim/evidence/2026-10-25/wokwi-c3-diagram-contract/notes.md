# Notas de análise — contrato de `diagram.json`

Estas notas separam **o que foi observado** do que foi **derivado**, **inferido**
ou permanece **desconhecido**. Elas não substituem a ficha
[`README.md`](README.md) nem a especificação [`spec.md`](spec.md).

## 1. Por que o escopo mudou

O alvo original era a página de criação de projeto
`https://wokwi.com/projects/new/esp32-c3`. O gate legal concluiu `blocked`:
o ToS veda uso não pessoal/não comercial, coleta automatizada, acesso para
desenvolver produto similar/concorrente e engenharia reversa sem autorização
escrita. O ESP32Sim é explicitamente um simulador concorrente; não há
autorização registrada.

Em vez de parar o trabalho, o parecer do `clean-room-auditor` indicou uma fonte
lícita para a parte mais valiosa da pergunta: a documentação do formato de
projeto é publicada sob **MIT** em `wokwi/wokwi-docs`. O escopo foi reduzido
para o **contrato de arquivo de diagrama** e a coleta foi feita nessa fonte,
com commit pinado.

Isso não é uma violação disfarçada: é a rota que o próprio projeto declara
preferir — "interoperabilidade documentada, não clone" (`docs/00-visao-geral.md`
§7) — e que o parecer classificou como `clean`.

## 2. O que a evidência sustenta

### 2.1 Estrutura do documento

`diagram.json` é um objeto JSON com:

- `version`: sempre `1` (documentado);
- `author`: nome de quem criou o arquivo;
- `editor`: aplicação usada para editar (ex.: `"wokwi"`);
- `parts`: array de componentes;
- `connections`: array de conexões;
- `serialMonitor`: bloco de configuração (obrigatoriedade não declarada —
  UN-002).

A documentação diz que `version` é sempre 1 e que `author`/`editor` são
metadados. Não há afirmação sobre obrigatoriedade em runtime.

### 2.2 Partes

Campos documentados:

| Campo | Obrigatório? | Observação |
| --- | --- | --- |
| `id` | sim | único; duplicata "pode não funcionar" |
| `type` | sim | ex.: `wokwi-led` |
| `left` | não | coordenada x em pixels |
| `top` | não | coordenada y em pixels |
| `attrs` | não | atributos específicos do tipo |
| `rotate` | não | rotação em graus |
| `hide` | não | oculta a parte |

A placa ESP32-C3 aparece como `board-esp32-c3-devkitm-1` na lista de
microcontroladores. O LED aparece como `wokwi-led`, com pinos `A` (anodo) e
`C` (catodo) e atributo `color` com default `"red"`.

### 2.3 Conexões

Cada conexão é um array posicional de quatro itens:

```text
[ "origem:pino", "destino:pino", "cor", [instruções de fio] ]
```

- cor vazia (`""`) oculta o fio;
- o quarto item é opcional;
- o mini-language usa `vN` (vertical), `hN` (horizontal) e no máximo um `*`;
- as instruções **antes** de `*` seguem a ordem escrita; **apenas** as
  instruções **depois** de `*` são aplicadas em ordem reversa.

### 2.4 Monitor serial

- Para **Arduino Uno/Mega**, anexa automaticamente à UART de hardware e
  detecta o baud rate. Para o C3, auto-attach e auto-baud não estão
  declarados (UN-007).
- Só aparece quando há saída, por padrão (`display: "auto"`).
- Configuração padrão: `collapse=false`, `convertEol=false`, `display="auto"`,
  `newline="lf"`.
- `display` aceita `auto`, `always`, `never`, `plotter`, `terminal`.
- `newline` aceita `lf`, `cr`, `crlf`, `none`.
- É possível rotear explicitamente por `$serialMonitor:TX` /
  `$serialMonitor:RX`; nesse caso a conexão é feita aos pinos RX/TX da UART.
- Para USB CDC, o chip aceita `serialInterface: "USB_SERIAL_JTAG"` e as
  conexões com `$serialMonitor` devem ser removidas.

## 3. Classificação

| Nível | Itens | Pode virar requisito? |
| --- | --- | --- |
| **Observado** | EV-001 a EV-012 | sim, com citação |
| **Observado (contextual)** | EV-013 (matriz de features do C3) | **não**; não é usado por nenhum requisito da spec |
| **Derivado** | DV-001 a DV-005 (conclusões lógicas dos documentos) | sim, com justificativa |
| **Inferido** | nenhum item promovido | não sem teste |
| **Desconhecido** | UN-001 a UN-009 | não; registrar como `unknown` |

Não há itens "inferidos" nesta ficha porque a etapa foi deliberadamente
documental. O que não está escrito ficou como `unknown`.

## 4. Consistência verificada

- `board-esp32-c3-devkitm-1` aparece 1 vez em `diagram-format.md`.
- `wokwi-led` aparece 5 vezes em `diagram-format.md` e 9 vezes em
  `wokwi-led.md`.
- `$serialMonitor` aparece 8 vezes em `serial-monitor.md` e 1 vez em
  `esp32.md`.
- `serialInterface` e `USB_SERIAL_JTAG` aparecem 1 vez cada em `esp32.md`.
- Os defaults de `serialMonitor` aparecem no bloco de configuração de
  `serial-monitor.md`.

O que **não** foi verificado:

- se o runtime do alvo valida exatamente esses campos;
- se o editor preserva campos desconhecidos;
- se `author`/`editor` são exigidos na prática;
- se o C3 usa USB CDC por padrão;
- se a documentação está sincronizada com o serviço na data da leitura;
- **quais são os nomes de pino da placa C3** — as páginas coletadas não os
  listam; a documentação sugere descobri-los na UI do alvo, que está
  `blocked` (UN-009).

## 5. Consequências de engenharia

1. **Adaptador, não modelo interno.** O `diagram.json` deve ser lido/escrito na
   fronteira do studio. O core recebe uma representação binária compacta
   (`docs/03-arquitetura.md` §10). Isso evita acoplar o core a um formato de
   terceiro.
2. **Tabela de mapeamento versionada.** Tipos públicos (`board-esp32-c3-devkitm-1`,
   `wokwi-led`) precisam de uma tabela explícita para IDs internos. Tipos
   desconhecidos não podem ser descartados em silêncio.
3. **Preservação de campos.** Para round-trip, campos não interpretados
   (incluindo `author`, `editor`, `left`, `top`, `rotate`, `hide`, `attrs` e
   instruções de fio) devem ser preservados.
4. **Serial como evento.** O monitor serial é projeção de eventos do core; não
   é estado no TypeScript.
5. **Sem assets do alvo.** Modelos 3D de placa/LED precisam de fonte licenciada
   própria e registro em `provenance/`.

## 6. O que deliberadamente não foi feito

- Não foi aberto o alvo, nem via navegador, nem via CDP/REA.
- Não foram capturados DOM, rede, WebSocket, console, storage ou scripts.
- Não foram baixados bundles, WASM, imagens ou modelos.
- Não foram acessados projetos públicos da comunidade.
- Não foi criado firmware dentro do alvo.
- Não foi enviado feedback à Wokwi.
- Não foi implementado código de produção.
- Não foi alterado nenhum doc de arquitetura/escopo.

## 7. Perguntas abertas

1. Qual é o nome e a organização dos arquivos de código de um projeto
   Arduino/ESP-IDF? (`sketch.ino`? `src/main.cpp`? `platformio.ini`?)
2. Como o projeto representa bibliotecas (`libraries.txt`)?
3. Qual é o contrato de compilação e de imagem (bin/elf/uf2, offsets,
   partições)?
4. Qual é o comportamento observável de boot e serial de um blink C3 em
   hardware real?
5. Como o ESP32Sim deve tratar tipos de partes desconhecidos: erro, quarentena
   ou preservação?
6. Qual é a política de versionamento do adaptador quando o formato público
   mudar?
7. Quais são os nomes de pino da placa C3 e de onde extraí-los de forma
   licenciada (pinout Espressif, `wokwi-elements` MIT com verificação por
   asset)?

Essas perguntas pertencem a etapas seguintes e não foram respondidas aqui.
