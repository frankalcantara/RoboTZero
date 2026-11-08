# 08 — Riscos e licenças

> Este documento é um registro de engenharia, não parecer jurídico. Decisões de
> distribuição exigem revisão profissional.

## 1. Política clean-room

### 1.1 Separação de papéis

| Papel | Pode | Não pode |
| --- | --- | --- |
| **Evidência** (`rea-evidence`) | Observar comportamento, capturar tráfego, ler documentação pública, escrever specs | Escrever código de produção do comportamento observado |
| **Auditoria** (`clean-room-auditor`) | Revisar proveniência, licenças, diffs e assets | Implementar features |
| **Implementação** (demais agentes) | Ler specs, datasheets, TRMs e código open-source compatível | Ler/copiar código proprietário do alvo |
| **Lead** | Orquestrar, decidir, revisar | Ignorar bloqueios de licença |

### 1.2 Regras

1. A spec descreve **comportamento**, não código.
2. Nenhum trecho de código, CSS, HTML, modelo 3D, imagem, fonte ou dado
   proprietário entra no repositório.
3. Nomes de APIs públicas podem ser usados para interoperabilidade; nomes
   internos proprietários não.
4. Assets precisam de licença explícita e registro em `provenance/`.
5. Dependências GPL/AGPL exigem decisão registrada antes de entrar no build.
6. Em dúvida: bloquear, registrar e escalar.

### 1.3 Manifesto de proveniência

Cada asset/dependência deve ter:

```yaml
id: board-esp32-c3-devkitm-1
type: 3d-model
source: "https://..."
author: "..."
license: "CC-BY-4.0"
retrieved_at: "2026-10-22"
sha256: "..."
modifications: "escala, materiais"
allowed_in_distribution: true
reviewed_by: "clean-room-auditor"
```

## 2. Licenças de referências e dependências

| Projeto | Licença | Uso permitido | Obrigações/riscos |
| --- | --- | --- | --- |
| REA | MIT | Ferramenta de análise | Nenhuma além do aviso; ferramentas externas à parte |
| QEMU | GPLv2 | Oráculo/teste; possível runtime | Distribuir binário/WASM exige cumprir GPLv2; revisar |
| QEMU lcgamboa | GPLv2 (verificar) | Referência/oráculo | Fork pode ter termos adicionais; revisar |
| Velxio | AGPLv3 | Referência de arquitetura | Copyleft forte; **não copiar**; uso em rede pode obrigar liberar fonte |
| avr8js | MIT | Referência de design AVR | Não cobre ESP32 |
| wokwi-elements | MIT | Referência visual | Assets podem ter licenças próprias; verificar cada um |
| wokwi/wokwi-docs | MIT | Documentação de referência (evidência) | Incluir aviso; não cobre software/marca do simulador |
| `agharsallah/esp-labs-web` | **Nenhuma** (todos os direitos reservados) | Referência de padrões de apresentação | **Nenhuma reutilização**: não copiar código, textos, GIFs, ícones ou modelos. Padrões reimplementados a partir do [ADR 0007](adr/0007-padroes-de-apresentacao-do-studio.md) |
| `agharsallah/esp-labs` | **Nenhuma** (todos os direitos reservados) | Referência de escopo de firmware | **Nenhuma reutilização**: não copiar firmware, mapa de pinos nem testes. Protocolos vêm de ISO/NXP/datasheet, não da implementação |
| three.js | MIT | Render 3D | Incluir aviso |
| Svelte | MIT | UI | Incluir aviso |
| Vite | MIT | Build | Incluir aviso |
| TypeScript | Apache-2.0 | UI | Incluir aviso |
| Emscripten | MIT/UIUC | Build WASM | Incluir aviso |
| CMake | BSD-3-Clause | Build | Incluir aviso |
| Catch2 | BSL-1.0 | Testes | Incluir aviso |
| GoogleTest | Apache-2.0 | Testes | Incluir aviso |
| WABT | Apache-2.0 | Inspeção WASM | Incluir aviso |
| ESP-IDF | Apache-2.0 | Firmware de teste/compilação | Componentes de terceiros podem ter outras licenças |
| Arduino core ESP32 | LGPL-2.1 (verificar) | Firmware de teste | Distribuição de binário/linkagem exige revisão |
| Arduino CLI | GPLv3 (verificar) | Compilação | Não embutir no cliente; usar serviço separado |
| FreeRTOS | MIT | Firmware | Incluir aviso |
| lwIP | BSD | Conectividade | Incluir aviso |
| OpenOCD | GPLv2 | Debug | Ferramenta separada; revisar distribuição |
| Cloudflare | Termos comerciais | Hospedagem | Custos, limites, dados |
| TRMs/datasheets Espressif | Copyright Espressif | Engenharia | Não redistribuir; citar versão |

