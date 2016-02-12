/******************************************************************************
 * @file 	aux.h
 * @author	Paolo Sassi
 * @date	10 February 2016
 * @brief	Application auxiliary functions header file.
 ******************************************************************************
 */

#ifndef AUX_H_
#define AUX_H_

/**
 * @defgroup auxiliary Auxiliary Functions
 * @{
 */
/**
 * @brief Prints a string on the LCD display.
 * @param x X coordinate of the string position.
 * @param y Y coordinate of the string position.
 * @param s Pointer to the string to be displayed.
 * @param f Pointer to font data structure.
 * @retval None
 */
void WPrint(uint16_t x, uint16_t y,char *s, sFONT *f);

/**
 * @brief Prints the axis on the LCD display.
 * @param None
 * @retval None
 */
void printAxis();

/**
 * @brief Makes the calling task enter a busy wait
 * for a fixed amount of milliseconds.
 * @param msec The amount of milliseconds the task will wait.
 * @retval None
 */
void sleep(int32_t msec);


#endif /* AUX_H_ */
