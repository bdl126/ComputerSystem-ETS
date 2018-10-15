#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h> 

#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/stat.h> 

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/cdev.h>

#include "circbuf.h"

MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");

int PiloteVar = 12;
int WRMOD=0;
int RDMOD=0;
module_param(PiloteVar, int, S_IRUGO);
EXPORT_SYMBOL_GPL(PiloteVar);

extern int initRoundbuff(unsigned int, rbuf* );
extern int writeRoundbuff(char, rbuf*);
extern int readRoundbuff(char *, rbuf*);
rbuf roundbuf;

//struct myModuleTag {
int  flags;
char  Tab[10];
dev_t dev;
struct class *cclass;
struct cdev  mycdev;
//} myModuleStruct; 

static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
static int module_open(struct inode *inode, struct file *filp);
static int module_release(struct inode *inode, struct file *filp);

static struct file_operations myModule_fops = {
	.owner 	 = THIS_MODULE,
	.write	 = module_write,
	.read	 = module_read,
	.open	 = module_open,
	.release = module_release,
};



static int  pilote_init (void);
static void pilote_exit (void);

/*
Fin definition de fonctions.
*/


static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	int i=0;	

	for(i=0;i<count;i++){
		if(-1==readRoundbuff(&Tab[i],&roundbuf)){
			return -1;
		};
	}
	raw_copy_to_user(buf, Tab, count);

   printk(KERN_WARNING"Pilote READ : Hello, world\n");
   return 0;
}

static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
	int i=0;
	raw_copy_from_user(Tab, buf, count);
	for(i=0;i<count;i++){
		if(writeRoundbuff(Tab[i],&roundbuf)==-1){
			return -1;
		};
	}
   printk(KERN_WARNING"Pilote WRITE : %s",Tab);
   return 0;
}

static int module_open(struct inode *inode, struct file *filp) {
//  filp.private_data = &myModuleStruct;

//verification du nombre de user
	if(WRMOD==1){
		return -(ENOTTY);
	}
	if(RDMOD==1){
		return -(ENOTTY);
	}
	if((filp->f_flags & O_ACCMODE)==O_WRONLY){
		WRMOD=1;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDWR){
		RDMOD=RDMOD+1;
		WRMOD=1;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDONLY){
		RDMOD=1;
	}

   printk(KERN_WARNING"Pilote OPEN : Hello, world\n");
   return 0;
}

static int module_release(struct inode *inode, struct file *filp) {

	if((filp->f_flags & O_ACCMODE)==O_WRONLY){
		WRMOD=0;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDWR){
		RDMOD=0;
		WRMOD=0;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDONLY){
		RDMOD=0;
	}

   printk(KERN_WARNING"Pilote RELEASE : Hello, world\n");
   return 0;
}


static int __init pilote_init (void) {

   alloc_chrdev_region(&dev, 0, 2, "MyPilote");

   cclass = class_create(THIS_MODULE, "moduleTest");
   device_create(cclass, NULL, dev, NULL, "etsele_cdev0");
   device_create(cclass, NULL, dev+1, NULL, "etsele_cdev1");

   cdev_init(&mycdev, &myModule_fops);
   cdev_add(&mycdev, dev, 2);

   initRoundbuff(8,&roundbuf);

   printk(KERN_WARNING"Pilote : Hello, world (Pilote : %u)\n", PiloteVar);
   return 0;
}



static void __exit pilote_exit (void) {
	// inverse de init
  kfree(roundbuf.buffer_data);
  cdev_del(&mycdev);

  device_destroy(cclass, dev);
  device_destroy(cclass, dev+1);
  class_destroy(cclass);
 
  unregister_chrdev_region(dev, 2);

  printk(KERN_ALERT"Pilote : Goodbye, cruel world\n");
}



module_init(pilote_init);
module_exit(pilote_exit);

