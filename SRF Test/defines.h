/**
 ******************************************************************************
 * @file 	defines.h
 * @author 	Paolo Sassi
 * @date 	4 February 2016
 * @brief 	Application global defines.
 ******************************************************************************
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * @defgroup app_macros Application Macros
 * @{
 */

/**
 * @brief Display macros
 */
#define SCALE		0.15	/* display scale factor */
#define SC_WIDTH	320		/* display width */
#define SC_HEIGHT	240		/* display height */
#define X_OFFSET	30
#define Y_OFFSET	10

/* converts the real coordinate into a screen coordinate */
#define SC_X(x)		x + X_OFFSET
#define SC_Y(x)		x / SCALE + Y_OFFSET

/* length of the displayed vertical axis */
#define AXIS_LENGHT	SC_HEIGHT - Y_OFFSET

/* maximum representable distance */
#define MAX_DIST	(AXIS_LENGHT - Y_OFFSET) * SCALE

#define CUT_DIST	10

/**
 * @brief Other macros
 */
#define K			0.85 	/* round correlation constant */
#define F			0.85		/* samples filtering constant */

#define MAX_POINTS	290		/* max stored samples */
#define MAX_ROUNDS	1		/* max ranging rounds */

/**
 * @}
 */

#endif /* DEFINES_H_ */
