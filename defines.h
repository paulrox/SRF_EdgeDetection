/**
 * @author Paolo Sassi
 * @date 4 February 2016
 * @brief Application global defines.
 */

#ifndef DEFINES_H_
#define DEFINES_H_

/**
 * @defgroup COMM_DEF	SRF Defines
 * @{
 */
#define ADDRESS		0xE0	/* device address */
#define COMM_REG	0x00	/* command register */
#define GAIN_REG	0x01	/* gain register */
#define RANGE_REG	0x02	/* range register */
#define RES_CM		0x51	/* ranging results in centimetres */
#define RANGE		140 	/* 6 meters range */
#define MAX_GAIN	0x02	/* Max gain 100 */
#define MAX_REG		35		/* max device register */
/**
 * @}
 */
/**
 * @defgroup DISP_DEF	Display Defines
 * @{
 */
#define SCALE		0.15	/* display scale factor */
#define SC_WIDTH	320		/* display width */
#define SC_HEIGHT	240		/* display height */
#define X_OFFSET	30
#define Y_OFFSET	10
/* converts the real coordinate into a screen coordinate */
#define SC_Y(x)		x / SCALE + Y_OFFSET
/* length of the displayed vertical axis */
#define AXIS_LENGHT	SC_HEIGHT - Y_OFFSET
/* maximum representable distance */
#define MAX_DIST	(AXIS_LENGHT - Y_OFFSET) * SCALE
/**
 * @}
 */
/**
 * @defgroup OTHERS Other Defines
 * @{
 */
#define K			0.7 	/* filter constant */
/**
 * @}
 */

#endif /* DEFINES_H_ */
