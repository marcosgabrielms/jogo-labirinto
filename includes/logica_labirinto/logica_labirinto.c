#include "logica_labirinto.h"
#include "includes/matriz_LED/matriz_LED.h"

#define LARGURA 5
#define ALTURA 5
#define MAX_NIVEIS 3

static int nivel_atual = 0;
static int jogador_x, jogador_y;
static int objetivo_x, objetivo_y;

// Mapas para os 3 níveis (1=parede, 0=caminho)
static const int mapas[MAX_NIVEIS][ALTURA][LARGURA] = {
    { // Nível 1
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    },
    { // Nível 2
        {0, 0, 0, 1, 0},
        {1, 1, 0, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0}
    },
    { // Nível 3
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    }
};

// Posições iniciais para cada nível {jogador_x, jogador_y, objetivo_x, objetivo_y}
static const int posicoes_niveis[MAX_NIVEIS][4] = {
    {0, 0, 4, 4}, // Nível 1
    {0, 0, 4, 0}, // Nível 2
    {0, 0, 4, 4}  // Nível 3
};

int coord_para_indice(int x, int y) {
    return (y % 2 == 0) ? (y * LARGURA + x) : (y * LARGURA + (LARGURA - 1 - x));
}

void iniciar_nivel(int nivel) {
    if (nivel >= MAX_NIVEIS) return;
    nivel_atual = nivel;
    jogador_x = posicoes_niveis[nivel_atual][0];
    jogador_y = posicoes_niveis[nivel_atual][1];
    objetivo_x = posicoes_niveis[nivel_atual][2];
    objetivo_y = posicoes_niveis[nivel_atual][3];
}

bool mover_jogador(int dx, int dy) {
    int prox_x = jogador_x + dx;
    int prox_y = jogador_y + dy;
    if (prox_x >= 0 && prox_x < LARGURA && prox_y >= 0 && prox_y < ALTURA &&
        mapas[nivel_atual][prox_y][prox_x] == 0) {
        jogador_x = prox_x;
        jogador_y = prox_y;
        return true;
    }
    return false;
}

bool verificar_vitoria() {
    return (jogador_x == objetivo_x && jogador_y == objetivo_y);
}

void desenhar_mapa_atual() {
    npClear();
    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            if (mapas[nivel_atual][y][x] == 1) {
                npSetLED(coord_para_indice(x, y), COR_PAREDE);
            }
        }
    }
    npSetLED(coord_para_indice(objetivo_x, objetivo_y), COR_OBJETIVO);
    npSetLED(coord_para_indice(jogador_x, jogador_y), COR_JOGADOR);
    npWrite();
}

int obter_nivel_atual() { return nivel_atual; }
int obter_max_niveis() { return MAX_NIVEIS; }