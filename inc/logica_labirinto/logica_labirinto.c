#include "pico/cyw43_arch.h"
#include <string.h>
#include <stdio.h>

#include "inc/logica_labirinto/logica_labirinto.h"
#include "inc/matriz_LED/matriz_LED.h"
#include "inc/display_oled/display_oled.h"
#include "inc/mpu6050/mpu6050.h"
#include "inc/servidor_railway/conexao.h"

int nivel = 1;
int jogador_x = 1, jogador_y = 0;
int chegada_x, chegada_y;

const int labirinto_nivel_1[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 3, 1, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 1}, 
    {1, 1, 1, 2, 1}};
const int labirinto_nivel_2[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {3, 0, 0, 0, 1}, 
    {1, 1, 1, 0, 1}, 
    {1, 0, 0, 0, 1}, 
    {1, 0, 1, 1, 1}, 
    {1, 0, 0, 0, 2}};
const int labirinto_nivel_3[MATRIZ_LINHA][MATRIZ_COLUNA] = {
    {1, 1, 1, 1, 2}, 
    {1, 0, 1, 1, 0}, 
    {1, 0, 0, 0, 0}, 
    {3, 0, 1, 0, 1}, 
    {1, 1, 1, 1, 1}};
    
const int (*labirintos[3])[MATRIZ_LINHA][MATRIZ_COLUNA] = {&labirinto_nivel_1, &labirinto_nivel_2, &labirinto_nivel_3};

int pegar_indice_LED(int x, int y)
{
    if (y % 2 != 0)
    {
        return y * MATRIZ_COLUNA + (MATRIZ_COLUNA - 1 - x);
    }
    else
    {
        return y * MATRIZ_COLUNA + x;
    }
}

void carregar_nivel()
{
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];
    for (int y = 0; y < MATRIZ_LINHA; y++)
    {
        for (int x = 0; x < MATRIZ_COLUNA; x++)
        {
            if ((*labirinto)[y][x] == 2)
            {
                chegada_x = x;
                chegada_y = y;
            }
            if ((*labirinto)[y][x] == 3)
            {
                jogador_x = x;
                jogador_y = y;
            }
        }
    }
}

void inicializar_jogo()
{
    display_mensagem("JOGO LABIRINTO", "Prepare-se...", "", "", "", "", "", "", true);
    sleep_ms(2000);
    carregar_nivel();
}

void mover_jogador(int dx, int dy)
{
    int novo_x = jogador_x + dx;
    int novo_y = jogador_y + dy;
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];

    if (novo_x >= 0 && novo_x < MATRIZ_COLUNA && novo_y >= 0 && novo_y < MATRIZ_LINHA && (*labirinto)[novo_y][novo_x] != 1)
    {
        jogador_x = novo_x;
        jogador_y = novo_y;
    }
}

void desenhar_labirinto()
{
    npClear();
    const int (*labirinto)[MATRIZ_LINHA][MATRIZ_COLUNA] = labirintos[nivel - 1];
    for (int y = 0; y < MATRIZ_LINHA; y++)
    {
        for (int x = 0; x < MATRIZ_COLUNA; x++)
        {
            int indice = pegar_indice_LED(x, y);
            if ((*labirinto)[y][x] == 1)
            {
                npSetLED(indice, 100, 0, 0);
            }
            else if ((*labirinto)[y][x] == 2)
            {
                npSetLED(indice, 0, 100, 0);
            }
        }
    }
    npSetLED(pegar_indice_LED(jogador_x, jogador_y), 0, 0, 100);
    npWrite();
}

void verificar_vitoria()
{
    if (jogador_x == chegada_x && jogador_y == chegada_y)
    {
        display_mensagem("Voce venceu!", "Parabens!", "", "Proximo nivel...", "", "", "", "", true);
        sleep_ms(3000);
        nivel++;
        if (nivel > 3)
        {
            nivel = 1;
            display_mensagem("Parabens!", "Voce completou", "todos os niveis!", "Reiniciando...", "", "", "", "", true);
            sleep_ms(3000);
        }
        carregar_nivel();
    }
}

static absolute_time_t ultimo_movimento_mpu;
#define MPU_MOVE_DELAY_US 300000
#define MPU_TILT_THRESHOLD 5000

// --- ALTERAÇÃO INICIADA ---
// Adicionado para controlar a frequência de envio para a nuvem
static absolute_time_t ultimo_envio_nuvem;
#define NUVEM_SEND_INTERVAL_MS 2000 // Enviar a cada 2 segundos
// --- ALTERAÇÃO FINALIZADA ---

void jogo_loop()
{
    ultimo_movimento_mpu = get_absolute_time();
    ultimo_envio_nuvem = get_absolute_time(); // Inicializa o timer

    char Direcao_str[20], Posicao_str[20], Nivel_str[20];

    mpu6050_accel_data_t accel_data;
    DadosJogoCompleto dados_completos;

    while (true)
    {
        cyw43_arch_poll();

        leitura_acelero_mpu6050(i2c0, &accel_data);
        strcpy(Direcao_str, "Parado");
        absolute_time_t agora = get_absolute_time();

        if (absolute_time_diff_us(ultimo_movimento_mpu, agora) > MPU_MOVE_DELAY_US)
        {
            int dx = 0, dy = 0;
            if (accel_data.y > MPU_TILT_THRESHOLD) { dx = 1; strcpy(Direcao_str, "Direita"); }
            else if (accel_data.y < -MPU_TILT_THRESHOLD) { dx = -1; strcpy(Direcao_str, "Esquerda"); }
            else if (accel_data.x > MPU_TILT_THRESHOLD) { dy = 1; strcpy(Direcao_str, "Frente"); }
            else if (accel_data.x < -MPU_TILT_THRESHOLD) { dy = -1; strcpy(Direcao_str, "Tras"); }
            if (dx != 0 || dy != 0)
            {
                mover_jogador(dx, dy);
                ultimo_movimento_mpu = agora;
            }
        }

        sprintf(Nivel_str, "Nivel: %d", nivel);
        sprintf(Posicao_str, "Pos: (%d, %d)", jogador_x, jogador_y);
        display_mensagem("JOGO LABIRINTO", Nivel_str, Posicao_str, "Movimento:", Direcao_str, "", "", "", true);

        desenhar_labirinto();
        verificar_vitoria();

        // --- ALTERAÇÃO INICIADA ---
        // Prepara e envia o estado completo do jogo para o servidor (com controle de tempo)
        if (absolute_time_diff_us(ultimo_envio_nuvem, agora) / 1000 > NUVEM_SEND_INTERVAL_MS)
        {
            dados_completos.jogador_x = jogador_x;
            dados_completos.jogador_y = jogador_y;
            dados_completos.nivel = nivel;
            dados_completos.accel_x = accel_data.x;
            dados_completos.accel_y = accel_data.y;
            memcpy(dados_completos.labirinto, *labirintos[nivel - 1], sizeof(dados_completos.labirinto));
            
            enviar_dados_para_nuvem(&dados_completos);
            ultimo_envio_nuvem = agora; // Reseta o timer
        }
        // --- ALTERAÇÃO FINALIZADA ---

        sleep_ms(100);
    }
}