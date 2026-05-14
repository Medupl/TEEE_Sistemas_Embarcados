// i2cmaster_wrapper.h
// Traduz as chamadas do Fleury para o TWI da Matiasus

#ifndef I2CMASTER_WRAPPER_H
#define I2CMASTER_WRAPPER_H

#include "twi.h"  // TWI da Matiasus

// Mapeamento das funções Fleury → Matiasus
#define i2c_init()          TWI_Init(TWI_FREQ)
#define i2c_start(addr)     TWI_Start(addr)
#define i2c_stop()          TWI_Stop()
#define i2c_write(data)     TWI_Write(data)
#define i2c_readAck()       TWI_Read(TWI_ACK)
#define i2c_readNak()       TWI_Read(TWI_NACK)

#define I2C_WRITE 0
#define I2C_READ  1

#endif