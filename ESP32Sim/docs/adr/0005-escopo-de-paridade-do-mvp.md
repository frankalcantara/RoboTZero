# ADR 0005 — Escopo de paridade do MVP

- **Status:** Aceito
- **Data:** 2026-10-25
- **Data de aceite:** 2026-10-25
- **Decisores:** lead, produto, engenharia
- **Evidência de entrada:** EV-2026-10-25-02 —
  [`evidence/2026-10-25/wokwi-feature-inventory/`](../../evidence/2026-10-25/wokwi-feature-inventory/README.md)
- **Relaciona-se com:** ADR 0002 (prioridade de chips), ADR 0006 (identidade e
  defaults)

## Contexto

O inventário documental EV-2026-10-25-02 lista **85 linhas** de funcionalidades
documentadas do Wokwi no recorte ESP32-C3/MVP: **84 funcionalidades + 1 agregado
de ausências** (H-023). Dessas, **30** foram marcadas como `MVP` na coluna de
relevância — que é **avaliação do lead**, não fato do documento nem requisito do
ESP32Sim.

O projeto já tem escopo de MVP em
[`02-escopo-requisitos.md`](../02-escopo-requisitos.md) §4 e Fase 1 em
[`04-plano-de-trabalho.md`](../04-plano-de-trabalho.md) §4. O risco desta ADR é
duplo:

1. **Scope creep:** tratar "o Wokwi documenta X" como "o ESP32Sim precisa de X
   no MVP", importando 30 itens para o backlog sem critério.
2. **Promessa de paridade:** comunicar paridade ampla e falhar no G1, que exige
   apenas blink, serial, timer/IRQ, boot, trace, projeto compartilhável e deploy.

O alvo `wokwi.com` continua **`blocked`**; a fonte é a documentação MIT, com
commit pinado, e **nada foi verificado em execução**. O parecer clean-room está
em [`audit-clean-room.md`](../../evidence/2026-10-25/wokwi-feature-inventory/audit-clean-room.md).

> **Atribuição:** `wokwi/wokwi-docs` @ `5c90e532d24fb8ac4cd471e9568db03917011e91`,
> MIT, Copyright (c) 2021-2024 CodeMagic LTD. Nenhum código, asset ou imagem do
> alvo foi usado.

## Decisão proposta

### 1. Paridade não é critério do MVP

O critério de aceitação do MVP continua sendo o **G1** de
[`04-plano-de-trabalho.md`](../04-plano-de-trabalho.md) §4, não a lista do
Wokwi. O inventário é **insumo de priorização e de comunicação**, não baseline
de aceitação.

Uma linha do inventário só vira requisito se:

- já existir requisito correspondente em `02-escopo-requisitos.md`; ou
- for aprovada como requisito novo, com spec própria e fonte de implementação
  independente (TRM, datasheet, Arduino-ESP32, ESP-IDF, hardware).

### 2. Quatro disposições

| Código | Significado | Vira requisito? |
| --- | --- | --- |
| **P0** | Paridade mínima do MVP | Sim, por meio de requisito já existente em `02`/`04` |
| **P1** | Paridade posterior | Não agora; mapeada para Fase 2 ou 3 |
| **P2** | Não objetivo explícito | Não |
| **U** | Desconhecido | Não; exige evidência própria antes de decidir |

### 3. Disposição das 30 linhas marcadas `MVP` no inventário

Esta tabela é a **proposta do lead** e substitui, para fins de escopo, a coluna
de relevância do inventário. Onde `02`/`04` divergirem do inventário, **`02`/`04`
prevalece**.