> “Verificar” significa: confirmar a licença exata na versão pinada antes de
> usar em distribuição.
>
> **Sem licença = sem permissão.** Um repositório público sem `LICENSE` e sem
> campo `license` está sob direitos autorais reservados. Ele pode ser lido como
> referência de comportamento, mas nenhum código, texto, asset ou dado pode ser
> copiado, adaptado ou redistribuído. Ideias e padrões só entram por spec escrita
> pela equipe, como no ADR 0007.

## 3. Estratégia de licenciamento do produto

### Opção A — Core permissivo (recomendado)

- Código próprio sob MIT/Apache-2.0.
- Dependências permissivas no build distribuído.
- GPL/AGPL apenas em ferramentas separadas (CI, oráculo, scripts não
  distribuídos) ou com isolamento jurídico.

**Prós:** adoção, contribuições, uso comercial.
**Contras:** concorrentes podem usar; necessidade de revisão de patentes.

### Opção B — Core aberto com copyleft

- GPLv3/AGPLv3 para o core.
- Garante abertura de melhorias.

**Prós:** protege o commons.
**Contras:** adoção comercial menor; obrigações de rede (AGPL); incompatível com
alguns clientes.

### Opção C — Núcleo aberto + edição comercial

- Core MIT/Apache e serviços/enterprise sob termos próprios.
- Modelo comum em ferramentas de desenvolvimento.

**Decisão:** pendente. Registrar em ADR antes do primeiro release público.

## 4. Matriz de riscos

Escala: probabilidade (P) e impacto (I) de 1 a 5.

| ID | Risco | P | I | Severidade | Mitigação | Dono |
| --- | --- | --- | --- | --- | --- | --- |
| R-01 | ToS/EULA do Wokwi proíbe engenharia reversa | 3 | 5 | 15 | Revisão jurídica; plano B sem REA; fontes públicas | lead/clean-room |
| R-02 | Violação de copyright por copiar código/assets | 2 | 5 | 10 | Clean-room, proveniência, revisão de diff | clean-room |
| R-03 | GPL do QEMU contaminar distribuição | 3 | 5 | 15 | QEMU só em CI/oráculo; revisão antes de WASM | clean-room |
| R-04 | AGPL do Velxio contaminar o produto | 2 | 5 | 10 | Não copiar; usar apenas conceitos públicos | clean-room |
| R-05 | Marca “ESP32”/“Wokwi” usada incorretamente | 3 | 3 | 9 | Uso descritivo; revisão de marca; disclaimer | lead |
| R-06 | Escopo explode (Wi-Fi/BLE/câmera) | 5 | 4 | 20 | Fases, gates, não objetivos | lead |
| R-07 | Desempenho C++→WASM abaixo do tempo real | 4 | 4 | 16 | Spikes, benchmark, otimização por perfil | wasm/cpp |
| R-08 | Fidelidade de periféricos insuficiente | 4 | 4 | 16 | TRM, testes, diferencial QEMU/hardware | soc |
| R-09 | Documentação de registradores incompleta | 4 | 3 | 12 | Marcar `unknown`, testes, engenharia reversa lícita | soc |
| R-10 | Custo/latência de compilação | 3 | 3 | 9 | Build local no MVP; Container com quotas | cloudflare |
| R-11 | Limites da Cloudflare | 3 | 3 | 9 | Simulação no cliente; arquitetura desacoplada | cloudflare |
| R-12 | COOP/COEP quebra integrações/embed | 3 | 3 | 9 | Fallback single-thread; testes | wasm/web |
| R-13 | Supply chain (dependência maliciosa) | 2 | 5 | 10 | Pin, SBOM, auditoria, lockfile | all |
| R-14 | Assets 3D sem licença | 3 | 4 | 12 | Proveniência obrigatória; substituir | clean-room/web |
| R-15 | Time pequeno e escopo grande | 4 | 4 | 16 | Fases, cortes, agentes, priorização | lead |
| R-16 | Regressão silenciosa de timing | 4 | 4 | 16 | Goldens, CI, determinismo | verification |
| R-17 | Firmware malicioso explorando o emulador | 2 | 4 | 8 | Validação, sandbox WASM, limites, fuzzing | cpp/verification |
| R-18 | Perda de dados de projetos | 2 | 4 | 8 | Backup, export, versionamento | cloudflare |
| R-19 | Dependência de browser específico | 2 | 3 | 6 | Matriz de browsers, fallback | web |
| R-20 | Patentes de Wi-Fi/BLE/ISA | 2 | 5 | 10 | Revisão jurídica antes de V2/V3; usar implementações licenciadas | lead |

