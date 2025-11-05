# Estrutura de Dados para Logging do Robô Seguidor de Linha



## Objetivo
Sistema de logging para armazenar dados de telemetria do robô durante a corrida, permitindo análise posterior do desempenho.

## Modos de Operação

### Inicialização
- Lê flag da EEPROM
- Se flag = 1 → Modo Report
- Se flag = 0 → Modo LineFollower

### Modo LineFollower
- Operação normal do robô
- Se DEBUG_LEVEL > 0:
  - Armazena dados na Flash periodicamente
- Ao finalizar corrida com debug ativo:
  - Seta flag = 1 na EEPROM

### Modo Report
- Aguarda botão
- Se botão pressionado:
  - Por ≥ 3 segundos: Limpa flag na EEPROM e retorna ao modo normal
  - Por < 3 segundos: Envia dados pela serial e permanece em modo report

## Estruturas de Dados

### 1. Registro de Eventos (8 bytes)
| Campo         | Tipo    | Tamanho | Descrição                    |
|--------------|---------|----------|------------------------------|
| timestamp    | uint32_t | 4 bytes | Milissegundos desde início   |
| tipo_evento  | uint8_t  | 1 byte  | Tipo do evento              |
| dados_evento | uint16_t | 2 bytes | Dados específicos do evento  |
| checksum     | uint8_t  | 1 byte  | Validação                   |

#### Tipos de Eventos
- INICIO_PROVA (0x01)
- INICIO_CURVA (0x02)
- FIM_CURVA (0x03)
- FIM_PROVA (0x04)

### 2. Registro de Correção (12 bytes)
| Campo            | Tipo    | Tamanho | Descrição                    |
|-----------------|---------|----------|------------------------------|
| timestamp       | uint32_t | 4 bytes | Milissegundos desde início   |
| erro           | int16_t  | 2 bytes | Erro da linha               |
| correção       | int16_t  | 2 bytes | Valor da correção aplicada   |
| velocidade_left | uint8_t  | 1 byte  | PWM motor esquerdo          |
| velocidade_right| uint8_t  | 1 byte  | PWM motor direito           |
| estado         | uint8_t  | 1 byte  | Flags de estado             |
| checksum       | uint8_t  | 1 byte  | Validação                   |

#### Bits do Campo Estado
- bit 0: em_curva
- bit 1: modo_preciso
- bit 2-7: reservado

## Frequência de Amostragem
- Em retas: 0.2 segundos (5 Hz)
- Em curvas: 0.1 segundos (10 Hz)

## Cálculos de Memória

### Por Segundo
1. Em reta:
   - 5 registros de correção/segundo
   - 5 * 12 bytes = 60 bytes/segundo

2. Em curva:
   - 10 registros de correção/segundo
   - 10 * 12 bytes = 120 bytes/segundo
   - 2 registros de evento (início/fim)
   - 2 * 8 bytes = 16 bytes
   - Total: 136 bytes/segundo de curva

### Volta Típica (30 segundos)
- 6 curvas (~6 segundos total)
- 24 segundos em reta
- 14 eventos totais (início/fim + 6 curvas)
- Eventos: 14 * 8 = 112 bytes
- Retas: 24s * 60 bytes/s = 1440 bytes
- Curvas: 6s * 136 bytes/s = 816 bytes
- **Total por volta: 2368 bytes**

### Capacidade Total
- Flash disponível: ~20KB
- Número de voltas: ~8-9 voltas completas

## Implementação

### Uso da Memória
- Flash: Armazenamento dos registros
- EEPROM: Flag de modo (1 byte)
  - 0x00: Modo normal
  - 0x01: Modo report

### Considerações
1. Escrita na Flash:
   - Deve ser feita em blocos/páginas
   - Não pode ocorrer durante interrupções
   - Processo relativamente lento

2. Gerenciamento de Memória:
   - Manter ponteiro de posição atual
   - Verificar espaço disponível
   - Implementar proteção contra overflow

3. Debug:
   - Logging só ocorre quando DEBUG_LEVEL > 0
   - Economiza Flash quando não necessário