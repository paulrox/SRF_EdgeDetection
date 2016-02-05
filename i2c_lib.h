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

/**
 * @defgroup bus	Bus Macros
 * @{
 */
#define I2C_CLOCK		100000			/* 100 KHz, standard mode */
#define I2C_ADDR_LENGTH I2C_AcknowledgedAddress_7bit
#define I2C_OWN_ADDR	0x00
#define I2C_ACK_STATUS	I2C_Ack_Disable	/* no ACK while reading */
#define I2C_DUTY_CYCLE	I2C_DutyCycle_2 /* 50% duty cycle */
#define TIMEOUT_T		20000
/**
 * @}
 */

/**
 * @defgroup err_codes Error Codes
 * @{
 */
#define NO_ERR			0
#define START_TIMEOUT	1
#define REG_TIMEOUT		2
#define RESTART_TIMEOUT 3
#define RECEIVE_TIMEOUT	4
#define SEND_TIMEOUT	3
/**
 * @}
 */

/**
 * @}
 */

/**
 * @defgroup I2C_func	I2C Library Functions
 * @{
 */
/**
 * @brief Initializes I2C Bus.
 * @param None
 * @retval None
 *
 * The used I2C bus is the I2C1, where PB8 pin is connected to the SCL line and
 * the PB9 pin is connected to the SDA line.
 */
void I2C_init();

/**
 * @brief Reads one byte from the slave device
 * @param address	Address of the device from which the byte is read.
 * @param reg		Location of the register on the specified device.
 * @param data		Received byte from the device;
 * @retval NO_ERR	No error occurred.
 * @retval START_TIMEOUT Start acknowledgment timeout.
 * @retval REG_TIMEOUT	 Send register location timeout.
 * @retval RESTART_TIMEOUT Repeated start timeout.
 * @retval RECEIVE_TIMEOUT Receive data timeout.
 */
uint8_t I2C_read(uint8_t address, uint8_t reg, uint8_t *data);

/**
 * @brief Writes one byte to the slave device
 * @param address	Address of the device to which the byte is written.
 * @param reg		Location of the register on the specified device.
 * @param data		Byte to be sent to the slave device.
 * @retval NO_ERR	No error occurred.
 * @retval REG_TIMEOUT Send register location timeout.
 * @retval SEND_TIMEOUT Send data timeout.
 */
uint8_t I2C_write(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @}
 */

#endif /* I2C_LIB_H_ */
