/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include "ee.h"

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery_lcd.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_i2c.h"

#define ADDRESS		0xE0
#define RANGE		140 	/* 6 meters range */
#define SCALE		0.25
#define SC_WIDTH	320
#define SC_HEIGHT	240
#define X_OFFSET	30
#define Y_OFFSET	10
#define AXIS_LENGHT	SC_HEIGHT - Y_OFFSET

static void WPrint(uint16_t x, uint16_t y,char *s)
{
	LCD_SetTextColor(Black);
	LCD_SetFont(&Font8x8);
	LCD_DisplayStringXY(x, y, s);
}

static void printAxis()
{
uint16_t y_mark = AXIS_LENGHT;
uint8_t value = 0;
char s[4];

	LCD_DrawUniLine(X_OFFSET, 5, X_OFFSET, AXIS_LENGHT);
	LCD_DrawUniLine(X_OFFSET, AXIS_LENGHT + 1, 320, AXIS_LENGHT + 1);

	for (y_mark = AXIS_LENGHT; y_mark >= 15; y_mark = y_mark - 15) {
		LCD_DrawUniLine(X_OFFSET, y_mark, X_OFFSET - 2, y_mark);
		sprintf(s, "%d\0", value);
		WPrint(0, y_mark - 4, s);
		value += 15 * SCALE;
	}
}

static void InitSonar()
{
	TM_I2C_Init(I2C1, TM_I2C_PinsPack_2, 100000);
	TM_I2C_Write(I2C1, ADDRESS, 0x02, RANGE);
}

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

#define MAX_ROUND 5;

TASK(TaskLCD)
{
static uint8_t data[2] = {0 , 0};
static uint16_t x = X_OFFSET + 1;
static uint16_t y1 = 0;
static uint16_t y2 = 0;
static uint16_t prev_y1 = 0;
static uint16_t prev_y2;
char s[20];
/*
	LCD_Clear(White);
	data[0] = TM_I2C_Read(I2C1, ADDRESS, 0x01);
	sprintf(s, "light: %d\0", data[0]);
	WPrint(150, 150, s);
	data[1] = TM_I2C_Read(I2C1, ADDRESS, 0x03);
	sprintf(s, "dist: %d\0", data[1]);
	WPrint(150, 200, s);
*/
	if (x == SC_WIDTH) {
		LCD_Clear(White);
		printAxis();
		x = X_OFFSET + 1;
	}

	data[0] = TM_I2C_Read(I2C1, ADDRESS, 0x03);	/* first echo low byte */
	data[1] = TM_I2C_Read(I2C1, ADDRESS, 0x02);	/* first echo high byte */
	y1 = (data[1]<<8) + data[0];
	LCD_DrawUniLine(x, SC_HEIGHT - prev_y1 / SCALE - Y_OFFSET,
			++x, SC_HEIGHT - y1 / SCALE - Y_OFFSET);
	prev_y1 = y1;
/*
	LCD_SetTextColor(Red);
	data[0] = TM_I2C_Read(I2C1, ADDRESS, 0x05);
	data[1] = TM_I2C_Read(I2C1, ADDRESS, 0x04);
	y2 = (data[1]<<8) + data[0];
	LCD_DrawUniLine(x, SC_HEIGHT - prev_y2 / SCALE - Y_OFFSET,
			++x, SC_HEIGHT - y2 / SCALE - Y_OFFSET);
	prev_y2 = y2;
	LCD_SetTextColor(Black);
	*/
}

TASK(TaskCapture)
{
	TM_I2C_Write(I2C1, ADDRESS, 0x00, 0x51);
}

int main(void)
{
	uint8_t test;
	uint8_t revision = 0;

	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 * Setup the microcontroller system.
	 * Initialize the Embedded Flash Interface, the PLL and update the
	 * SystemFrequency variable.
	 * For default settings look at:
	 * pkg/mcu/st_stm32_stm32f4xx/src/system_stm32f4xx.c
	 */
	SystemInit();

	/*Initialize Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	IOE_Config();
	STM32f4_Discovery_LCD_Init();
	InitTouch(-0.0853, 0.0665, -331, 15);
	/* initialize the sonar with a range of 1 metre */
	InitSonar();
	LCD_Clear(White);
	printAxis();
	/* Program cyclic alarms which will fire after an initial offset,
	 * and after that periodically
	 * */
	SetRelAlarm(AlarmLCD, 75, 100);
	SetRelAlarm(AlarmCapture, 10, 100);

	/* Forever loop: background activities (if any) should go here */
	for (;;);

}

