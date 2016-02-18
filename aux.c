/**
 ******************************************************************************
 * @file 	aux.c
 * @author	Paolo Sassi
 * @date	10 February 2016
 * @brief	Application auxiliary functions source code.
 ******************************************************************************
 */

#include "ee.h"
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4_discovery_lcd.h"
#include "defines.h"

#include "aux.h"

/**
 * @brief Initializes the SRF
 *
 * In this case we don't need to explicitly initialize the I2C
 * bus because the LCD initialization function has already initialized it.
 * @param None
 * @retval 0 Initialization Ok. Other values, an error occurred.
 */
uint8_t sonarInit() {
uint8_t ret = 0;
	/* I2C_init(); */
	/* write the custom range value */
	ret = I2C_write(ADDRESS, RANGE_REG, RANGE);
	/* write the custom analogue gain */
	ret += I2C_write(ADDRESS, GAIN_REG, MAX_GAIN);
	return ret;
}

/**
 * @brief Prints a string on the LCD display.
 * @param x X coordinate of the string position.
 * @param y Y coordinate of the string position.
 * @param s Pointer to the string to be displayed.
 * @param f Pointer to font data structure.
 * @retval None
 */
void printString(uint16_t x, uint16_t y, uint8_t *s, sFONT *f)
{
	LCD_SetTextColor(Black);
	LCD_SetFont(f);
	LCD_DisplayStringXY(x, y, s);
}

/**
 * @brief Prints the axis on the LCD display.
 * @param None
 * @retval None
 */
void printAxis()
{
uint16_t y_mark = AXIS_LENGHT;
uint8_t value = 0;
char s[4];

	LCD_SetTextColor(Red);
	LCD_DrawUniLine(X_OFFSET + 1, AXIS_LENGHT, 320, AXIS_LENGHT);
	LCD_SetTextColor(Black);
	LCD_DrawUniLine(X_OFFSET, Y_OFFSET, X_OFFSET, AXIS_LENGHT);
	LCD_DrawUniLine(X_OFFSET + 1, Y_OFFSET, 320, Y_OFFSET);
	for (y_mark = Y_OFFSET; y_mark <= AXIS_LENGHT; y_mark += 15) {
		LCD_DrawUniLine(X_OFFSET, y_mark, X_OFFSET - 2, y_mark);
		sprintf(s, "%d\0", value);
		printString(0, y_mark - 4, s, FONT_SMALL);
		value += 15 * SCALE;
	}
}

/**
 * @brief Prints the sampling results on the LCD
 * @param buff Buffer containing the points to be displayed
 * @retval None
 */
void printResults(uint16_t *buff)
{
uint16_t i, x;

#ifdef NO_FILTER
		LCD_SetTextColor(Red);
		x = 0;
#ifdef REVERSE_SAMPLES
		for (i = MAX_POINTS; i > 0; i--) {
			LCD_DrawUniLine(SC_X(x), SC_Y(buff[i]), SC_X(x + 1),
					SC_Y(buff[i - 1]));
			x += 1;
		}
#else
		for (i = 0; i < MAX_POINTS - 1; i++) {
			LCD_DrawUniLine(SC_X(x), SC_Y(buff[i]), SC_X(x + 1),
					SC_Y(buff[i + 1]));
			x += 1;
		}
#endif	/* REVERSE_ORDER */
		LCD_SetTextColor(Black);
#endif	/* NO_FILTER */
		cutOff(buff);
		if (MAX_ROUNDS == 1) filterSamples(buff);
		x = 0;
#ifdef REVERSE_SAMPLES
		for (i = MAX_POINTS; i > 0; i--) {
			LCD_DrawUniLine(SC_X(x), SC_Y(buff[i]), SC_X(x + 1),
					SC_Y(buff[i - 1]));
			x += 1;
		}
#else
		for (i = 0; i < MAX_POINTS - 1; i++) {
			LCD_DrawUniLine(SC_X(x), SC_Y(buff[i]), SC_X(x + 1),
					SC_Y(buff[i + 1]));
			x += 1;
		}
#endif	/* REVERSE_ORDER */
}

/**
 * @brief Makes the calling task enter a busy wait
 * for a fixed amount of milliseconds.
 * @param msec The amount of milliseconds the task will wait.
 * @retval None
 */
void sleep(int32_t msec)
{
AlarmType alarm;
TaskType task;
EventMaskType ev;

	GetTaskID(&task);
	alarm = (task == AppController) ? AlarmACsleep : AlarmRFCsleep;
	SetRelAlarm(alarm, msec, 0);
	WaitEvent(sleep_event);
	ClearEvent(sleep_event);
}

/**
 * @brief Cuts off the spurious samples detected by the range
 * finder when crossing an hole.
 * @param buff Buffer containing the sampled data.
 * @retval 0 No spurious sample detected
 * @retval 1 At least one spurious sample has been detected
 */

uint8_t cutOff(uint16_t *buff)
{
uint8_t ret = 0;
int16_t min_i, max_i, i, j;

	min_i = max_i = -1;
	for (i = 0; i < MAX_POINTS - 1; i++) {
		if (buff[i] >= (MAX_DIST - CUT_SENSE) && buff[i + 1] <
				(MAX_DIST - CUT_SENSE) && min_i == -1) {
			min_i = i;
		}
		if (buff[i] < (MAX_DIST - CUT_SENSE) && buff[i + 1] >= (MAX_DIST - CUT_SENSE)
				&& min_i != -1) {
			max_i = i;
		}
		if (min_i != -1 && max_i != -1 && max_i - min_i <= CUT_DIST) {
			for (j = min_i; j <= max_i; j++) {
				buff[j] = MAX_DIST;
			}
			ret = 1;
			min_i = max_i = -1;
		}
		if (max_i - min_i > CUT_DIST) min_i = max_i = -1;
	}
return ret;
}

/**
 * @brief Applies a low-pass filtering to the sampled values
 * @param buff Buffer containing the sampled data
 * @retval None
 */
void filterSamples(uint16_t *buff)
{
uint16_t i;

	for (i = 1; i < MAX_POINTS; i++) {
		if (buff[i] != MAX_DIST) {
			buff[i] = F * buff[i - 1] + (1 - F) * buff[i];
		}
	}
}
