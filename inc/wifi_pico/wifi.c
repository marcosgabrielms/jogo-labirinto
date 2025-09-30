#include "wifi.h"
#include "pico/cyw43_arch.h"
#include <stdio.h> // Adicionado para usar printf

int conexao_wifi() {
    printf("DIAGNOSTICO: Inicializando Wi-Fi...\n");
    if (cyw43_arch_init()) {
        printf("DIAGNOSTICO: ERRO FATAL ao inicializar modulo Wi-Fi.\n");
        return -1;
    }
    
    cyw43_arch_enable_sta_mode();
    printf("DIAGNOSTICO: Tentando conectar a rede Wi-Fi '%s'...\n", NOME_WIFI);

    // Tenta conectar por 30 segundos
    int status = cyw43_arch_wifi_connect_timeout_ms(NOME_WIFI, SENHA_WIFI, CYW43_AUTH_WPA2_AES_PSK, 30000);

    if (status == 0) {
        printf("DIAGNOSTICO: SUCESSO! Wi-Fi conectado.\n");
        printf("DIAGNOSTICO: IP da Placa: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    } else {
        printf("DIAGNOSTICO: ERRO! Falha ao conectar no Wi-Fi. Codigo de erro: %d\n", status);
    }
    
    return status;
}