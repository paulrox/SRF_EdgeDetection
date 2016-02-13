/**
 ******************************************************************************
 * @file 	i2c_lib.h
 * @author 	Paolo Sassi
 * @date 	2 February 2016
 * @brief 	I2C library header file
 ******************************************************************************
 */

#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include "stm32f4xx.h"
#include "stm32f4xx_i2c.h"

/**
 * @addtogroup I2C_lib I2C Library
 * @{
 */
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
 * @defgroup I2C_exp_func	I2C Library Exported Functions
 * @{
 */
void I2C_init();
uint8_t I2C_read(uint8_t address, uint8_t reg, uint8_t *data);
uint8_t I2C_write(uint8_t address, uint8_t reg, uint8_t data);

/**
 * @}
 */
/**
 * @}
 */

#endif /* I2C_LIB_H_ */
