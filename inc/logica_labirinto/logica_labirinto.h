#ifndef LOGICA_LABIRINTO_H
#define LOGICA_LABIRINTO_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"

#define MATRIZ_LINHA 5
#define MATRIZ_COLUNA 5

extern int nivel;
extern int jogador_x, jogador_y;

void inicializar_jogo();
void carregar_nivel();
void mover_jogador(int dx, int dy);
void desenhar_labirinto();
void verificar_vitoria();

#endif