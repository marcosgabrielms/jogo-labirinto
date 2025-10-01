#ifndef CONEXAO_H
#define CONEXAO_H

#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "../logica_labirinto/logica_labirinto.h"

// --- ALTERAÇÃO INICIADA ---
// !!! ATENÇÃO !!!
// Altere o IP abaixo para o endereço IP do computador onde o servidor local (API) está a ser executado.
// Pode encontrar este IP executando o comando 'ipconfig' (Windows) ou 'ifconfig'/'ip addr' (Linux/macOS) no terminal.
#define PROXY_HOST "192.168.3.11" 
// --- ALTERAÇÃO FINALIZADA ---
#define PROXY_PORT 5000

typedef struct {
    int labirinto[MATRIZ_LINHA][MATRIZ_COLUNA];
    int jogador_x;
    int jogador_y;
    int nivel;
    int16_t accel_x;
    int16_t accel_y;
} DadosJogoCompleto;

void enviar_dados_para_nuvem(const DadosJogoCompleto *dados_a_enviar);

#endif