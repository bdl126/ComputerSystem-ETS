
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
	rbuf roundTXbuf;
	rbuf roundRXbuf;
	int wr_mod;
	int rd_mod;
	int interrupt_flag;
	Serial_reg * SerialPCF;
	spinlock_t dev_slock;
	wait_queue_head_t dev_queue;

}myModuleTag;


//extern myModuleTag device;

extern unsigned int set_datasize(unsigned long size, myModuleTag * device);

extern unsigned int set_parity_en(unsigned long state, myModuleTag * device);
extern unsigned int set_parity_sel(unsigned long type, myModuleTag * device);
extern unsigned int set_baudrate(unsigned long baudrate, myModuleTag * device);
extern unsigned int set_interrup_en(unsigned long state, myModuleTag * device);


extern unsigned int set_fifo(unsigned long depth, myModuleTag * device);
extern unsigned int pfc_init(myModuleTag * device);


extern irqreturn_t my_interrupt_dev(int irq, void *dev);


#endif /* PCF_FUNC_H */
