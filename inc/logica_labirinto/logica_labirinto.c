#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/mpu6050/mpu6050.h"
#include <string.h>

int main() {
    stdio_init_all();
    npInit(LED_PIN);
    npClear();
    npWrite();
    
    inicializacao_display(); 
    mpu6050_init(i2c1);
    inicializar_jogo();

    int16_t accel[3], gyro[3], temp;
    char Gyro_X_str[20], Gyro_Y_str[20], Direcao_str[20];
    char Posicao_str[20]; 

    while (true) {
        mpu6050_read_raw(i2c1, accel, gyro, &temp);
        strcpy(Direcao_str, "Parado");

        int limite_movimento = 10000;

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

        sprintf(Gyro_X_str, "Gyro X: %d", gyro[0]);
        sprintf(Gyro_Y_str, "Gyro Y: %d", gyro[1]);
        
        sprintf(Posicao_str, "Pos: (%d, %d)", jogador_x, jogador_y);

        display_mensagem("--- DEBUG ---",
                         Gyro_X_str,
                         Gyro_Y_str,
                         Posicao_str,
                         "Rotacao:",
                         Direcao_str,
                         "", "", true);

        desenhar_labirinto();
        verificar_vitoria();
        sleep_ms(150);
    }
    return 0;
}