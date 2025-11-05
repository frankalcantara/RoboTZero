#ifndef CONFIG_H
#define CONFIG_H

// ====== Debugging ======
// Ative (1) para ver informações no monitor serial ou desative (0) para melhor performance
#define DEBUG_LEVEL 0

// ====== Pinos ======
// Altere apenas se modificar as conexões físicas do robô
#define PINBUTTON 11      // Botão para iniciar/calibrar
#define PINLED 13         // LED indicador de status
#define AIN1 7           // Direção Motor Esquerdo
#define AIN2 4           // Direção Motor Esquerdo
#define PWMA 3           // Velocidade Motor Esquerdo
#define BIN1 8           // Direção Motor Direito
#define BIN2 9           // Direção Motor Direito
#define PWMB 10          // Velocidade Motor Direito

// Tempo de debounce do botão - Aumente se estiver detectando múltiplos toques
#define DEBOUNCE_DELAY 50  // Em milissegundos

// Pinos dos sensores - Altere apenas se modificar as conexões físicas
#define SENSOR_1 A6      // Sensor mais à esquerda
#define SENSOR_2 A5
#define SENSOR_3 A4
#define SENSOR_4 A3
#define SENSOR_5 A2
#define SENSOR_6 A1      // Sensor mais à direita
#define SENSOR_HIT_L A7  // Marcador esquerdo
#define SENSOR_HIT_R A0  // Marcador direito

// ====== Velocidades Predefinidas ======
// Ajuste estas velocidades conforme a potência dos seus motores e peso do robô
#define VELOCIDADE_0 0    // Parado
#define VELOCIDADE_1 80   // Aumente se o robô não iniciar movimento (25-75)
#define VELOCIDADE_2 100  // Ajuste para curvas suaves (75-125)
#define VELOCIDADE_3 120  // Velocidade de frenagem - Aumente se parar bruscamente
#define VELOCIDADE_4 140  // Velocidade média - Ajuste baseado na pista
#define VELOCIDADE_5 160  // Modo lento - Aumente se muito lento (170-220)
#define VELOCIDADE_6 180  // Velocidade alta - Ajuste conforme estabilidade
#define VELOCIDADE_7 200  // Velocidade muito alta - Use com cautela
#define VELOCIDADE_8 220  // Máxima - Não alterar

// ====== Parâmetros de Controle de Velocidade ======
// Aumente ACEL_STEP para saída mais rápida de curvas (20-40)
#define ACEL_STEP 25          
// Aumente FREIO_STEP para frenagem mais forte nas curvas (30-50)
#define FREIO_STEP 60         
// Diminua CURVA_VEL se derrapar nas curvas (100-140)
#define CURVA_VEL 120         
// Aumente CURVA_ERRO se detectar curvas muito cedo (40-60)
#define CURVA_ERRO 45         
// Diminua RETA_ERRO se demorar para acelerar após curvas (15-25)
#define RETA_ERRO 20          
// Aumente BOOST_TEMPO para boost mais longo após curvas (5-15)
#define BOOST_TEMPO 10        
// Aumente BOOST_VALOR para aceleração mais agressiva (20-40)
#define BOOST_VALOR 20        

// ====== Parâmetros de Controle PID ======
// Aumente KPROP para correção mais agressiva (5.0-9.0)
#define KPROP_DEFAULT 5.0f
// Diminua KDERIV se oscilar muito nas retas (200-400)
#define KDERIV_DEFAULT 600.0f  
// Aumente ALPHA para resposta mais rápida (0.6-0.9)
#define ALPHA_DEFAULT 0.6f     

// Posição central desejada (não alterar sem recalibrar todo o controle)
#define POSICION_IDEAL_DEFAULT 0

// ====== Parâmetros dos Marcadores ======
// Ajuste UMBRAL conforme a refletância da sua marcação (150-200)
#define UMBRAL 180
// Velocidades base - Ajuste SLOW se muito lento em modo lento
#define BASE_SLOW 110 //110
// BASE_FAST é máximo - não alterar
#define BASE_FAST 110 // original 110, testado 160, testado 180

// ====== Delays e Tempos ======
// Aumente SETUP_DELAY se iniciação muito rápida
#define SETUP_DELAY 600       
// Mais amostras = calibração mais precisa, mas mais lenta (300-500)
#define CALIBRATION_SAMPLES 400 
// Menor delay = calibração mais rápida, mas pode ter ruído (20-40)
#define CALIBRATION_DELAY 30   
// Tempo até parar após marcador final - ajuste conforme pista
#define STOP_DELAY 300      

// ====== Parâmetros dos Sensores ======
// Não alterar sem modificar todo o código
#define NUM_SENSORES 6        
#define SENSOR_MAX_VALUE 1023 
#define SENSOR_MIN_VALUE 0    
// Aumente se não detectar linha bem (100-150)
#define SENSOR_THRESHOLD 120  

// ====== Pesos dos Sensores ======
// Aumente valores para correções mais agressivas nas bordas
// Diminua para movimento mais suave
// Mantenha a proporção entre os valores ao ajustar
#define PESO_S1 -2.5f  // Extrema esquerda (1.2-1.8)
#define PESO_S2 -1.2f  // Esquerda (0.9-1.5)
#define PESO_S3 -0.6f  // Centro-esquerda (0.4-0.8)
#define PESO_S4 0.6f   // Centro-direita (0.4-0.8)
#define PESO_S5 1.2f   // Direita (0.9-1.5)
#define PESO_S6 2.5f   // Extrema direita (1.2-1.8)

#endif // CONFIG_H