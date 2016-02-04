/**
 * @file i2c_lib.h
 * @author Paolo Sassi
 * @date 2 February 2016
 * @brief I2C library header file
 */

#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

/**
 * @defgroup I2C_Macros I2C Macros
 * @{
 */

#define I2C_CLOCK		100000			/* 100 KHz, standard mode */
#define I2C_ADDR_LENGTH I2C_AcknowledgedAddress_7bit
#define I2C_OWN_ADDR	0x00
#define I2C_ACK_STATUS	I2C_Ack_Disable	/* no ACK while reading */
#define I2C_DUTY_CYCLE	I2C_DutyCycle_2 /* 50% duty cycle */

/**
 * @}
 */
/**
 * @defgroup I2C_func	I2C Library Functions
 * @{
 */
/**
 * @brief Initializes I2C1 Bus
 * @param None
 * @retval None
 */
void I2C_init();

/**
 * @brief Reads one byte from the slave device
 * @param address	Address of the device from which the byte is read.
 * @param reg		Location of the register on the specified device.
 * @retval data		Data read from the device.
 */
uint8_t I2C_read(uint8_t address, uint8_t reg);

/**
 * @brief Writes one byte to the slave device
 * @param address	Address of the device to which the byte is written.
 * @param reg		Location of the register on the specified device.
 * @retval None
 */
void I2C_write(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @}
 */


#endif /* I2C_LIB_H_ */
