/*
 * iocmd.h
 *
 *  Created on: Oct 25, 2018
 *      Author: AM98710
 */

#ifndef IOCMD_H_
#define IOCMD_H_



#include <linux/ioctl.h>

#define IOCTL_GET				0x10
#define IOCTL_SET				0x20
#define IOCTL_STREAMON			0x30
#define IOCTL_STREAMOFF			0x40
#define IOCTL_GRAB				0x50
#define IOCTL_PANTILT			0x60
#define IOCTL_PANTILT_RESEST	0x70


#define HAUT					0
#define BAS						1
#define GAUCHE					2
#define DROITE					3


#endif /* IOCMD_H_ */

