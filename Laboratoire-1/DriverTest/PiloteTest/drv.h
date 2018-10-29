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

#include "circbuf.h"



#define DEV_MINOR 0x00
#define DEV_MINOR_LAST 0x02

//int PiloteVar = 12;
int WRMOD=0;
int RDMOD=0;
//module_param(PiloteVar, int, S_IRUGO);
//EXPORT_SYMBOL_GPL(PiloteVar);

typedef struct  {
int  flags;
dev_t dev;
struct class *cclass;
struct cdev  mycdev;
//rbuf roundTXbuf;
//rbuf roundRXbuf;

}myModuleTag;

//access
static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static int module_open(struct inode *inode, struct file *filp);
static int module_release(struct inode *inode, struct file *filp);

// ioctl
static ssize_t module_ioctl ( struct file *filp, unsigned int cmd, unsigned long args);


struct file_operations myModule_fops = {
	.owner 	 	= THIS_MODULE,
	.write	 	= module_write,
	.read	 	= module_read,
	.open	 	= module_open,
	.release 	= module_release,
	.unlocked_ioctl		= module_ioctl,
};



static int  pilote_init (void);
static void pilote_exit (void);

module_init(pilote_init);
module_exit(pilote_exit);
