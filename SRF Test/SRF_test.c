/**
 ******************************************************************************
 * @file	SRF_test.c
 * @author	Paolo Sassi
 * @date	13 February 2016
 * @brief	This file contains the test functions and the
 * 			CUnit initialization functions.
 ******************************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

#include "aux_test.h"
#include "defines.h"
#include "test.h"

/**
 * @defgroup test_func Testing Functions
 * @{
 */

/**
 * @defgroup cut_test	cutOff() Testing Functions
 * @{
 */

static void cutAllMax(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST;
	}

	CU_ASSERT(cutOff(buff) == 0);
}

static void cutAllZero(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = 0;
	}

	CU_ASSERT(cutOff(buff) == 0);
}

static void cutOneBound(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST;
	}
	for (i = 0; i < CUT_DIST; i++) {
		buff[100 + i] = NOMINAL_DIST;
	}

	CU_ASSERT(cutOff(buff) == 1);
}

static void cutOneEdgeOver(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST;
	}
	for (i = 0; i < CUT_DIST + 1; i++) {
		buff[100 + i] = NOMINAL_DIST;
	}

	CU_ASSERT(cutOff(buff) == 0);
}

static void cutOneEdgeUnder(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST;
	}
	for (i = 0; i < CUT_DIST - 1; i++) {
		buff[100 + i] = NOMINAL_DIST;
	}

	CU_ASSERT(cutOff(buff) == 1);
}

/**
 * @}
 */

/**
 * @defgroup filter_test filterSamples() Testing Functions
 * @{
 */

static uint8_t checkBounds(uint16_t *tmp)
{
uint8_t ret = 1;
uint16_t i;

	for (i = 0; i < MAX_POINTS; i++) {
		if (tmp[i] > MAX_DIST)	ret = 0;
	}

	return ret;
}

static void filterAllMax(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST;
	}

	filterSamples(buff);

	CU_ASSERT(checkBounds(buff));
}

static void filterAllZero(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = 0;
	}

	filterSamples(buff);

	CU_ASSERT(checkBounds(buff));
}

static void filterAllNominal(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = NOMINAL_DIST;
	}

	filterSamples(buff);

	CU_ASSERT(checkBounds(buff));
}

static void filterAllOutOfBound(void)
{
uint16_t i;
uint16_t buff[MAX_POINTS];

	for (i = 0; i < MAX_POINTS; i++) {
		buff[i] = MAX_DIST + 1;
	}

	filterSamples(buff);

	CU_ASSERT_FALSE(checkBounds(buff));
}

/**
 * @}
 */

/**
 * @}
 */

int main()
{
CU_pSuite pSuite;
CU_pTest pTest;

	CU_initialize_registry();

	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());

	pSuite = CU_add_suite("cutOff function test", NULL, NULL);

	pTest = CU_add_test(pSuite, "All points max dist.", cutAllMax);
	pTest = CU_add_test(pSuite, "All points zero dist.", cutAllZero);
	pTest = CU_add_test(pSuite, "One cut on boundary", cutOneBound);
	pTest = CU_add_test(pSuite, "One edge over the detection boundary",
			cutOneEdgeOver);
	pTest = CU_add_test(pSuite, "One edge under the detection boundary",
			cutOneEdgeUnder);

	pSuite = CU_add_suite("filterSamples function test", NULL, NULL);

	pTest = CU_add_test(pSuite, "All points max dist.", filterAllMax);
	pTest = CU_add_test(pSuite, "All points zero dist.", filterAllZero);
	pTest = CU_add_test(pSuite, "All points nominal dist.", filterAllNominal);
	pTest = CU_add_test(pSuite, "All points out of bound.", filterAllOutOfBound);

	/* Register suites. */

	CU_automated_run_tests();

	CU_cleanup_registry();

	return 0;
}
