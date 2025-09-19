#include "inc/mpu6050/mpu6050.h"

void inicializar_mpu6050(i2c_inst_t *i2c_instance)
{
    i2c_init(i2c_instance, 400 * 1000);
    gpio_set_function(MPU6050_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(MPU6050_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(MPU6050_SDA_PIN);
    gpio_pull_up(MPU6050_SCL_PIN);

    uint8_t buf[] = {MPU6050_REG_PWR_MGMT_1, 0x00};
    i2c_write_blocking(i2c_instance, MPU6050_ADDR, buf, 2, false);
}

void leitura_acelero_mpu6050(i2c_inst_t *i2c_instance, mpu6050_accel_data_t *accel_data)
{
    uint8_t buffer[6];

    uint8_t reg = MPU6050_REG_ACCEL_XOUT_H;
    i2c_write_blocking(i2c_instance, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_instance, MPU6050_ADDR, buffer, 6, false);

    accel_data->x = (int16_t)((buffer[0] << 8) | buffer[1]);
    accel_data->y = (int16_t)((buffer[2] << 8) | buffer[3]);
    accel_data->z = (int16_t)((buffer[4] << 8) | buffer[5]);
}