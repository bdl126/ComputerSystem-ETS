#include "drv.h"
#include "ioctlcmd.h"
#include "hw.h"
#include <asm/io.h>

MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");

extern int initRoundbuff(unsigned int, rbuf* );
extern int writeRoundbuff(char, rbuf*);
extern int readRoundbuff(char *, rbuf*);
rbuf roundbuf;

Serial_reg * SerialPCF;
Serial_reg Serialtoto;
myModuleTag device;

static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	uint8_t err=0;
	uint8_t toto=0;
/*	int i=0;	
	char  Tab[10];
	for(i=0;i<count;i++){
		if(readRoundbuff(&Tab[i],&roundbuf)<0){
			return -EFAULT;
		};
	}
	if((err=raw_copy_to_user(buf, Tab, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;
	}
*/
//	raw_copy_to_user(buf, &SerialPCF->LCR_REG, count);
	//printk(KERN_WARNING"Pilote READ 1: (toto:%x)\n",toto);
	//toto=inb(SerialHardware_Base_Addr+7);
	//printk(KERN_WARNING"Pilote READ 2: (toto:%x)\n",toto);
	toto=ioread8(&(SerialPCF->SCR_REG));
	//toto=SerialPCF->SCR_REG;
	raw_copy_to_user(buf, &toto, count);
	/*if((err=raw_copy_to_user(buf, &toto, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;
	}*/
	/*if((err=access_ok(VERIFY_WRITE,buf,count))){
		printk(KERN_WARNING"Pilote:acces not ok");
		return -ENOTTY;
	}
	__put_user(&SerialPCF->SCR_REG,buf);*/

	//printk(KERN_WARNING"Pilote READ 3: (toto:0x%x)\n",SerialPCF->SCR_REG);
	printk(KERN_WARNING"Pilote READ : Hello, world\n");
   return 0;
}

static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
//	int i=0;
	//char  Tab[10];
	uint8_t err=0;
	uint8_t toto=0;
	/*if((err=raw_copy_from_user(&toto, buf, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;	
	}*/
	//printk(KERN_WARNING"Pilote write 1: (toto:0x%x)\n",SerialPCF->SCR_REG);
	raw_copy_from_user(&toto, buf, count);
	//SerialPCF->SCR_REG=toto;
	iowrite8(toto,&(SerialPCF->SCR_REG));
	//printk(KERN_WARNING"Pilote write 2: (toto:0x%x)\n",SerialPCF->SCR_REG);

	/*if((err=access_ok(VERIFY_READ,buf,count))){
		printk(KERN_WARNING"Pilote:acces not ok");
		return -ENOTTY;
	}
	__get_user(buf,&SerialPCF->SCR_REG);*/

	//outb(toto,SerialHardware_Base_Addr+7);
	//printk(KERN_WARNING"Pilote write 3: (toto:%x)\n",&SerialPCF->SCR_REG);

	//raw_copy_from_user(&SerialPCF->LCR_REG, buf, count);
	/*if((err=raw_copy_from_user(Tab, buf, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;
	}
	for(i=0;i<count;i++){
		if(writeRoundbuff(Tab[i],&roundbuf)==-1){
			return -EFAULT;
		};
	}
   printk(KERN_WARNING"Pilote WRITE : %s",Tab);*/
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
	int retval=0;
	if(_IOC_TYPE(cmd) != SERIAL_MAGIC_NUM){
		printk(KERN_WARNING"error serial magic num");
		return -ENOTTY;
	}
	if(_IOC_NR(cmd) > SERIAL_IOC_MAXNUMBER){
		printk(KERN_WARNING"command out of range : %d",_IOC_NR(cmd));
		return -ENOTTY;
	}
	switch(cmd){
	
	case SERIAL_SET_BAUD:
			break;
	case SERIAL_SET_DATASIZE:
			break;
	case SERIAL_SET_PARITY:
    		printk(KERN_WARNING "SERIAL_SET_PARITY: %ld\n", args);
			break;
	case SERIAL_SET_BUF_SIZE:
			break;
	case SERIAL_GET_BUF_SIZE:
			break;

	default: 
		return -ENOTTY;

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
		printk(KERN_WARNING"buf : %s\n", buf);
		device_create(device.cclass, NULL, device.dev+i, NULL, buf);
	}

   	cdev_init(&device.mycdev, &myModule_fops);
   	cdev_add(&device.mycdev, device.dev, DEV_MINOR_LAST);
	
	if(request_region(SerialHardware_Base_Addr,Number_of_Reg,"SerialPCF16550")==NULL) {
		return -ENOTTY;
	}
	SerialPCF=ioport_map(SerialHardware_Base_Addr,Number_of_Reg);
   	initRoundbuff(8,&roundbuf);
	
	
   	printk(KERN_WARNING"Pilote : Hello, world (Pilote : %u)\n", PiloteVar);
  	return 0;
}



static void __exit pilote_exit (void) {
	// inverse de init
	int i =0;
	kfree(roundbuf.buffer_data);
	ioport_unmap(&SerialPCF);
	release_region(SerialHardware_Base_Addr,Number_of_Reg);
	cdev_del(&device.mycdev);
	for (i=0;i<DEV_MINOR_LAST;i++){
		device_destroy(device.cclass, device.dev+i);
	}


	class_destroy(device.cclass);
	 
	unregister_chrdev_region(device.dev, DEV_MINOR_LAST);

	printk(KERN_ALERT"Pilote : Goodbye, cruel world\n");
}




