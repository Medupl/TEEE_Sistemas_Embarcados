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
#include "ssd1306.h"        // Biblioteca para LCD

volatile uint8_t flag_100ms = 0;

void setup_timer1(void) {
    TCCR1B |= (1 << WGM12);           // Modo CTC
    TIMSK1 |= (1 << OCIE1A);          // Habilita interrupção por comparação

    // F_CPU (16MHz) / Prescaler (1024) = 15.625 pulsos por segundo
    // Para 10Hz (100ms): 15.625 / 10 = 1562.5
    OCR1A = 1562;
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

ISR(TIMER1_COMPA_vect) {
    flag_100ms = 1; 
}

int main(){

  // Inicializa o MPU6050
  mpu6050_init();
  SSD1306_Init(SSD1306_ADDR);
  _delay_ms(500); 

  // Inicializa a UART para comunicação serial.
  uart_init(UART_BAUD_SELECT(BAUD, F_CPU));
  setup_timer1();
  sei(); 

  uart_puts("Iniciando Teste de comunicacao...\r\n");

  if (i2c_start(0x68 << 1) == 0) {
      uart_puts("MPU6050 conectado com sucesso!\r\n");
      i2c_stop(); 
  } else {
      uart_puts("Falha na conexao fisica.\r\n");
      while(1);
  }

  SSD1306_ClearScreen();
  SSD1306_SetPosition(0, 0);
  SSD1306_DrawString("Calibrando...");

  // Declaração de variáveis para armazenar os dados do acelerômetro e giroscópio
  int16_t ax_, ay_, az_, gx_, gy_, gz_;
  int32_t pitch = 0, roll = 0, yaw = 0;
  char buffer[120];
  char linha[22];

  //Calibração inicial do giroscópio para obter o offset do eixo Z
  int32_t gx_off = 0, gy_off = 0, gz_off = 0;
  for(int i=0; i<500; i++) {
      mpu6050_getRawData(&ax_, &ay_, &az_, &gx_, &gy_, &gz_);
      gx_off += gx_; gy_off += gy_; gz_off += gz_;
      _delay_ms(2);
  }
  gx_off /= 500; gy_off /= 500; gz_off /= 500;

  while(1) {
    if (flag_100ms) {
      
      flag_100ms = 0;
      // Lê os dados brutos do MPU6050
      mpu6050_getRawData(&ax_, &ay_, &az_, &gx_, &gy_, &gz_);

      //Normalização acelerômetro:
      // ax, ay, az estão em mili-g (1g = 1000 mg)
      int32_t ax = ((int32_t)ax_ * 1000) / 16384;
      int32_t ay = ((int32_t)ay_ * 1000) / 16384;
      int32_t az = ((int32_t)az_ * 1000) / 16384;

      // Normalização giroscópio:
      // gx, gy, gz estão em mili-graus por segundo (1°/s = 1000 mg/s)
      int32_t gx = ((int32_t)(gx_ - gx_off) * 10000) / 164; 
      int32_t gy = ((int32_t)(gy_ - gy_off) * 10000) / 164;
      int32_t gz = ((int32_t)(gz_ - gz_off) * 10000) / 164;

      // Calcula os ângulos de inclinação usando os dados do acelerômetro.
      // Roll, Pitch e Yaw estão em mili-graus (1g = 1000 mg)
      int32_t roll_a  = atan2(ay, sqrt((ax*ax) + (az*az))) * 180000 / M_PI;
      int32_t pitch_a = atan2(-ax, sqrt((ay*ay) + (az*az))) * 180000 / M_PI;
      int32_t yaw_a   = 0;

      // Usando a logica dos links mandado pelo professor, fazemos:
      // Encontrando o valor do filtro complementar para cada ângulo.
      int32_t alpha = 90;  // Fator de filtro complementar (ajuste conforme necessário)
      pitch = (alpha * (pitch + gx / 10) + (100 - alpha) * pitch_a) / 100;
      roll  = (alpha * (roll  + gy / 10) + (100 - alpha) * roll_a)  / 100;
      //yaw   = (alpha * (yaw   + gz / 100) + (100 - alpha) * yaw_a)   / 100;
      // Yaw com zona morta para reduzir drift
      int32_t gz_filtrado = (gz > 500 || gz < -500) ? gz : 0;
      yaw = yaw + gz_filtrado / 10 + yaw_a;

      // Exibe os ângulos no Serial Monitor
      sprintf(buffer, "Ac: X:%ld.%1ld Y:%ld.%1ld Z:%ld.%1ld | "
                      "Giro: X:%ld.%1ld Y:%ld.%1ld Z:%ld.%1ld | "
                      "Euler: P:%ld.%1ld R:%ld.%1ld Y:%ld.%1ld\r\n", 
                      // Aceleração
                      ax/1000, labs(ax%1000)/100,
                      ay/1000, labs(ay%1000)/100,
                      az/1000, labs(az%1000)/100,
                      // Velocidade angular
                      gx/1000, labs(gx%1000)/100,
                      gy/1000, labs(gy%1000)/100,
                      gz/1000, labs(gz%1000)/100,
                      // Ângulos de Euler
                      pitch/1000, labs(pitch%1000)/100,
                      roll/1000,  labs(roll%1000) /100,
                      yaw/1000,   labs(yaw%1000)  /100);

      uart_puts(buffer);

      // 7. Atualiza display
      SSD1306_ClearScreen();
      SSD1306_SetPosition(0, 0);
      sprintf(linha, "P:%ld.%1ld",
          pitch/1000, labs(pitch%1000)/100);
      SSD1306_DrawString(linha);

      SSD1306_SetPosition(0, 2);
      sprintf(linha, "R:%ld.%1ld",
          roll/1000, labs(roll%1000)/100);
      SSD1306_DrawString(linha);

      SSD1306_SetPosition(0, 4);
      sprintf(linha, "Y:%ld.%1ld",
          yaw/1000, labs(yaw%1000)/100);
      SSD1306_DrawString(linha);

      SSD1306_UpdateScreen(SSD1306_ADDR);

    }
  }

    return 0;
}
