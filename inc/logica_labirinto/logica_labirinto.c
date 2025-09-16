#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/mpu6050/mpu6050.h"
#include <string.h>

// --- INÍCIO DA CORREÇÃO ---

// Pinos I2C corretos para o conector I2C0 da BitDogLab
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

int main() {
    stdio_init_all();
    npInit(LED_PIN);
    npClear();
    npWrite();
    
    // Inicializa o display no i2c1
    inicializacao_display(); 

    // Inicializa o barramento i2c0 para o MPU6050 nos pinos corretos
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    // Inicializa o sensor MPU6050 no i2c0
    mpu6050_init(i2c0);
    inicializar_jogo();

    int16_t accel[3], gyro[3], temp;
    char Direcao_str[20];
    char Posicao_str[20]; 
    char Nivel_str[20];

    while (true) {
        mpu6050_read_raw(i2c0, accel, gyro, &temp);
        strcpy(Direcao_str, "Parado");

        // Diminuído o limite para aumentar a sensibilidade do movimento
        int limite_movimento = 8000; 

        // Lógica de movimento baseada no giroscópio
        if (gyro[1] > limite_movimento) {
            strcpy(Direcao_str, "Para Tras");
            mover_jogador(0, 1);
        } else if (gyro[1] < -limite_movimento) {
            strcpy(Direcao_str, "Para Frente");
            mover_jogador(0, -1);
        }

        if (gyro[0] > limite_movimento) {
            strcpy(Direcao_str, "Para Direita");
            mover_jogador(1, 0);
        } else if (gyro[0] < -limite_movimento) {
            strcpy(Direcao_str, "Para Esquerda");
            mover_jogador(-1, 0);
        }

        // Prepara as strings para exibição no display
        sprintf(Nivel_str, "Nivel: %d", nivel);
        sprintf(Posicao_str, "Pos: (%d, %d)", jogador_x, jogador_y);

        // Exibe no display as informações do jogo
        display_mensagem("JOGO LABIRINTO",
                         Nivel_str,
                         "",
                         Posicao_str,
                         "",
                         "Movimento:",
                         Direcao_str, 
                         "", 
                         true);

        desenhar_labirinto();
        verificar_vitoria();
        sleep_ms(150);
    }
    return 0;
}