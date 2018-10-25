/*
 * iocmd.h
 *
 *  Created on: Oct 25, 2018
 *      Author: AM98710
 */

#ifndef IOCMD_H_
#define IOCMD_H_

#include <linux/ioctl.h>

#define SERIAL_MAGIC_NUM 'B'

#define SERIAL_SET_BAUD		_IOW(SERIAL_MAGIC_NUM, 0 ,int) 	//50 A 115200

#define SERIAL_SET_DATASIZE	_IOW(SERIAL_MAGIC_NUM, 1 ,int) 	//5 A 8 BIT

#define SERIAL_SET_PARITY	_IOW(SERIAL_MAGIC_NUM, 2 ,int) 	//0 PAS DE PARITE
								//1 PARITE IMPAIRE
								//2 PARITE PAIRE

#define SERIAL_SET_BUF_SIZE	_IOW(SERIAL_MAGIC_NUM, 3 ,int)	//
#define SERIAL_GET_BUF_SIZE	_IOR(SERIAL_MAGIC_NUM, 4 ,int)


#define SERIAL_IOC_MAXNUMBER		10




#endif /* IOCMD_H_ */
