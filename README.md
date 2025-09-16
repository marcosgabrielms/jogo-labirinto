# Jogo do Labirinto com MPU6050 na BitDogLab

## Resumo

Este é um projeto de jogo de labirinto desenvolvido em C/C++ para o Raspberry Pi Pico, utilizando a placa de desenvolvimento BitDogLab. O jogador, representado por um LED azul em uma matriz de LEDs 5x5, é controlado pelos movimentos de um sensor giroscópio MPU6050. O objetivo é navegar pelo labirinto, evitando as paredes (LEDs vermelhos) e alcançar a saída (LED verde).

O projeto conta com múltiplos níveis que progridem automaticamente e fornece feedback em tempo real ao jogador através de um display OLED, mostrando o nível atual, a posição e a direção do movimento.

## Funcionalidades

- **Controle por Movimento:** Utilize o giroscópio do sensor MPU6050 para mover o jogador para frente, para trás, para a esquerda e para a direita.
- **Display Interativo:** Um display OLED 128x64 exibe informações cruciais do jogo, como nível, posição do jogador e o último movimento realizado.
- **Múltiplos Níveis:** O jogo possui 3 níveis de labirintos pré-definidos, com progressão automática após a conclusão de cada um.
- **Feedback Visual:** A matriz de LEDs 5x5 representa o labirinto, com cores distintas para paredes, caminhos, jogador e a saída.
- **Plataforma BitDogLab:** O projeto é otimizado para a placa BitDogLab, aproveitando seus periféricos embarcados e conectores I2C.

## Hardware e Pinagem

Este projeto foi desenvolvido e testado na placa **BitDogLab** com um Raspberry Pi Pico acoplado. Os periféricos utilizam a seguinte pinagem:

| Periférico | Conexão | Pino(s) |
| :--- | :--- | :--- |
| **Matriz de LEDs** | Sinal (Data) | `GPIO7` |
| **Display OLED** | Barramento I2C 1 | SDA: `GPIO14`, SCL: `GPIO15` |
| **Sensor MPU6050** | Barramento I2C 0 | SDA: `GPIO0`, SCL: `GPIO1` |

## Como Utilizar e Testar

Se você possui uma placa BitDogLab e um sensor MPU6050, siga os passos abaixo para testar o projeto.

### Pré-requisitos

1.  **Ambiente de Desenvolvimento:** É necessário ter o ambiente de desenvolvimento para o Raspberry Pi Pico (Pico SDK) configurado em sua máquina. Siga o guia oficial [Getting started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).
2.  **Hardware:**
    * Placa BitDogLab com Raspberry Pi Pico.
    * Sensor MPU6050 com conector compatível.
    * Cabo Micro-USB.

### Passo a Passo

1.  **Clone o Repositório:**
    ```bash
    git clone [https://github.com/seu-usuario/seu-repositorio.git](https://github.com/seu-usuario/seu-repositorio.git)
    cd seu-repositorio
    ```

2.  **Conecte o Hardware:**
    * Conecte o sensor MPU6050 no conector **I2C0** da placa BitDogLab.

3.  **Configure o CMake:**
    * Crie uma pasta `build` e acesse-a:
      ```bash
      mkdir build
      cd build
      ```
    * Execute o CMake para gerar os arquivos de compilação. Certifique-se de que a variável `PICO_SDK_PATH` está configurada corretamente no seu sistema.
      ```bash
      cmake ..
      ```

4.  **Compile o Projeto:**
    * Dentro da pasta `build`, execute o comando `make`:
      ```bash
      make
      ```

5.  **Carregue o Firmware:**
    * Coloque o seu Raspberry Pi Pico em modo **BOOTSEL**. Para isso, segure o botão `BOOTSEL` na placa e conecte-a ao computador via USB.
    * A placa será montada como um dispositivo de armazenamento.
    * Arraste e solte o arquivo `jogo_labirinto.uf2` (que está na pasta `build`) para dentro do dispositivo de armazenamento do Pico.

A placa irá reiniciar automaticamente e o jogo começará a ser executado. Divirta-se!
