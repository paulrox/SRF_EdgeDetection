/**
 ******************************************************************************
 * @file 	aux_test.c
 * @author	Paolo Sassi
 * @date	10 February 2016
 * @brief	Application testing auxiliary functions source code.
 ******************************************************************************
 */

#include <stdio.h>

#include "aux_test.h"
#include "defines.h"
#include "test.h"


/**
 * @brief Cuts off the spurious samples detected by the range
 * finder when crossing an hole.
 * @param buff Buffer containing the sampled data.
 * @retval None
 */
#ifdef	TEST
uint8_t cutOff(uint16_t *buff) \
{ \
uint8_t ret = 0;
#else
void cutOff(uint16_t *buff) \
{
#endif	/* TEST */
int16_t min_i, max_i, i, j;

	min_i = max_i = -1;
	for (i = 0; i < MAX_POINTS - 1; i++) {
		if (buff[i] == MAX_DIST && buff[i + 1] < MAX_DIST) {
			min_i = i;
		}
		if (buff[i] < MAX_DIST && buff[i + 1] == MAX_DIST && min_i != -1) {
			max_i = i;
		}
		if (min_i != -1 && max_i != -1 && max_i - min_i <= CUT_DIST) {
			for (j = min_i; j <= max_i; j++) {
				buff[j] = MAX_DIST;
			}
#ifdef	TEST
			ret = 1;
#endif	/* TEST */
			min_i = max_i = -1;
		}
	}
#ifdef	TEST
return ret;
#endif	/* TEST */
}

/**
 * @brief Applies a low-pass filtering to the sampled values
 * @param buff Buffer containing the sampled data
 * @retval None
 */
void filterSamples(uint16_t *buff)
{
uint16_t tmp[MAX_POINTS];
uint16_t i;

	tmp[0] = buff[0];
	for (i = 1; i < MAX_POINTS; i++) {
		tmp[i] = F * buff[i - 1] + (1 - F) * buff[i];
	}
	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = tmp[i];
	}
}
