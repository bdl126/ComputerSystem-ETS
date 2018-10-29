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
//#include <linux/errno.h> 
#include <asm/io.h>

#define F_CLK_CST 1843200/16


extern Serial_reg * SerialPCF;

//extern unsigned int pfc_init();
extern unsigned int set_datasize(unsigned long size);

extern unsigned int set_parity_en(unsigned long state);
extern unsigned int set_parity_sel(unsigned long type);
extern unsigned int set_baudrate(unsigned long baudrate);

//extern unsigned int get_bufsize(myModuleTag * dev);
//extern unsigned int set_bufsize(unsigned int newSize, myModuleTag * dev);


//extern unsigned int set_fifo(unsigned long depth);