| ID | Funcionalidade | Disposição | Requisito / observação |
| --- | --- | --- | --- |
| F-001 | Arduino-ESP32 / ESP-IDF no navegador | **P0** (só Arduino) | `02` F-003; ESP-IDF é `02` F-004 (V1) → P1 |
| F-005 | Placa ESP32-C3-DevKitM-1 | **P0** | `02` §1; `04` E1-06 |
| H-001 | Núcleo do processador | **P0** | `02` C-001 |
| H-002 | GPIO | **P0** | `02` P-001 |
| H-003 | IOMUX | **P0** | `02` P-001 |
| H-004 | UART | **P0** | `02` P-002 |
| H-005 | USB Serial + JTAG | **P1** | `02` P-015; hipótese: MVP usa UART0 (H-004) até UN-007. Se UN-007 indicar USB CDC, H-005 e S-009 sobem a P0 |
| H-012 | Timers | **P0** | `02` P-003 |
| H-013 | Watchdog | **P0** (básico) | `04` E1-02 prevalece para escopo de fase; `02` P-013 alinhado em 2026-10-25 (§9) |
| E-001 | Adicionar parte | **P0** | `02` U-002; `04` E1-06 |
| E-002 | Mover parte | **P0** | idem |
| E-003 | Rotacionar parte | **P1** | UX; não bloqueia o blink |
| E-004 | Duplicar parte | **P1** | UX |
| E-005 | Excluir parte | **P0** | `02` U-002 |
| E-006 | Seleção múltipla | **P1** | UX |
| E-007 | Copiar/colar | **P1** | UX |
| E-008 | Criar fio | **P0** | `02` U-002; `04` E1-06 |
| E-009 | Cor do fio | **P1** | UX |
| E-010 | Excluir fio | **P0** | `02` U-002 |
| E-011 | Atalhos de teclado | **P1** | Sem requisito-âncora em `02`/`04`; zoom/undo/redo são recomendados, não P0 |
| E-012 | Grid e régua | **P1** | Sem requisito-âncora em `02`/`04`; snap é recomendado, não P0 |
| S-001 | Anexo automático à UART (Uno/Mega) | **P2** | Documentado só para Uno/Mega; não é paridade do C3 |
| S-001 (C3) | Serial padrão do C3 | **U** | Derivado de S-001; **não é linha adicional do inventário**; ver U-001 |
| S-002 | Modos de exibição | **P0** (`auto`/`always`/`never`) | `02` U-004 (exibir TX/RX); `plotter`/`terminal` P1 |
| S-004 | Collapse | **P1** | UX |
| S-005 | Newline de entrada | **P1** | `02` U-004 cobre TX/RX, não a convenção de newline |
| S-007 | Entrada para o firmware | **P0** | `02` U-004 (TX/RX) |
| S-008 | Configuração no diagrama | **P0** | `04` E1-06 (monitor serial) |
| S-009 | USB CDC no C3 | **P1** | `02` P-015; **UN-007** (não U-001) |
| B-001 | Autocomplete de include | **P1** | `02` F-009 é compilação de sketch; autocomplete é IDE, não emulação |
| B-002 | Bibliotecas embutidas | **P1** | Propriedade do Arduino core/toolchain, não do simulador |

**Contagem da proposta:** 16 `P0`, 13 `P1`, 1 `P2` entre as 30 linhas do
inventário; mais **1 `U` derivada** (S-001/C3, que não é linha adicional). As
demais 55 linhas seguem a disposição por área abaixo.

> **Editor:** para o editor, `P0` é o conjunto mínimo para montar e editar o
> blink (adicionar, mover, excluir parte; criar e excluir fio), ancorado em
> `02` U-002 e `04` E1-06. Não é paridade das 12 linhas do inventário.

### 4. Disposição por área (linhas fora das 30)

A tabela agrupa faixas; não repete as 30 linhas de §3.

| Área | Disposição | Observação |
| --- | --- | --- |
| F — Firmware/placa | F-004 **P0** (por `02` F-008); F-007, F-009, F-010, F-011, F-012 **P1**; F-002, F-003, F-006, F-008, F-013, F-014 **P2** | F-004 sobe a P0 porque `02` F-008 exige upload de firmware pré-compilado, apesar de o inventário marcá-lo Fase 2. F-002 desce de Fase 2 (inventário) para P2. F-010 é P1 porque `02` F-002/`04` E1-03 exigem partições corretas, não um `partitions.csv` de projeto |
| H — Periféricos | H-006..H-011, H-014..H-022 **P1**; H-023 **P2** (agregado de ausências) | H-020/H-021 são parciais e ficam para a Fase 3 |
| S — Serial | S-003, S-006, S-010 **P1** | Terminal, EOL e plotter |
| L — Logic analyzer | L-001..L-008 **P1** | Fase 2 (`04` E2-07) |
| D — Debugger web | D-001..D-004 **P2** | Beta e só AVR; `02` U-008 (Depurador) é atendido por GDB (`02` F-007, `04` E2-06), não por este item |
| W — Wi-Fi | W-001, W-002, W-004, W-007, W-008, W-009 **P1**; W-003, W-005, W-006 **P2** | Fase 3; paridade de recursos pagos não é compromisso |
| B — Bibliotecas | B-003, B-005 **P1**; B-004 **P2** | Upload pago não é compromisso |

### 5. Não objetivos explícitos de paridade

- **Debugger no navegador para o C3.** O debugger embutido documentado é beta e
  só AVR. Para o C3, GDB é Fase 2 (`02` F-007; `04` E2-06), não paridade de MVP.
- **Wi-Fi, BLE e 802.15.4.** Fase 3; fora do caminho crítico do G1.
- **MicroPython, CircuitPython e Rust.** Não estão no roteiro atual.
- **PSRAM, I2S, Bluetooth, MCPWM, PCNT, ECC, Hall e ULP no C3.** Não existem no
  chip ou não estão implementados; não há paridade a prometer.
- **Recursos de plano pago do terceiro** (APs customizados, Private Gateway,
  upload de biblioteca). Não são compromisso de produto.
- **Compatibilidade byte a byte com o formato de projeto do Wokwi.** A meta é
  interoperabilidade documentada, não clone (`00-visao-geral.md` §7).

### 6. Regras

