# --- Configurações do Projeto ---
TARGET = estimador_pose
MCU = atmega328p
F_CPU = 16000000UL

# --- Ferramentas do AVR ---
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# --- Pastas de Inclusão (Onde o compilador vai procurar os .h) ---
# O -I avisa o compilador onde estão os cabeçalhos
INCLUDES = -I. -I../lib_mpu6050/src/mpu6050 -I../lib_mpu6050/src/i2chw -I../lib_mpu6050/src/uart

# --- Flags de Compilação ---
# -Os otimiza para tamanho (essencial para AVR)
# -lm inclui a biblioteca de matemática <math.h>
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) $(INCLUDES)

# --- Arquivos Fonte (.c) ---
# Adicione aqui todos os arquivos .c que precisam ser compilados
SRCS = main.c \
       ../lib_mpu6050/src/mpu6050/mpu6050.c \
       ../lib_mpu6050/src/i2chw/twimastertimeout.c \
       ../lib_mpu6050/src/uart/uart.c

# Transforma a lista de .c em .o (arquivos objeto)
OBJS = $(SRCS:.c=.o)

# --- Regras de Compilação ---

# Regra padrão executada ao digitar 'make'
all: $(TARGET).hex

# Cria o arquivo .hex (que vai para a placa) a partir do .elf
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@
	@echo "Build concluído com sucesso!"

# Junta todos os .o para criar o .elf
$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compila cada .c em um .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Limpeza e Gravação ---

# Digite 'make clean' para apagar os arquivos compilados
clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex

# Digite 'make flash' para gravar no Arduino/ATmega
# Atenção: Mude a porta /dev/ttyUSB0 para a porta correta do seu Linux (ex: /dev/ttyACM0)
flash: $(TARGET).hex
	$(AVRDUDE) -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 115200 -U flash:w:$(TARGET).hex:i