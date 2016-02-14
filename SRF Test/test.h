/**
 ******************************************************************************
 * @file	test.h
 * @author	Paolo Sassi
 * @date	13 February 2016
 * @brief	Test specific macros
 ******************************************************************************
 */

#ifndef TEST_H_
#define TEST_H_

#include "defines.h"

/**
 * @brief Enables the testing
 */
#define TEST

#define NOMINAL_DIST	MAX_DIST / 2

typedef				char	char_t;
typedef signed		char	int8_t;
typedef signed		short	int16_t;
typedef signed 		int		int32_t;
typedef signed 		long	int64_t;
typedef unsigned 	char	uint8_t;
typedef unsigned 	short	uint16_t;
typedef unsigned 	int		uint32_t;
typedef unsigned 	long	uint64_t;
typedef 			float	float32_t;
typedef 			double 	float64_t;
typedef long		double 	float128_t;

#endif /* TEST_H_ */
