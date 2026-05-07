/* MPU6050 AVR Library
 *
 * Copyright (C) 2026 Marcos Eduardo.
 * Written by Marcos Eduardo.
 */

#ifndef __MPU_6050_H__
#define __MPU_6050_H__

#include <stdint.h> 

// --- Endereços do MPU6050 baseados no Manual Oficial ---
#define SIGNAL_PATH_RESET 0x68 // Endereço I2C padrão de 7 bits
#define PWR_MGMT_1        0x6B // Registrador 107
#define GYRO_CONFIG       0x1B // Registrador 27
#define ACCEL_CONFIG      0x1C // Registrador 28

// --- Protótipos das Funções do MPU6050 ---
// Somente o ponto e vírgula! A lógica vai no arquivo .c
void mpu6050_init(void);

#endif /* __MPU_6050_H__ */