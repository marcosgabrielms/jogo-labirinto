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

    // Inicializa matriz de LEDs
    npInit(LED_PIN);
    npClear();
    npWrite();

    // Inicializa display OLED
    inicializacao_display();

    // Inicializa I2C
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    // Inicializa MPU6050
    mpu6050_init(i2c0);

    // Inicializa jogo
    inicializar_jogo();

    // Loop principal do jogo
    jogo_loop();

    return 0;
}
