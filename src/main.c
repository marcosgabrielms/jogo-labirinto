#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/mpu6050/mpu6050.h"

// Pinos I2C corretos para o conector I2C0 da BitDogLab
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

int main() {
    stdio_init_all();

    // Inicializa a matriz de LEDs
    npInit(LED_PIN);

    // Inicializa o display OLED
    inicializacao_display();

    // Inicializa o sensor MPU6050 (que por sua vez inicializa o I2C)
    inicializar_mpu6050(i2c0);

    // Inicializa as variáveis do jogo
    inicializar_jogo();

    // Inicia o loop principal do jogo
    jogo_loop();

    return 0;
}