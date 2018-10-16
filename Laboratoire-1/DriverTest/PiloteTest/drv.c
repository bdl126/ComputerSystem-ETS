#include "drv.h"
#include "ioctlcmd.h"

MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");

extern int initRoundbuff(unsigned int, rbuf* );
extern int writeRoundbuff(char, rbuf*);
extern int readRoundbuff(char *, rbuf*);
rbuf roundbuf;

myModuleTag device;

static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	int i=0;	
	char  Tab[10];
	for(i=0;i<count;i++){
		if(readRoundbuff(&Tab[i],&roundbuf)<0){
			return -EFAULT;
		};
	}
	raw_copy_to_user(buf, Tab, count);

   printk(KERN_WARNING"Pilote READ : Hello, world\n");
   return 0;
}

static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
	int i=0;
	char  Tab[10];
	raw_copy_from_user(Tab, buf, count);
	for(i=0;i<count;i++){
		if(writeRoundbuff(Tab[i],&roundbuf)==-1){
			return -EFAULT;
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

static ssize_t module_ioctl(struct file *filp, unsigned int cmd, unsigned long args){
	int retval=0;;
	if(_IOC_TYPE(cmd) != SERIAL_MAGIC_NUM){
		return -ENOTTY;
	}
	if(_IOC_NR(cmd) < SERIAL_IOC_MAXNUMBER){
		return -ENOTTY;
	}

	switch(cmd){
	
	case SERIAL_SET_BAUD:
			break;
	case SERIAL_SET_DATASIZE:
			break;
	case SERIAL_SET_PARITY:
			break;
	case SERIAL_SET_BUF_SIZE:
			break;
	case SERIAL_GET_BUF_SIZE:
			break;

	default: return -ENOTTY;

	}

	return retval;
}


static int __init pilote_init (void) {
	int result=0;
	int i=0;
	char buf[15];
	result=alloc_chrdev_region(&device.dev, DEV_MINOR, DEV_MINOR_LAST, "MyPilote");
  	if (result < 0) {
    		printk(KERN_WARNING "ELE784_DRIVER can't get major %d\n", result);

		return result;
	}

   	device.cclass = class_create(THIS_MODULE, "moduleTest");

	for (i=0;i<DEV_MINOR_LAST;i++){
		sprintf(buf, "etsele_cdev%d",i);
		printk(KERN_WARNING"buf : %s)\n", buf);
		device_create(device.cclass, NULL, device.dev+i, NULL, buf);
	}

   	cdev_init(&device.mycdev, &myModule_fops);
   	cdev_add(&device.mycdev, device.dev, DEV_MINOR_LAST);

   	initRoundbuff(8,&roundbuf);

   	printk(KERN_WARNING"Pilote : Hello, world (Pilote : %u)\n", PiloteVar);
  	return 0;
}



static void __exit pilote_exit (void) {
	// inverse de init
  kfree(roundbuf.buffer_data);
  cdev_del(&device.mycdev);

  device_destroy(device.cclass, device.dev);
  device_destroy(device.cclass, device.dev+1);
  class_destroy(device.cclass);
 
  unregister_chrdev_region(device.dev, DEV_MINOR_LAST);

  printk(KERN_ALERT"Pilote : Goodbye, cruel world\n");
}


