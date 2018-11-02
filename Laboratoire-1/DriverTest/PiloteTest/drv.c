#include "drv.h"
#include "ioctlcmd.h"

MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");

myModuleTag device[2];


static ssize_t module_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {
	int minor_number=iminor(filp->f_path.dentry->d_inode);
	uint8_t err=0;
	uint8_t toto=0;
	int i=0;	
	int data_red=8;
	char  Tab[8];
	//set_interrup_en(0,&(device[minor_number]));
	spin_lock(&(device[minor_number].dev_slock));
	for(i=0;i<count;i++){
		if(readRoundbuff(&Tab[i],&(device[minor_number].roundRXbuf))==-1){
			if(i == 0){
				// if no data has been read 
				spin_unlock(&(device[minor_number].dev_slock));
				return -EFAULT;
			}
			else{
			data_red=i-1;
			}
		};
	}
	spin_unlock(&(device[minor_number].dev_slock));
	if((err=raw_copy_to_user(buf, Tab, min(data_red, (int)count)))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;
	}
	//set_interrup_en(1,&(device[minor_number]));
//	raw_copy_to_user(buf, &SerialPCF->LCR_REG, count);
	//printk(KERN_WARNING"Pilote READ 1: (toto:%x)\n",toto);
	//toto=inb(SerialHardware_Base_Addr+7);
	//printk(KERN_WARNING"Pilote READ 2: (toto:%x)\n",toto);
	//toto=ioread8(&(device[minor_number].SerialPCF->LCR_REG));
	//toto=SerialPCF->SCR_REG;
	//raw_copy_to_user(buf, Tab, count);
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
	printk(KERN_WARNING"Pilote READ min(data_red, (int)count): %d\n",min(data_red, (int)count));
	printk(KERN_WARNING"Pilote READ data_red: %d\n",min(data_red, (int)count));
	printk(KERN_WARNING"Pilote READ (int)count: %d\n",(int)count);
	printk(KERN_WARNING"Pilote READ minor_number: %d\n",minor_number);
   return 0;
}

static ssize_t module_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {
	int minor_number=iminor(filp->f_path.dentry->d_inode);
	int i=0;
	int data_red=8;
	char  Tab[8];
	uint8_t err=0;
	uint8_t toto=0;
	/*if((err=raw_copy_from_user(&toto, buf, count))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;	
	}*/
	//printk(KERN_WARNING"Pilote write 1: (toto:0x%x)\n",SerialPCF->SCR_REG);
	//raw_copy_from_user(Tab, buf, count);
	//SerialPCF->SCR_REG=toto;
	//iowrite8(toto,&(device[minor_number].SerialPCF->LCR_REG));
	//printk(KERN_WARNING"Pilote write 2: (toto:0x%x)\n",SerialPCF->SCR_REG);

	/*if((err=access_ok(VERIFY_READ,buf,count))){
		printk(KERN_WARNING"Pilote:acces not ok");
		return -ENOTTY;
	}
	__get_user(buf,&SerialPCF->SCR_REG);*/

	//outb(toto,SerialHardware_Base_Addr+7);
	//printk(KERN_WARNING"Pilote write 3: (toto:%x)\n",&SerialPCF->SCR_REG);

	//raw_copy_from_user(&SerialPCF->LCR_REG, buf, count);
	//set_interrup_en(0,&(device[minor_number]));
	if((err=raw_copy_from_user(Tab, buf, min(data_red, (int)count)))){
		printk(KERN_ALERT"BYTE TO BE COPIED: (err:%x)\n",err);
		return -EAGAIN;
	}
	spin_unlock(&(device[minor_number].dev_slock));
	for(i=0;i<count;i++){
		if(writeRoundbuff(Tab[i],&(device[minor_number].roundTXbuf))==-1){
			return -EFAULT;
		};
	}
	spin_unlock(&(device[minor_number].dev_slock));
	//set_interrup_en(1,&(device[minor_number]));
   printk(KERN_WARNING"Pilote WRITE : %d",minor_number);
   return 0;
}

static int module_open(struct inode *inode, struct file *filp) {
	int err=0;
	int minor_number=iminor(filp->f_path.dentry->d_inode);
	int interrupt_state=1;
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
	

	set_interrup_en(interrupt_state,&(device[minor_number]));




   return 0;
}

static int module_release(struct inode *inode, struct file *filp) {

	int minor_number=iminor(filp->f_path.dentry->d_inode);
	int interrupt_state=0;
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
	
	set_interrup_en(interrupt_state,&(device[minor_number]));



   printk(KERN_WARNING"Pilote RELEASE : Hello, world\n");
   return 0;
}

static ssize_t module_ioctl(struct file *filp, unsigned int cmd, unsigned long args){
	uint8_t read_data=0;	
	int debug=0;
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
	if(debug){
		read_data=ioread8(&(device[minor_number].SerialPCF->RBR_THR_DLL_REG));
		printk(KERN_WARNING"ioctl:RBR_THR_DLL_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->IER_DLM_REG));
		printk(KERN_WARNING"ioctl:IER_DLM_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->FCR_IIR_REG));
		printk(KERN_WARNING"ioctl:FCR_IIR_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->LCR_REG));
		printk(KERN_WARNING"ioctl:LCR_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->MCR_REG));
		printk(KERN_WARNING"ioctl:MCR_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->LSR_REG));
		printk(KERN_WARNING"ioctl:LSR_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->MSR_REG));
		printk(KERN_WARNING"ioctl:MSR_REG: 0x%x\n\n", read_data);

		read_data=ioread8(&(device[minor_number].SerialPCF->SCR_REG));
		printk(KERN_WARNING"ioctl:SCR_REG: 0x%x\n\n", read_data);


	}
	

	return retval;
}


static int __init pilote_init (void) {
	//int err=0;
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
		printk(KERN_WARNING "ELE784_DRIVER err0:");
		return -ENOTTY;
	}
	if(request_region(SerialHardware_Base_Addr1,Number_of_Reg,"SerialPCF16550")==NULL) {
		printk(KERN_WARNING "ELE784_DRIVER err1:");
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

	pfc_init(&(device[0]));
	pfc_init(&(device[1]));

	if((request_irq(SerialHardware_IRQ_Addr0, &my_interrupt_dev, IRQF_SHARED,"MyPilote0",&(device[0])))!=0){
		//printk(KERN_WARNING "ELE784_DRIVER err0: %d\n", err);
		return -EFAULT;
	}
	
	if((request_irq(SerialHardware_IRQ_Addr1, &my_interrupt_dev, IRQF_SHARED,"MyPilote0",&(device[1])))!=0){
		//printk(KERN_WARNING "ELE784_DRIVER err0: %d\n", err);
		return -EFAULT;
	}

   	printk(KERN_WARNING"Pilote : Hello, world\n");
  	return 0;
}



static void __exit pilote_exit (void) {
	// inverse de init
	int i =0;
	
	free_irq(SerialHardware_IRQ_Addr0,&(device[0]));
	free_irq(SerialHardware_IRQ_Addr1,&(device[1]));

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




