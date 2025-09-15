#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/i2c.h"
#include <stdlib.h> // Necessário para a função abs()

#include "includes/matriz_LED/matriz_LED.h"
#include "includes/display_oled/display_oled.h"
#include "includes/mpu6050/mpu6050.h"
#include "includes/logica_labirinto/logica_labirinto.h"

// --- NOVA LÓGICA: MÁQUINA DE ESTADOS DE DIREÇÃO ---
// Usamos um 'enum' para deixar o código mais legível
typedef enum {
    PARADO,
    CIMA,
    BAIXO,
    ESQUERDA,
    DIREITA
} DirecaoMovimento;

void inicializar_hardware_i2c() {
    i2c_init(i2c0, 400 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);
    gpio_pull_up(2);
    gpio_pull_up(3);
}

int main() {
    stdio_init_all();
    
    inicializar_hardware_i2c();
    npInit(LED_PIN);
    inicializacao_display();
    inicializar_acelerometro();
    
    display_mensagem("Calibrando...", "Mantenha o sensor", "PARADO e na", "HORIZONTAL", "", "", "", "", true);
    calibrar_acelerometro();
    display_mensagem("Calibracao", "Concluida!", "", "", "", "", "", "", true);
    sleep_ms(1500);

    inicializar_labirinto();
    display_mensagem("Jogo do Labirinto", "", "Incline para mover", "", "", "", "", "", true);
    sleep_ms(2000);

    // Variável que guarda a direção "travada" do movimento
    DirecaoMovimento direcao_atual = PARADO;

    while (true) {
        int16_t aceleracao[3];
        ler_acelerometro(aceleracao);
        int limiar = 5000;

        // 1. Descobre qual é a NOVA direção que o jogador quer
        DirecaoMovimento direcao_desejada = PARADO;

        // Compara qual eixo tem a inclinação mais forte
        if (abs(aceleracao[1]) > abs(aceleracao[0])) { // Movimento horizontal (Y do sensor) é mais forte
            if (aceleracao[1] > limiar) direcao_desejada = DIREITA;
            else if (aceleracao[1] < -limiar) direcao_desejada = ESQUERDA;
        } else { // Movimento vertical (X do sensor) é mais forte
            if (aceleracao[0] > limiar) direcao_desejada = CIMA;
            else if (aceleracao[0] < -limiar) direcao_desejada = BAIXO;
        }

        // 2. Atualiza a direção "travada" APENAS se o jogador inclinar para um lado novo
        if (direcao_desejada != PARADO) {
            direcao_atual = direcao_desejada;
        }

        // 3. Move o jogador na direção que está "travada"
        if (direcao_atual != PARADO) {
            // A função mover_jogador agora retorna 'false' se bater na parede
            bool conseguiu_mover = false;
            switch (direcao_atual) {
                case DIREITA:  conseguiu_mover = mover_jogador(1, 0); break;
                case ESQUERDA: conseguiu_mover = mover_jogador(-1, 0); break;
                case CIMA:     conseguiu_mover = mover_jogador(0, -1); break;
                case BAIXO:    conseguiu_mover = mover_jogador(0, 1); break;
                case PARADO:   break; // Não faz nada se estiver parado
            }
            // Se bateu na parede, para o movimento
            if (!conseguiu_mover) {
                direcao_atual = PARADO;
            }
        }

        desenhar_labirinto();

        if (verificar_vitoria()) {
            display_mensagem("Parabens!", "Voce venceu!", "", "", "", "", "", "", true);
            sleep_ms(5000);
            inicializar_labirinto();
            direcao_atual = PARADO; // Reseta o movimento ao reiniciar
        }
        sleep_ms(250); // Ajuste a velocidade do movimento aqui
    }
}