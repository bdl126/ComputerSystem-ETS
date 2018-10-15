#ifndef IOCTL_H
#define IOCTL_H

#include <linux/ioctl.h>


#define MAGIC_VAL 'R'

#define IOCTL_GET_NUM_DATA _IO(MAGIC_VAL, 0)

#define IOCTL_GET_NUM_READERS _IO(MAGIC_VAL, 1)

#define IOCTL_GET_BUF_SIZE _IO(MAGIC_VAL, 2)

#define IOCTL_SET_BUF_SIZE _IOW(MAGIC_VAL, 3, unsigned long)


#endif
