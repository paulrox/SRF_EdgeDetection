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
 * @brief Sonic Range Finder related macros
 */
#define ADDRESS		0xE0	/* device address */
#define COMM_REG	0x00	/* command register */
#define GAIN_REG	0x01	/* gain register */
#define RANGE_REG	0x02	/* range register */
#define RES_INCH	0x50	/* ranging results in inches */
#define RES_CM		0x51	/* ranging results in centimeters */
#define RANGE		140 	/* 6 meters range */
#define MAX_GAIN	0x0C	/* Max gain 145 */
#define MAX_REG		35		/* max device register */

/**
 * @brief Display macros
 */
#define SCALE		0.15	/* display scale factor */
#define SC_WIDTH	320		/* display width */
#define SC_HEIGHT	240		/* display height */
#define X_OFFSET	30
#define Y_OFFSET	10
#define FONT_SMALL	&Font8x8
#define FONT_BIG	&Font12x12

/* converts the real coordinate into a screen coordinate */
#define SC_X(x)		x + X_OFFSET
#define SC_Y(x)		x / SCALE + Y_OFFSET

/* length of the displayed vertical axis */
#define AXIS_LENGHT	SC_HEIGHT - Y_OFFSET

/* maximum representable distance */
#define MAX_DIST	(AXIS_LENGHT - Y_OFFSET) * SCALE

/* maximum distance between two cut points */
#define CUT_DIST	10
/* CutOff sensibility */
#define CUT_SENSE	3

/**
 * @brief Other macros
 */
#define K			0.85 	/* round correlation constant */
#define F			0.75	/* samples filtering constant */

#define MAX_POINTS	290		/* max stored samples */
#define MAX_ROUNDS	1		/* max ranging rounds */

/**
 * @}
 */

#endif /* DEFINES_H_ */
