#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// Definições dos Registradores do MPU6050
#define MPU6050_ADDR         _u(0x68)
#define MPU6050_SMPLRT_DIV   _u(0x19)
#define MPU6050_CONFIG       _u(0x1A)
#define MPU6050_GYRO_CONFIG  _u(0x1B)
#define MPU6050_ACCEL_CONFIG _u(0x1C)
#define MPU6050_ACCEL_XOUT_H _u(0x3B)
#define MPU6050_PWR_MGMT_1   _u(0x6B)

void mpu6050_init(i2c_inst_t *i2c);
void mpu6050_read_raw(i2c_inst_t *i2c, int16_t accel[3], int16_t gyro[3], int16_t *temp);

#endif