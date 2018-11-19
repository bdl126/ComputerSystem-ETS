
#ifndef IOCTLCMD_H
#define IOCTLCMD_H

#include <linux/ioctl.h>


#define IOCTL_GET			0x10
#define IOCTL_SET			0x20
#define IOCTL_STREAMON			0x30
#define IOCTL_STREAMOFF			0x40
#define IOCTL_GRAB			0x50
#define IOCTL_PANTILT			0x60
#define IOCTL_PANTILT_RESEST		0x70


#define HAUT				0
#define BAS				1
#define GAUCHE				2
#define DROITE				3


#define PIPE_ENDPOINT			0

#define STREAM_REQUEST			0x0b
#define TILT_REQUEST			0x01
#define TILT_RESET			0x03


#endif /* IOCTLCMD_H */

