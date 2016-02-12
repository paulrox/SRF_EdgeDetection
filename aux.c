/******************************************************************************
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

void WPrint(uint16_t x, uint16_t y,char *s, sFONT *f)
{
	LCD_SetTextColor(Black);
	LCD_SetFont(f);
	LCD_DisplayStringXY(x, y, s);
}

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
		WPrint(0, y_mark - 4, s, FONT_SMALL);
		value += 15 * SCALE;
	}
}

void sleep(int32_t msec) {
	EventMaskType ev;
	SetRelAlarm(AlarmLCDsleep, msec, 0);
	while(GetEvent(LCDController, &ev) == E_OK && ev != sleep_event); /* busy wait */
	ClearEvent(sleep_event);
}