### 4.1 Riscos de maior prioridade

1. **R-01/R-03/R-04** — licenças e termos podem inviabilizar partes do plano.
2. **R-06** — escopo é o maior risco de execução.
3. **R-07/R-08/R-16** — desempenho e fidelidade são o núcleo técnico.
4. **R-15** — capacidade do time.

### 4.2 Gatilhos de bloqueio

- Termos proíbem observação → bloquear `rea-evidence` no alvo.
- Licença GPL/AGPL em componente distribuído → bloquear release até decisão.
- Asset sem proveniência → não entra no repositório.
- Golden quebrado → não faz merge.
- Divergência de hardware não explicada → não declara compatibilidade.

## 5. Política de marcas

- “ESP32” é marca da Espressif. Usar apenas para descrever compatibilidade:
  “simulador compatível com ESP32-C3”, não “ESP32 oficial”.
- “Wokwi” é marca de seu proprietário. Não usar em nome do produto, logo ou
  domínio; não sugerir afiliação/endosso.
- Incluir disclaimer: “Projeto independente, não afiliado à Espressif ou à
  Wokwi.”
- Revisar nomes de placas/componentes; alguns são marcas de terceiros.

## 6. Política de dados

- Firmware e projetos do usuário pertencem ao usuário.
- Coletar o mínimo; permitir export/exclusão.
- Não treinar modelos com código do usuário sem consentimento explícito.
- Logs agregados e anonimizados.
- Local-first quando possível.

## 7. Processo de revisão de licença

1. Identificar componente/asset e versão.
2. Ler a licença no repositório oficial.
3. Classificar: permissiva, fraca copyleft, forte copyleft, proprietária,
   desconhecida.
4. Avaliar modo de uso: ferramenta, build, linkagem, distribuição, SaaS.
5. Registrar obrigações (aviso, fonte, patente, marca).
6. Se desconhecida ou incompatível: bloquear.
7. Atualizar `THIRD_PARTY_NOTICES` e SBOM.

## 8. Checklist de release

- [ ] Todas as dependências têm licença identificada.
- [ ] `THIRD_PARTY_NOTICES` atualizado.
- [ ] SBOM gerada e assinada.
- [ ] Nenhum asset sem proveniência.
- [ ] Nenhum código proprietário copiado.
- [ ] Termos de uso do alvo revisados.
- [ ] Disclaimer de marcas presente.
- [ ] Política de privacidade publicada.
- [ ] Exportação/exclusão de dados funcionando.
