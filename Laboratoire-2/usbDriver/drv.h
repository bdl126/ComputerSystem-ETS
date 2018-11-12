#include <linux/usb.h>
//#include <linux/usb_device.h>
//#include <linux/video.h>

#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/slab.h> 
#include <linux/errno.h> 
#include <linux/slab.h> 

#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/slab.h> 
#include <linux/types.h>
#include <linux/stat.h> 

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/cdev.h>


#define DEV_MAJOR 250
#define DEV_MINOR 0

#define USB_VENDOR_ID	0x046d 
#define USB_PRODUCT_ID 	0x08cc //08cc


static int ele784_probe(struct usb_interface *intf, const struct usb_device_id *id);
static void ele784_disconnect(struct usb_interface *intf);
static int ele784_open(struct inode *inode, struct file *filp);
static ssize_t ele784_ioctl(struct file *filp, unsigned int cmd, unsigned long args);
static ssize_t ele784_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);

struct file_operations fops = {
	.owner 		=	THIS_MODULE,
	.read 		=	ele784_read,
	.open 		=	ele784_open,
	.unlocked_ioctl	= 	ele784_ioctl,
};

struct usb_class_driver class_driver = {
	.name ="etsele_cdev%d",
	.fops=&fops,
	.minor_base =DEV_MINOR,
};

struct usb_device_id usb_device []= {
	{USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
	{USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},

};
//MODULE(usb,usb_device);
struct usb_driver udriver ={
	.name="Camera_driver_orbit",
	.probe=ele784_probe,
	.disconnect=ele784_disconnect,
	.id_table =usb_device,
	
};
