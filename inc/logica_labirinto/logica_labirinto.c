#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/mpu6050/mpu6050.h"
#include <string.h>
#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 

// Reuso do I2C0 da BitDogLab
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

void jogo_loop() {
    int16_t accel[3], gyro[3], temp;
    char Direcao_str[20];
    char Posicao_str[20]; 
    char Nivel_str[20];

    while (true) {
        mpu6050_read_raw(i2c0, accel, gyro, &temp);
        strcpy(Direcao_str, "Parado");
        
        float angle_x_rad = atan2(accel[1], accel[2]);
        float angle_y_rad = atan2(accel[0], accel[2]);

        float angle_x_deg = angle_x_rad * 180.0 / M_PI;
        float angle_y_deg = angle_y_rad * 180.0 / M_PI;

        float angulo_limite = 25.0;
        float abs_angle_x = fabs(angle_x_deg);
        float abs_angle_y = fabs(angle_y_deg);

        // Bias: movimento só conta se eixo dominante for 40% maior
        float bias = 1.4;

        if (abs_angle_x > angulo_limite || abs_angle_y > angulo_limite) {
            
            if (abs_angle_y > abs_angle_x * bias) {
                if (angle_y_deg > 0) {
                    strcpy(Direcao_str, "Para Esquerda");
                    mover_jogador(-1, 0);
                } else {
                    strcpy(Direcao_str, "Para Direita");
                    mover_jogador(1, 0);
                }
            } 
            else if (abs_angle_x > abs_angle_y * bias) {
                if (angle_x_deg > 0) {
                    strcpy(Direcao_str, "Para Tras");
                    mover_jogador(0, -1);
                } else {
                    strcpy(Direcao_str, "Para Frente");
                    mover_jogador(0, 1);
                }
            }
        }

        printf("Angulo X: %.2f, Angulo Y: %.2f, Direcao: %s\n", angle_x_deg, angle_y_deg, Direcao_str);

        sprintf(Nivel_str, "Nivel: %d", nivel);
        sprintf(Posicao_str, "Pos: (%d, %d)", jogador_x, jogador_y);

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
        sleep_ms(250);
    }
}
