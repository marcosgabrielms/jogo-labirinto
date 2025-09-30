#include "pico/stdlib.h"

// Inclusão dos headers de TODOS os seus módulos
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/mpu6050/mpu6050.h"
#include "wifi.h"

// Pinos I2C
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

int main() {
    // 1. Inicializa o básico (USB/UART)
    stdio_init_all();

    // 2. Tenta conectar ao Wi-Fi (sem bloquear o jogo se falhar)
    if (conexao_wifi() != 0) {
        printf("AVISO: Falha ao conectar no Wi-Fi. O envio para a nuvem não funcionará.\n");
    }

    // 3. Inicializa cada um dos seus periféricos e a lógica do jogo
    npInit(LED_PIN);
    inicializacao_display();
    inicializar_mpu6050(i2c0);
    inicializar_jogo(); // Chama a função de preparação do seu logica_labirinto.c

    // 4. Inicia o loop principal do jogo que está em logica_labirinto.c
    // Esta função contém o loop infinito e nunca mais retorna.
    jogo_loop();

    return 0; // Esta linha nunca será alcançada
}