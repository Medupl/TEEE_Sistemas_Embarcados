/**
 * twi.c — Wrapper que implementa a API TWI da Matiasus
 * usando a biblioteca I2C do Fleury (i2cmaster)
 */
#include "twi.h"
#include "i2cmaster.h"

void TWI_Init(void) {
    // Nada a fazer — i2c já está inicializado pelo MPU6050
}

char TWI_MT_Start(void) {
    return SUCCESS;
}

char TWI_MT_Send_SLAW(char address) {
    // Fleury espera endereço com bit R/W: (addr << 1) | 0
    if (i2c_start((unsigned char)(address << 1)) == 0) {
        return SUCCESS;
    }
    return ERROR;
}

char TWI_MT_Send_Data(char data) {
    if (i2c_write((unsigned char)data) == 0) {
        return SUCCESS;
    }
    return ERROR;
}

void TWI_Stop(void) {
    i2c_stop();
}
