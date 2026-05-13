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
#include <stdlib.h>
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
  int16_t ax_, ay_, az_, gx_, gy_, gz_;
  int32_t pitch_g = 0, roll_g = 0, yaw_g = 0;
  char buffer[200];

  while(1) {

    // Lê os dados brutos do MPU6050
    mpu6050_getRawData(&ax_, &ay_, &az_, &gx_, &gy_, &gz_);

    //Normalização acelerômetro:
    // ax, ay, az estão em mili-g (1g = 1000 mg)
    int32_t ax = ((int32_t)ax_ * 1000) / 16384;
    int32_t ay = ((int32_t)ay_ * 1000) / 16384;
    int32_t az = ((int32_t)az_ * 1000) / 16384;

    // Normalização giroscópio:
    // gx, gy, gz estão em mili-graus por segundo (1°/s = 1000 mg/s)
    int32_t gx = ((int32_t)gx_ * 10000) / 164;
    int32_t gy = ((int32_t)gy_ * 10000) / 164;
    int32_t gz = ((int32_t)gz_ * 10000) / 164;

    // Calcula os ângulos de inclinação usando os dados do acelerômetro.
    // Roll, Pitch e Yaw estão em mili-graus (1g = 1000 mg)
    int32_t roll  = atan2(ay, sqrt((ax*ax) + (az*az))) * 180000 / M_PI;
    int32_t pitch = atan2(-ax, sqrt((ay*ay) + (az*az))) * 180000 / M_PI;
    int32_t yaw   = 0;

    // Calcula os ângulos de rotação usando os dados do giroscópio.
    // pitch_g, roll_g e yaw_g estão em mili-graus (1°/s = 1000 mg/s)
    pitch_g = pitch_g + gx / 10;
    roll_g  = roll_g  + gy / 10;
    yaw_g   = yaw_g   + gz / 10; 

    sprintf(buffer, "Pitch_Acc: %ld.%03ld , Roll_Acc: %ld.%03ld , Yaw_Acc: %ld.%03ld "
                    "| Pitch_Giro: %ld.%03ld, Roll_Giro: %ld.%03ld , Yaw_Giro: %ld.%03ld ,", 
        pitch / 1000, labs(pitch % 1000), 
        roll / 1000, labs(roll % 1000), 
        yaw / 1000, labs(yaw % 1000),
        pitch_g / 1000, labs(pitch_g % 1000),
        roll_g / 1000, labs(roll_g % 1000),
        yaw_g / 1000, labs(yaw_g % 1000));
    uart_puts(buffer);
    
   _delay_ms(100);
  }

    return 0;
}
