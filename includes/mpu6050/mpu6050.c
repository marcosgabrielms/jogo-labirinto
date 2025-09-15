#include "mpu6050.h"
#include <stdio.h>

static const uint8_t ADDR = 0x68;
static const uint8_t REG_PWR_MGMT_1 = 0x6B;
static const uint8_t REG_ACCEL_XOUT_H = 0x3B;

static int16_t offset_x = 0;
static int16_t offset_y = 0;

void inicializar_mpu6050() {
    uint8_t buf[] = {REG_PWR_MGMT_1, 0x00};
    i2c_write_blocking(i2c1, ADDR, buf, 2, false);
}

void calibrar_mpu6050() {
    int32_t acc_x = 0, acc_y = 0;
    const int leituras = 1000;

    for (int i = 0; i < leituras; i++) {
        uint8_t buffer[6];
        uint8_t val = REG_ACCEL_XOUT_H;
        i2c_write_blocking(i2c1, ADDR, &val, 1, true);
        i2c_read_blocking(i2c1, ADDR, buffer, 6, false);
        
        acc_x += (int16_t)(buffer[0] << 8 | buffer[1]);
        acc_y += (int16_t)(buffer[2] << 8 | buffer[3]);
        sleep_ms(2);
    }
    offset_x = acc_x / leituras;
    offset_y = acc_y / leituras;
}

void ler_mpu6050(int16_t *aceleracao) {
    uint8_t buffer[6];
    uint8_t val = REG_ACCEL_XOUT_H;
    i2c_write_blocking(i2c1, ADDR, &val, 1, true);
    i2c_read_blocking(i2c1, ADDR, buffer, 6, false);

    aceleracao[0] = (int16_t)(buffer[0] << 8 | buffer[1]) - offset_x;
    aceleracao[1] = (int16_t)(buffer[2] << 8 | buffer[3]) - offset_y;
    aceleracao[2] = (int16_t)(buffer[4] << 8 | buffer[5]);
}