
#ifndef PCF_FUNC_H
#define PCF_FUNC_H



#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h> 
#include <linux/errno.h> 


#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/stat.h> 

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/cdev.h>
#include "hw.h"
#include "circbuf.h"
//#include <linux/errno.h> 
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>


#define F_CLK_CST 1843200/16

typedef struct  {
	dev_t dev;
	struct class *cclass;
	struct cdev  mycdev;
	rbuf roundTXbuf[2];
	rbuf roundRXbuf[2];
	int wr_mod[2];
	int rd_mod[2];
	Serial_reg * SerialPCF[2];
	spinlock_t dev_slock[2];
	wait_queue_head_t dev_queue[2];

}myModuleTag;


//extern myModuleTag device;

//extern unsigned int pfc_init();
extern unsigned int set_datasize(unsigned long size, myModuleTag * device, int minor_number);

extern unsigned int set_parity_en(unsigned long state, myModuleTag * device, int minor_number);
extern unsigned int set_parity_sel(unsigned long type, myModuleTag * device, int minor_number);
extern unsigned int set_baudrate(unsigned long baudrate, myModuleTag * device, int minor_number);


extern unsigned int set_fifo(unsigned long depth, myModuleTag * device, int minor_number);


#endif /* PCF_FUNC_H */
