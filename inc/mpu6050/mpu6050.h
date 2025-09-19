#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define MPU6050_ADDR 0x68
#define MPU6050_SDA_PIN 0
#define MPU6050_SCL_PIN 1

#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_PWR_MGMT_1 0x6B

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} mpu6050_accel_data_t;

void inicializar_mpu6050(i2c_inst_t *i2c_instance);
void leitura_acelero_mpu6050(i2c_inst_t *i2c_instance, mpu6050_accel_data_t *accel_data);

#endif