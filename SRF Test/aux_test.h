/**
 ******************************************************************************
 * @file 	aux_test.h
 * @author	Paolo Sassi
 * @date	10 February 2016
 * @brief	Application testing auxiliary functions header file.
 ******************************************************************************
 */

#ifndef AUX_TEST_H_
#define AUX_TEST_H_

#include "test.h"


#ifdef	TEST
uint8_t cutOff(uint16_t *buff);
#else
void cutOff(uint16_t *buff);
#endif	/* TEST */
void filterSamples(uint16_t *buff);


#endif /* AUX_TEST_H_ */
