#include "conexao.h"

// Callback para a resposta do servidor
static err_t callback_resposta_recebida(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    if (!p) {
        tcp_close(pcb);
        return ERR_OK;
    }
    pbuf_free(p);
    return ERR_OK;
}

// Callback para quando a conexão TCP é estabelecida
static err_t callback_conectado(void *arg, struct tcp_pcb *pcb, err_t err) {
    DadosJogoCompleto *dados_recebidos = (DadosJogoCompleto *)arg;
    if (err != ERR_OK) {
        printf("DIAGNOSTICO: Falha ao conectar no socket TCP. Erro LwIP: %d\n", err);
        tcp_abort(pcb);
        free(dados_recebidos);
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

    cyw43_arch_lwip_begin();
    tcp_write(pcb, requisicao, strlen(requisicao), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);
    cyw43_arch_lwip_end();
    
    free(dados_recebidos);
    return ERR_OK;
}

// Função principal de envio de dados
void enviar_dados_para_nuvem(const DadosJogoCompleto *dados_a_enviar) {
    DadosJogoCompleto* copia_dados = (DadosJogoCompleto*)malloc(sizeof(DadosJogoCompleto));
    if (!copia_dados) return;
    memcpy(copia_dados, dados_a_enviar, sizeof(DadosJogoCompleto));

    printf("DIAGNOSTICO: Tentando conectar ao servidor em %s:%d\n", PROXY_HOST, PROXY_PORT);

    ip_addr_t ip_servidor;
    ipaddr_aton(PROXY_HOST, &ip_servidor); 

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_V4);
    if (!pcb) {
        free(copia_dados);
        return;
    }
    
    tcp_arg(pcb, copia_dados);
    cyw43_arch_lwip_begin();
    err_t err = tcp_connect(pcb, &ip_servidor, PROXY_PORT, callback_conectado);
    cyw43_arch_lwip_end();

    if (err != ERR_OK) {
        printf("DIAGNOSTICO: Erro imediato ao chamar tcp_connect. Erro LwIP: %d\n", err);
    }
}