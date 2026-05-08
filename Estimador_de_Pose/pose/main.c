/*
MPU6050 lib 0x05

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.

References:
  - most of the code is a port of the arduino mpu6050 library by Jeff Rowberg
    https://github.com/jrowberg/i2cdevlib
  - Mahony complementary filter for attitude estimation
    http://www.x-io.co.uk
*/
/**
 * @file main.c
 * @author marcos
 * @date 2026-05-06
 * @brief Main function
 */

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL

#include <avr/interrupt.h> // Para usar interrupções!
#include <math.h>          // Muito útil se for lidar com ângulos e trigonometria

// Bibliotecas locais
#include "mpu6050.h" // Biblioteca do MPU6050
#include "i2cmaster.h" // Biblioteca de I2C do Fleury
#include "uart.h" // Biblioteca de UART para comunicação serial

int main(){

    // Add your code here and press Ctrl + Shift + B to build

    return 0;
}
