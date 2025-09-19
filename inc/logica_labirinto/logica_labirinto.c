#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/mpu6050/mpu6050.h"
#include <string.h>
#include <stdio.h>

// --- Variáveis Globais do Jogo ---
int nivel = 1;
int jogador_x = 1, jogador_y = 0; // Posição inicial do jogador
int chegada_x, chegada_y;         // Posição do objetivo

// --- Definições dos Mapas dos Labirintos ---
// 0 = Caminho livre, 1 = Parede, 2 = Chegada, 3 = Ponto de Partida
const int labirinto_nivel_1[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 3, 1, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 2, 1}
};

const int labirinto_nivel_2[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {3, 0, 0, 0, 1},
    {1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 1, 1},
    {1, 0, 0, 0, 2}
};

const int labirinto_nivel_3[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 1, 1, 1, 2},
    {1, 0, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {3, 0, 1, 0, 1},
    {1, 1, 1, 1, 1}
};

// Ponteiro para os labirintos
const int (*labirintos[3])[MATRIZ_LINHA][MATRIZ_COLUNA] = {&labirinto_nivel_1, &labirinto_nivel_2, &labirinto_nivel_3};

// --- Funções de Lógica do Jogo ---

int pegar_indice_LED(int x, int y) {
    if (y % 2 != 0) {
        // Linha ímpar (1, 3): a contagem de X é da direita para a esquerda
        return y * MATRIZ_COLUNA + (MATRIZ_COLUNA - 1 - x);
    } else {
        // Linha par (0, 2, 4): a contagem de X é normal, da esquerda para a direita
        return y * MATRIZ_COLUNA + x;
    }
}

void carregar_nivel() {
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];
    for (int y = 0; y < MATRIZ_LINHA; y++) {
        for (int x = 0; x < MATRIZ_COLUNA; x++) {
            if ((*labirinto)[y][x] == 2) { // Encontra a chegada
                chegada_x = x;
                chegada_y = y;
            }
            if ((*labirinto)[y][x] == 3) { // Encontra a partida
                jogador_x = x;
                jogador_y = y;
            }
        }
    }
}

void inicializar_jogo() {
    display_mensagem("JOGO LABIRINTO", "Prepare-se...", "", "", "", "", "", "", true);
    sleep_ms(2000);
    carregar_nivel();
}

void mover_jogador(int dx, int dy) {
    int novo_x = jogador_x + dx;
    int novo_y = jogador_y + dy;
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];

    // Verifica se a nova posição está dentro do mapa e não é uma parede
    if (novo_x >= 0 && novo_x < MATRIZ_COLUNA && novo_y >= 0 && novo_y < MATRIZ_LINHA && (*labirinto)[novo_y][novo_x] != 1) {
        jogador_x = novo_x;
        jogador_y = novo_y;
    }
}

void desenhar_labirinto() {
    npClear();
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];
    for (int y = 0; y < MATRIZ_LINHA; y++) {
        for (int x = 0; x < MATRIZ_COLUNA; x++) {
            int indice = pegar_indice_LED(x, y);
            if ((*labirinto)[y][x] == 1) {
                npSetLED(indice, 100, 0, 0); // Parede (vermelho)
            } else if ((*labirinto)[y][x] == 2) {
                npSetLED(indice, 0, 100, 0); // Chegada (verde)
            }
            // Caminhos e ponto de partida ficam apagados
        }
    }
    npSetLED(pegar_indice_LED(jogador_x, jogador_y), 0, 0, 100); // Jogador (azul)
    npWrite();
}

void verificar_vitoria() {
    if (jogador_x == chegada_x && jogador_y == chegada_y) {
        display_mensagem("Voce venceu!", "Parabens!", "", "Proximo nivel...", "", "", "", "", true);
        sleep_ms(3000);
        nivel++;
        if (nivel > 3) { // Se passar do último nível, volta para o primeiro
            nivel = 1;
            display_mensagem("Parabens!", "Voce completou", "todos os niveis!", "Reiniciando...", "", "", "", "", true);
            sleep_ms(3000);
        }
        carregar_nivel(); // Carrega o próximo nível
    }
}

// --- Loop Principal do Jogo ---

// Constantes para o controle do MPU
static absolute_time_t ultimo_movimento_mpu;
#define MPU_MOVE_DELAY_US 300000 // Delay para 300ms para dar mais controle
#define MPU_TILT_THRESHOLD 5000  // Sensibilidade da inclinação

void jogo_loop() {
    ultimo_movimento_mpu = get_absolute_time();
    
    char Direcao_str[20];
    char Posicao_str[20]; 
    char Nivel_str[20];

    while (true) {
        mpu6050_accel_data_t accel;
        leitura_acelero_mpu6050(i2c0, &accel); // Usa a função do driver MPU
        strcpy(Direcao_str, "Parado");

        absolute_time_t agora = get_absolute_time();

        // Lógica de movimento baseada na inclinação
        if (absolute_time_diff_us(ultimo_movimento_mpu, agora) > MPU_MOVE_DELAY_US) {
            int dx = 0;
            int dy = 0;

            // Inclinar para a direita/esquerda
            if (accel.x > MPU_TILT_THRESHOLD) { // X positivo = inclinar para esquerda
                strcpy(Direcao_str, "Esquerda");
                dx = 1;
            } else if (accel.x < -MPU_TILT_THRESHOLD) { // X negativo = inclinar para direita
                strcpy(Direcao_str, "Direita");
                dx = -1;
            }
            // Inclinar para frente/trás
            else if (accel.y > MPU_TILT_THRESHOLD) { // Y positivo = inclinar para frente
                strcpy(Direcao_str, "Tras");
                dy = -1; 
            } else if (accel.y < -MPU_TILT_THRESHOLD) { // Y negativo = inclinar para trás
                strcpy(Direcao_str, "Frente");
                dy = 1; 
            }

            if (dx != 0 || dy != 0) {
                mover_jogador(dx, dy);
                ultimo_movimento_mpu = agora;
            }
        }

        // Atualiza display e matriz de LEDs
        sprintf(Nivel_str, "Nivel: %d", nivel);
        sprintf(Posicao_str, "Pos: (%d, %d)", jogador_x, jogador_y);
        display_mensagem("JOGO LABIRINTO", Nivel_str, Posicao_str, "Movimento:", Direcao_str, "", "", "", true);

        desenhar_labirinto();
        verificar_vitoria();
        sleep_ms(100); // Pequeno delay no loop
    }
}