#include "inc/mpu6050/mpu6050.h"

// Função auxiliar para escrever em um registrador
void mpu6050_write_register(i2c_inst_t *i2c, uint8_t reg, uint8_t data) {
    uint8_t buf[] = {reg, data};
    i2c_write_blocking(i2c, MPU6050_ADDR, buf, 2, false);
}

void mpu6050_init(i2c_inst_t *i2c) {
    // Reseta o dispositivo para limpar configurações antigas
    mpu6050_write_register(i2c, MPU6050_PWR_MGMT_1, 0x80);
    sleep_ms(100);

    // Acorda o sensor e configura a fonte de clock
    mpu6050_write_register(i2c, MPU6050_PWR_MGMT_1, 0x00);
    sleep_ms(100);

    // Configura a taxa de amostragem (Sample Rate) para 1KHz
    mpu6050_write_register(i2c, MPU6050_SMPLRT_DIV, 0x07);
    
    // Configura o Digital Low Pass Filter (DLPF)
    mpu6050_write_register(i2c, MPU6050_CONFIG, 0x00);
    
    // Configura o giroscópio para a escala de +/- 2000 graus/s
    mpu6050_write_register(i2c, MPU6050_GYRO_CONFIG, 0x18);
    
    // Configura o acelerômetro para a escala de +/- 2g
    mpu6050_write_register(i2c, MPU6050_ACCEL_CONFIG, 0x00);

    sleep_ms(100); // Delay final para garantir que tudo estabilizou
}

void mpu6050_read_raw(i2c_inst_t *i2c, int16_t accel[3], int16_t gyro[3], int16_t *temp) {
    uint8_t buffer[14];
    uint8_t reg = MPU6050_ACCEL_XOUT_H;
    i2c_write_blocking(i2c, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c, MPU6050_ADDR, buffer, 14, false);

    accel[0] = (buffer[0] << 8) | buffer[1];
    accel[1] = (buffer[2] << 8) | buffer[3];
    accel[2] = (buffer[4] << 8) | buffer[5];
    *temp = (buffer[6] << 8) | buffer[7];
    gyro[0] = (buffer[8] << 8) | buffer[9];
    gyro[1] = (buffer[10] << 8) | buffer[11];
    gyro[2] = (buffer[12] << 8) | buffer[13];
}