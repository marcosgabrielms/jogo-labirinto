#include "inc/logica_labirinto/logica_labirinto.h"

int nivel = 1;
int jogador_x = 1, jogador_y = 1;
int chegada_x, chegada_y;

const int labirinto_nivel_1[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 1, 1, 2, 1}
};

const int labirinto_nivel_2[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 2, 1, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1}
};

const int labirinto_nivel_3[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 2},
    {1, 1, 1, 1, 1}
};

const int (*labirintos[3])[MATRIZ_LINHA][MATRIZ_COLUNA] = {&labirinto_nivel_1, &labirinto_nivel_2, &labirinto_nivel_3};

void carregar_nivel() {
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];
    for (int y = 0; y < MATRIZ_LINHA; y++) {
        for (int x = 0; x < MATRIZ_COLUNA; x++) {
            if ((*labirinto)[y][x] == 2) {
                chegada_x = x;
                chegada_y = y;
            }
        }
    }
    jogador_x = 1;
    jogador_y = 1;
}

void inicializar_jogo() {
    display_mensagem("Inicializando", "jogo", "", "", "", "", "", "", true);
    sleep_ms(2000);
    carregar_nivel();
    display_mensagem("Jogo iniciado", "", "", "", "", "", "", "", true);
}

void mover_jogador(int dx, int dy) {
    int novo_x = jogador_x + dx;
    int novo_y = jogador_y + dy;
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];

    if (novo_x >= 0 && novo_x < MATRIZ_COLUNA && novo_y >= 0 && novo_y < MATRIZ_LINHA && (*labirinto)[novo_y][novo_x] != 1) {
        jogador_x = novo_x;
        jogador_y = novo_y;
    }
}

// CORREÇÃO: Função de mapeamento simplificada para um grid padrão
int pegar_indice_LED(int x, int y) {
    // Garante que o cálculo não saia dos limites do array
    if (x >= 0 && x < MATRIZ_COLUNA && y >= 0 && y < MATRIZ_LINHA) {
        return y * MATRIZ_COLUNA + x;
    }
    return 0; // Retorna um valor seguro caso as coordenadas sejam inválidas
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
        }
    }
    npSetLED(pegar_indice_LED(jogador_x, jogador_y), 0, 0, 100); // Jogador (azul)
    npWrite();
}

void verificar_vitoria() {
    if (jogador_x == chegada_x && jogador_y == chegada_y) {
        display_mensagem("Voce chegou,", "parabens!", "", "", "", "", "", "", true);
        sleep_ms(3000);
        nivel++;
        if (nivel > 3) {
            nivel = 1;
        }
        inicializar_jogo();
    }
}