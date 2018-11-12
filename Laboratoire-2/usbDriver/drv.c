#include "drv.h"
#include "ioctlcmd.h"


MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");






static int ele784_probe(struct usb_interface *intf, const struct usb_device_id *id){
	int result =0;
	 printk(KERN_ALERT "ELE784 -> Pen drive (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
	result=usb_register_dev(intf,&class_driver);
	printk(KERN_ALERT"ELE784 -> probe \n\r");

/*
	int retval;

	device = interface_to_usbdev(intf);

	if ((retval = usb_register_dev(intf, &class_driver)) < 0)
	{
		printk(KERN_ERR "Not able to get a minor for this device.");
	}
	else
	{
		printk(KERN_INFO "Minor obtained: %d\n", interface->minor);
	}

	return retval;*/

}

static void ele784_disconnect(struct usb_interface *intf){

	usb_deregister_dev(intf,&class_driver);
	printk(KERN_ALERT"ELE784 -> Disconnect \n\r");

}

static ssize_t ele784_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
	printk(KERN_ALERT"ELE784 -> read \n\r");
	return 0;
}

static int ele784_open(struct inode *inode, struct file *file){

	struct usb_interface *intf;
	int subminor;
	printk(KERN_WARNING "ELE784 -> Open \n\r");
	subminor = iminor(inode);
	intf = usb_find_interface(&udriver, subminor);
	if (!intf) {
	printk(KERN_WARNING "ELE784 -> Open: Ne peux ouvrir le peripherique");
	return -ENODEV;
	}
	file->private_data = intf;
	return 0;

}

static ssize_t ele784_ioctl(struct file *filp, unsigned int cmd, unsigned long args){

	switch(cmd){
	
	case IOCTL_GET:
		printk(KERN_ALERT"ELE784 -> IOCTL_GET \n\r");
		break;
	case IOCTL_SET:
		printk(KERN_ALERT"ELE784 -> IOCTL_SET \n\r");
		break;
	case IOCTL_STREAMON:
		printk(KERN_ALERT"ELE784 -> IOCTL_STREAMON \n\r");
		break;
	case IOCTL_STREAMOFF:
		printk(KERN_ALERT"ELE784 -> IOCTL_STREAMOFF \n\r");
		break;
	case IOCTL_GRAB:
		printk(KERN_ALERT"ELE784 -> IOCTL_GRAB \n\r");
		break;
	case IOCTL_PANTILT:
		printk(KERN_ALERT"ELE784 -> IOCTL_PANTILT \n\r");
		break;
	case IOCTL_PANTILT_RESEST:
		printk(KERN_ALERT"ELE784 -> IOCTL_PANTILT_RESEST \n\r");
		break;

	default: 
		return -ENOTTY;
	
	}

	printk(KERN_ALERT"ELE784 -> ioctl \n\r");
	
	return 0;
}


module_usb_driver(udriver)


