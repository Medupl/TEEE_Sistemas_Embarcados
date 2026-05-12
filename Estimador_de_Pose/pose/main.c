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
#define F_CPU 16000000UL        // Frequência do cristal (16 MHz)
#define BAUD 9600               // Taxa de transmissão desejada (Baud Rate)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>  // Para usar interrupções!
#include <math.h>           // Muito útil se for lidar com ângulos e trigonometria

// Bibliotecas locais
#include "mpu6050.h"        // Biblioteca do MPU6050
#include "i2cmaster.h"      // Biblioteca de I2C do Fleury
#include "uart.h"           // Biblioteca de UART para comunicação serial

int main(){

  // Inicializa a UART para comunicação serial.
  uart_init(UART_BAUD_SELECT(BAUD, F_CPU));
  sei(); 

  // Inicializa o MPU6050
  mpu6050_init();
  _delay_ms(500); 

  uart_puts("Iniciando Teste de comunicacao...\r\n");

  if (i2c_start(0x68 << 1) == 0) {
      uart_puts("MPU6050 conectado com sucesso!\r\n");
      i2c_stop(); 
  } else {
      uart_puts("Falha na conexao fisica.\r\n");
      while(1);
  }

  // Declaração de variáveis para armazenar os dados do acelerômetro e giroscópio
  int16_t ax, ay, az, gx, gy, gz;
  char buffer[50];

  while(1) {

    // Lê os dados brutos do MPU6050
    mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);

    // Envia os dados lidos para o terminal serial
    sprintf(buffer, "Accel: Ax:%d Ay:%d Az:%d | Gyro: Gx:%d Gy:%d Gz:%d\r\n", ax, ay, az, gx, gy, gz);
    uart_puts(buffer);
    _delay_ms(300);
  }

    return 0;
}
