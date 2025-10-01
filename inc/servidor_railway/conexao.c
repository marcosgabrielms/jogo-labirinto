#include "conexao.h"
#include <string.h> // Adicionado para strstr

// --- ALTERAÇÃO INICIADA ---
// Callback para a resposta do servidor modificado para verificar a resposta HTTP
static err_t callback_resposta_recebida(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    if (!p) {
        printf("DIAGNOSTICO: Conexao fechada pelo servidor.\n");
        tcp_close(pcb);
        return ERR_OK;
    }

    printf("DIAGNOSTICO: Resposta do servidor recebida:\n---\n%.*s\n---\n", p->len, (char *)p->payload);

    if (strstr((char *)p->payload, "HTTP/1.1 200 OK") != NULL) {
        printf("DIAGNOSTICO: SUCESSO! Servidor confirmou o recebimento dos dados.\n");
    } else {
        printf("DIAGNOSTICO: AVISO! Resposta inesperada do servidor.\n");
    }

    pbuf_free(p);
    tcp_close(pcb);
    return ERR_OK;
}
// --- ALTERAÇÃO FINALIZADA ---

// --- ALTERAÇÃO INICIADA ---
// Callback de conexão modificado para melhor tratamento de erros e liberação de memória
static err_t callback_conectado(void *arg, struct tcp_pcb *pcb, err_t err) {
    DadosJogoCompleto *dados_recebidos = (DadosJogoCompleto *)arg;
    if (err != ERR_OK) {
        printf("DIAGNOSTICO: Falha ao conectar no socket TCP. Erro LwIP: %d\n", err);
        tcp_abort(pcb);
        if (dados_recebidos) {
            free(dados_recebidos);
        }
        return err;
    }
    printf("DIAGNOSTICO: Conexao TCP estabelecida. Enviando dados...\n");
    tcp_recv(pcb, callback_resposta_recebida);

    char labirinto_str[256] = "";
    char temp_str[8];
    strcat(labirinto_str, "[");
    for (int y = 0; y < MATRIZ_LINHA; y++) {
        strcat(labirinto_str, "[");
        for (int x = 0; x < MATRIZ_COLUNA; x++) {
            sprintf(temp_str, "%d", dados_recebidos->labirinto[y][x]);
            strcat(labirinto_str, temp_str);
            if (x < MATRIZ_COLUNA - 1) strcat(labirinto_str, ",");
        }
        strcat(labirinto_str, "]");
        if (y < MATRIZ_LINHA - 1) strcat(labirinto_str, ",");
    }
    strcat(labirinto_str, "]");

    char corpo_json[1024];
    snprintf(corpo_json, sizeof(corpo_json),
             "{\"labirinto\": %s, \"jogador_x\": %d, \"jogador_y\": %d, \"nivel\": %d, \"accel_x\": %d, \"accel_y\": %d}",
             labirinto_str, dados_recebidos->jogador_x, dados_recebidos->jogador_y, dados_recebidos->nivel,
             dados_recebidos->accel_x, dados_recebidos->accel_y);
    
    char requisicao[1536];
    snprintf(requisicao, sizeof(requisicao),
             "POST /dados HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n\r\n%s",
             PROXY_HOST, strlen(corpo_json), corpo_json);
             
    printf("DIAGNOSTICO: Enviando requisicao HTTP:\n---\n%s\n---\n", requisicao);

    cyw43_arch_lwip_begin();
    err_t write_err = tcp_write(pcb, requisicao, strlen(requisicao), TCP_WRITE_FLAG_COPY);
    if (write_err == ERR_OK) {
        tcp_output(pcb);
    } else {
        printf("DIAGNOSTICO: Erro ao escrever para o socket TCP. Erro LwIP: %d\n", write_err);
    }
    cyw43_arch_lwip_end();
    
    if (dados_recebidos) {
        free(dados_recebidos);
    }
    return ERR_OK;
}
// --- ALTERAÇÃO FINALIZADA ---

// --- ALTERAÇÃO INICIADA ---
// Função de envio de dados modificada para corrigir potencial vazamento de memória
void enviar_dados_para_nuvem(const DadosJogoCompleto *dados_a_enviar) {
    DadosJogoCompleto* copia_dados = (DadosJogoCompleto*)malloc(sizeof(DadosJogoCompleto));
    if (!copia_dados) {
        printf("DIAGNOSTICO: Erro de alocacao de memoria para copia_dados.\n");
        return;
    }
    memcpy(copia_dados, dados_a_enviar, sizeof(DadosJogoCompleto));

    printf("DIAGNOSTICO: Tentando conectar ao servidor em %s:%d\n", PROXY_HOST, PROXY_PORT);

    ip_addr_t ip_servidor;
    ipaddr_aton(PROXY_HOST, &ip_servidor); 

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb) {
        printf("DIAGNOSTICO: Nao foi possivel criar o pcb TCP.\n");
        free(copia_dados);
        return;
    }
    
    tcp_arg(pcb, copia_dados);
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(pcb, &ip_servidor, PROXY_PORT, callback_conectado);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("DIAGNOSTICO: Erro imediato ao chamar tcp_connect. Erro LwIP: %d\n", err);
        tcp_abort(pcb);
        if (copia_dados) {
            free(copia_dados);
        }
    }
}
// --- ALTERAÇÃO FINALIZADA ---