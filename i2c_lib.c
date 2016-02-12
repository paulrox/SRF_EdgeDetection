/******************************************************************************
 * @author	Paolo Sassi
 * @date	2 February 2016
 * @brief	I2C Library source file.
 ******************************************************************************
 */

#include "i2c_lib.h"

/* Sends the start sequence and the device address */
static uint8_t I2C_start(uint8_t address, uint8_t direction)
{
uint32_t timeout;

	timeout = TIMEOUT_T;
	/* send the start sequence */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* wait until the slave has acknowledged the start sequence */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) {
		timeout--;
		if (timeout == 0) return 1;	/* no response */
	}
	/* send the slave address */
	I2C_Send7bitAddress(I2C1, address, direction);
	/* wait the acknowledge */
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
	return 0;	/* ok */
}

void I2C_init()
{
/* Initialization structures */
GPIO_InitTypeDef GPIO_InitStruct;
I2C_InitTypeDef I2C_InitStruct;

	/* enable APB1 peripheral clock for I2C1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* enable clock for SCL pin*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* We are using I2C1, where:
	 * SCL is PB8
	 * SDA is PB9
	 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	/* set pins to alternate function */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	/* set "Medium" GPIO speed */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	/* open drain, the line has only to be pulled down */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	/* disable pull-up resistors */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Set the pins alternate functions */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);	/* SCL */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); /* SDA */

	/* Initialize I2C1 */
	I2C_InitStruct.I2C_ClockSpeed = I2C_CLOCK;		/* clock speed */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			/* I2C mode */
	I2C_InitStruct.I2C_DutyCycle = I2C_DUTY_CYCLE;	/* duty cycle */
	I2C_InitStruct.I2C_OwnAddress1 = I2C_OWN_ADDR;
	I2C_InitStruct.I2C_Ack = I2C_ACK_STATUS;
	/* choose the I2C address length */
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_ADDR_LENGTH;
	/* We first disable the I2C, then we initialize it and
	 * finally we enable it
	 */
	I2C_Cmd(I2C1, DISABLE);
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE);
}

uint8_t I2C_read(uint8_t address, uint8_t reg, uint8_t *data)
{
uint32_t timeout = TIMEOUT_T;

	/* send the start sequence and the address */
	if (I2C_start(address, I2C_Direction_Transmitter) != 0) {
		return START_TIMEOUT;
	}
	/* send the register location */
	I2C_SendData(I2C1, reg);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		timeout--;
		if (timeout == 0) return REG_TIMEOUT;
	}
	timeout = TIMEOUT_T;
	/* disable ack */
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	/* send another start sequence (repeated start) */
	if (I2C_start(address, I2C_Direction_Receiver) != 0) {
		return RESTART_TIMEOUT;
	}
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
		timeout--;
		if (timeout == 0) return RECEIVE_TIMEOUT;
	}
	/* read data from I2C1 data register and return data byte */
	*data = I2C_ReceiveData(I2C1);
	I2C_GenerateSTOP(I2C1, ENABLE);
	return NO_ERR;
}

uint8_t I2C_write(uint8_t address, uint8_t reg, uint8_t data)
{
uint32_t timeout = TIMEOUT_T;

	/* send the start sequence and the address */
	if (I2C_start(address, I2C_Direction_Transmitter) != 0) {
		return START_TIMEOUT;
	}
	/* send the register location */
	I2C_SendData(I2C1, reg);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		timeout--;
		if (timeout == 0) return REG_TIMEOUT;
	}
	timeout = TIMEOUT_T;
	/* send the data byte */
	I2C_SendData(I2C1, data);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
		timeout--;
		if (timeout == 0) return SEND_TIMEOUT;
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
	return NO_ERR;
}

