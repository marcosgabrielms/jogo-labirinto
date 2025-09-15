#ifndef LOGICA_LABIRINTO_H
#define LOGICA_LABIRINTO_H

#include <stdbool.h>

#define COR_JOGADOR  0, 0, 100
#define COR_PAREDE  100, 0, 0
#define COR_OBJETIVO 0, 100, 0

void iniciar_nivel(int nivel);
bool mover_jogador(int dx, int dy);
bool verificar_vitoria();
void desenhar_mapa_atual();
int obter_nivel_atual();
int obter_max_niveis();

#endif