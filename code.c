/**
 ******************************************************************************
 * @author Paolo Sassi
 * @date 21 January 2016
 * @brief Application main source file.
 ******************************************************************************
 * @attention
 *
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
 ******************************************************************************
 */

#include "ee.h"

#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery_lcd.h"
#include "i2c_lib.h"
#include "aux.h"
#include "defines.h"

uint8_t r_num = 0;
uint16_t p_buff[MAX_POINTS];


/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

TASK(LCDController)
{
static uint16_t i;
char s[20];
static uint16_t x = 0;
uint32_t p_x, p_y;

	if (r_num == 0) {
		sprintf(s, "Touch the screen to\0");
		WPrint(40, 120, s, FONT_BIG);
		sprintf(s, "start the ranging...\0");
		WPrint(40, 150, s, FONT_BIG);
		GetTouch_SC_Sync(&p_x, &p_y);
		LCD_Clear(White);
	}

	if (r_num < MAX_ROUNDS) {
		/* Waits 3 seconds, then starts the ranging */
		for(i = 0; i < 3; i++) {
			LCD_DrawFilledRect(40, 115, 300, 135, White, White);
			sprintf(s, "Capturing starts in: %i\0", 3 - i);
			WPrint(40, 120, s, FONT_BIG);
			sleep(1000);
		}
		LCD_DrawFilledRect(40, 115, 300, 135, White, White);
		SetRelAlarm(AlarmSR, 10, 100);
		SetRelAlarm(AlarmGR, 75, 100);
	} else {
		LCD_Clear(White);
		printAxis();
		for (i = 0; i < MAX_POINTS - 1; i++) {
			LCD_DrawUniLine(SC_X(x), SC_Y(p_buff[i]), SC_X(x + 1),
					SC_Y(p_buff[i + 1]));
			x += 1;
		}
	}
}

TASK(GetRanging)
{
static uint16_t i = 0;
uint16_t y = 0;
uint8_t low, high;
char s[20];

	if (i == 0) {
		LCD_Clear(White);
		sprintf(s, "Ranging round %d out of %d", r_num + 1, MAX_ROUNDS);
		WPrint(10, 10, s, FONT_SMALL);
	}
	low = high = 0;
	LCD_SetTextColor(Black);
	I2C_read(ADDRESS, 0x03, &low);	/* first echo low byte */
	I2C_read(ADDRESS, 0x02, &high);	/* first echo high byte */
	y = (high << 8) + low;

	if (r_num == 0) {
		p_buff[i] = y;
	} else {
		p_buff[i] = K * p_buff[i] + (1 - K) * y;
	}
	if (p_buff[i] > MAX_DIST) p_buff[i] = MAX_DIST;

	LCD_DrawFilledRect(40, 115, 300, 135, White, White);
	sprintf(s, "Ranging sample %d/%d\0", i, MAX_POINTS);
	WPrint(10, 120, s, FONT_BIG);

	if (i == MAX_POINTS) {
		r_num++;
		i = 0;
		LCD_Clear(White);
		CancelAlarm(AlarmGR);
		CancelAlarm(AlarmSR);
		ActivateTask(LCDController);
	} else {
		i++;
	}
}


TASK(StartRanging)
{
	I2C_write(ADDRESS, COMM_REG, RES_CM);
}

int main(void)
{
	SystemInit();

	/* Initialize Erika related stuffs*/
	EE_system_init();

	/* Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/* Initialize the LCD Display */

	IOE_Config();
	STM32f4_Discovery_LCD_Init();
	InitTouch(-0.0853, 0.0665, -331, 15);

	/* Initialize the sonar */
	//I2C_init();
	I2C_write(ADDRESS, RANGE_REG, RANGE);
	I2C_write(ADDRESS, GAIN_REG, MAX_GAIN);

	/* Clear the screen */
	LCD_Clear(White);

	ActivateTask(LCDController);

	/* Forever loop: background activities (if any) should go here */
	for (;;);

}