1. Nenhuma linha do inventário vira requisito sem spec própria e fonte de
   implementação independente.
2. Onde `02`/`04` divergir do inventário, `02`/`04` prevalece — nos dois
   sentidos (F-004 sobe; F-002, S-001 e B-001/B-002 descem).
3. Identificadores de marca e defaults de rede seguem o ADR 0006.
4. A atribuição MIT da documentação é mantida; nenhum código, asset, imagem ou
   modelo do alvo entra no produto.
5. A coluna de relevância do inventário é **avaliação**, não fato; a
   classificação "observado (documental)" não autoriza declarar funcionamento.

### 6.1 Desempate entre `02` e `04`

Quando `02` e `04` divergirem entre si:

- `04` prevalece para **escopo de fase** (o que entra no MVP);
- `02` prevalece para **critério de aceitação** (como provar que entrou);
- se o conflito for normativo nos dois sentidos, a decisão é registrada e o
  requisito conflitante é corrigido no mesmo PR — não se escolhe a fonte por
  conveniência.

Aplicação imediata: H-013 (watchdog). `04` E1-02 inclui "watchdog básico" na
Fase 1; `02` P-013 marca watchdog como V1 parcial. Vale `04` para escopo, e
atualizar `02` P-013 é **condição de aceite** desta ADR (§9).

### 6.2 Namespaces de incógnitas

`U-001` deste inventário é o ID do **inventário** (comportamento do C3). Não
confundir com `02` U-001 ("Cena 3D") nem com `UN-007` da ficha irmã (pergunta
estreita: "o C3 usa USB CDC por padrão?"). S-001 usa U-001; S-009 usa UN-007.

### 7. Consequências

#### Positivas

- O G1 continua sendo o critério; o backlog não infla com 30 itens.
- A comunicação de suporte fica honesta: há lacunas declaradas.
- O clean-room fica preservado: paridade é decisão de escopo, não cópia.
- O conflito `02` × `04` (watchdog) e a exceção F-004 ficam registrados.

#### Negativas

- Usuários que esperam paridade com o Wokwi encontrarão diferenças: sem
  debugger de C3 no navegador, sem Wi-Fi antes da Fase 3, sem MicroPython.
- O MVP **assume UART0** como hipótese; se UN-007 indicar USB CDC, H-005 e
  S-009 sobem a P0 e a experiência da DevKitM-1 precisa ser reavaliada.
- Renomear identificadores exige adaptador se houver importação de projetos.

#### Mitigações

- Publicar matriz de suporte e "limitações conhecidas" no studio.
- Manter a lista de 85 linhas como material de comunicação, com a disposição
  desta ADR.
- Reavaliar a disposição a cada gate, com dados de uso.

### 8. Questões abertas

| ID | Questão | Dono | Quando |
| --- | --- | --- | --- |
| U-001 (inventário) | Comportamento do C3 no studio (auto-attach, timing, periféricos) | `firmware-runtime-engineer` | antes de fechar o escopo de serial |
| UN-007 (ficha irmã) | O C3 usa USB CDC por padrão? | `firmware-runtime-engineer` | antes do aceite de `04` E1-02 |
| UN-009 | Nomes de pino da placa C3 | `rea-evidence` | antes das fixtures de blink |
| — | `partitions.csv` é artefato de projeto ou do binário? | `esp32-lead` / `cpp-core-engineer` | antes de `04` E1-03 |

> **Rastreabilidade:** o item A3 da ficha irmã
> ([`audit-clean-room.md`](../../evidence/2026-10-25/wokwi-c3-diagram-contract/audit-clean-room.md) §7)
> referenciava `docs/adr/0005-...` como slot reservado para o parecer jurídico de
> desbloqueio. O slot 0005 agora é esta ADR; o registro jurídico deve usar o
> próximo número livre (0007+).

## 9. Condições de aceite

- [x] `02` P-013 alinhado a `04` E1-02 (watchdog básico no MVP).
- [x] Critérios de `02` U-002 e U-004 apertados: E-001/E-002/E-005 e S-002
  passam a ter critério verificável.
- [x] H-005 e S-009 explicitamente condicionados a UN-007, sem premissa fechada.
- [x] Nenhum `P0` sem requisito-âncora em `02`/`04`.
- [x] ADRs 0005 e 0006 incluídos no índice de `03-arquitetura.md` §15 e nos
  entregáveis de `04` §3.

> **Status (2026-10-25):** condições atendidas e ADR aceito; resta a decisão de
> UN-007 para H-005/S-009.

## 10. Critérios de revisão

- G1 ameaçado por escopo → cortar `P1`, nunca `P0`.
- Evidência nova de uso (serial, Wi-Fi, debugger) → reavaliar a disposição.
- Mudança de fase ou de chip → revisar a tabela por área.
- Novo parecer clean-room ou mudança de licença → revisar as regras.
- Se o custo de um `P1` cair abaixo do custo de mantê-lo fora, promover com
  spec própria — não por paridade.
