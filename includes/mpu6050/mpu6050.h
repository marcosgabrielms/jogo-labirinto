#ifndef MPU6050_H
#define MPU6050_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

void inicializar_mpu6050();
void calibrar_mpu6050();
void ler_mpu6050(int16_t *aceleracao);

#endif