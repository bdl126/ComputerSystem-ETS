#include "drv.h"
#include "ioctlcmd.h"

MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");

myModuleTag device[2];

static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	int minor_number=iminor(filp->f_path.dentry->d_inode);
	//uint8_t err=0;
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
	toto=ioread8(&(device[minor_number].SerialPCF->LCR_REG));
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
	printk(KERN_WARNING"Pilote READ : Hello, world : toto: 0x%x\n",toto);
   return 0;
}

static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
	int minor_number=iminor(filp->f_path.dentry->d_inode);
//	int i=0;
	char  Tab[10];
	//uint8_t err=0;
	uint8_t toto=0;
	/*if((err=raw_copy_from_user(&toto, buf, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;	
	}*/
	//printk(KERN_WARNING"Pilote write 1: (toto:0x%x)\n",SerialPCF->SCR_REG);
	raw_copy_from_user(&toto, buf, count);
	//SerialPCF->SCR_REG=toto;
	iowrite8(toto,&(device[minor_number].SerialPCF->LCR_REG));
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

	int minor_number=iminor(filp->f_path.dentry->d_inode);

	//verification du nombre de user
	if(device[minor_number].wr_mod==1){
		return -(ENOTTY);
	}
	if(device[minor_number].rd_mod==1){
		return -(ENOTTY);
	}
	if((filp->f_flags & O_ACCMODE)==O_WRONLY){
		device[minor_number].wr_mod=1;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDWR){
		device[minor_number].rd_mod=device[minor_number].rd_mod+1;
		device[minor_number].wr_mod=1;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDONLY){
		device[minor_number].rd_mod=1;
	}
   return 0;
}

static int module_release(struct inode *inode, struct file *filp) {

	int minor_number=iminor(filp->f_path.dentry->d_inode);

	if((filp->f_flags & O_ACCMODE)==O_WRONLY){
		device[minor_number].wr_mod=0;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDWR){
		device[minor_number].rd_mod=0;
		device[minor_number].wr_mod=0;
	}
	if((filp->f_flags & O_ACCMODE)==O_RDONLY){
		device[minor_number].rd_mod=0;
	}
	
	if(){

	}

   printk(KERN_WARNING"Pilote RELEASE : Hello, world\n");
   return 0;
}

static ssize_t module_ioctl(struct file *filp, unsigned int cmd, unsigned long args){
	int retval=0;
	int minor_number=iminor(filp->f_path.dentry->d_inode);
	printk(KERN_WARNING"minor_number:%d",minor_number);

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
			set_baudrate(args, &(device[minor_number]));
			printk(KERN_WARNING"SERIAL_SET_BAUD: %ld\n", args);
			break;
	case SERIAL_SET_DATASIZE:
			set_datasize(args, &(device[minor_number]));
			printk(KERN_WARNING"SERIAL_SET_DATASIZE: %ld\n", args);
			break;
	case SERIAL_SET_PARITY_EN:
			set_parity_en(args, &(device[minor_number]));
			printk(KERN_WARNING"SERIAL_SET_PARITY_EN: %ld\n", args);
			break;
	case SERIAL_SET_PARITY_SEL:
			set_parity_sel(args, &(device[minor_number]));
    			printk(KERN_WARNING"SERIAL_SET_PARITY: %ld\n", args);
			break;
	case SERIAL_SET_BUF_SIZE:
			set_bufsize(&(device[minor_number].roundTXbuf), args);
			set_bufsize(&(device[minor_number].roundRXbuf), args);
			printk(KERN_WARNING"SERIAL_SET_PARITY_EN: %ld\n", args);
			break;
	case SERIAL_GET_BUF_SIZE:
			retval=get_bufsize(&(device[minor_number].roundTXbuf));
			printk(KERN_WARNING"SERIAL_GET_BUF_SIZE: %d\n", retval);
			break;
	case SERIAL_SET_FIFO:
			set_fifo(args,&(device[minor_number]));
			printk(KERN_WARNING"SERIAL_SET_FIFO: %ld\n", args);
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
	result=alloc_chrdev_region(&device[0].dev, DEV_MINOR, DEV_MINOR_LAST, "MyPilote");
  	if (result < 0) {
    		printk(KERN_WARNING "ELE784_DRIVER can't get major %d\n", result);

		return result;
	}

   	device[0].cclass = class_create(THIS_MODULE, "moduleTest");

	if(request_region(SerialHardware_Base_Addr0,Number_of_Reg,"SerialPCF16550")==NULL) {
		return -ENOTTY;
	}
	if(request_region(SerialHardware_Base_Addr1,Number_of_Reg,"SerialPCF16550")==NULL) {
		return -ENOTTY;
	}
	device[0].SerialPCF=ioport_map(SerialHardware_Base_Addr0,Number_of_Reg);
	device[1].SerialPCF=ioport_map(SerialHardware_Base_Addr1,Number_of_Reg);

	for (i=0;i<DEV_MINOR_LAST;i++){
		sprintf(buf, "%s%d",DEV_BASE_NAME,i);
		printk(KERN_WARNING"buf : %s\n", buf);
		device_create(device[0].cclass, NULL, device[0].dev+i, NULL, buf);

		initRoundbuff(8,&(device[i].roundTXbuf));
	   	initRoundbuff(8,&(device[i].roundRXbuf));
		init_waitqueue_head(&(device[i].dev_queue));
		spin_lock_init(&(device[i].dev_slock));
		device[i].wr_mod=0;
		device[i].rd_mod=0;

	}

   	cdev_init(&(device[0].mycdev), &myModule_fops);
   	cdev_add(&(device[0].mycdev), device[0].dev, DEV_MINOR_LAST);

	if(request_irq(SerialHardware_IRQ_Addr0, &my_interrupt_dev, IRQF_SHARED,"MyPilote",&(device[0]))){
		return -EFAULT;
	}
	
	if(request_irq(SerialHardware_IRQ_Addr1, &my_interrupt_dev, IRQF_SHARED,"MyPilote",&(device[1]))){
		return -EFAULT;
	}
	
	
   	printk(KERN_WARNING"Pilote : Hello, world\n");
  	return 0;
}



static void __exit pilote_exit (void) {
	// inverse de init
	int i =0;
	kfree(device[0].roundTXbuf.buffer_data);
	kfree(device[1].roundTXbuf.buffer_data);
	kfree(device[0].roundRXbuf.buffer_data);
	kfree(device[1].roundRXbuf.buffer_data);

	ioport_unmap(&(device[0].SerialPCF));
	ioport_unmap(&(device[1].SerialPCF));

	release_region(SerialHardware_Base_Addr0,Number_of_Reg);
	release_region(SerialHardware_Base_Addr1,Number_of_Reg);

	cdev_del(&device[0].mycdev);
	for (i=0;i<DEV_MINOR_LAST;i++){
		device_destroy(device[0].cclass, device[0].dev+i);
	}


	class_destroy(device[0].cclass);
	 
	unregister_chrdev_region(device[0].dev, DEV_MINOR_LAST);

	printk(KERN_ALERT"Pilote : Goodbye, cruel world\n");
}




