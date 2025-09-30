#ifndef CONEXAO_H
#define CONEXAO_H

#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "../logica_labirinto/logica_labirinto.h"

// --- CONFIGURAÇÃO DO SERVIDOR LOCAL ---
// O IP do seu computador (verificado com 'ipconfig')
#define PROXY_HOST "192.168.134.127" 
// A porta em que o seu servidor Flask está rodando
#define PROXY_PORT 5000

// Estrutura para os dados completos do jogo a serem enviados
typedef struct {
    int labirinto[MATRIZ_LINHA][MATRIZ_COLUNA];
    int jogador_x;
    int jogador_y;
    int nivel;
    int16_t accel_x;
    int16_t accel_y;
} DadosJogoCompleto;

// Função que envia os dados para o servidor
void enviar_dados_para_nuvem(const DadosJogoCompleto *dados_a_enviar);

#endif