#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/usb.h>
#include <linux/usb/video.h>
#include <linux/completion.h>
#include "usbvideo.h"

#include <asm/atomic.h>
#include <asm/uaccess.h>


#define DEV_MAJOR 250
#define DEV_MINOR 0

#define USB_VENDOR_ID	0x046d 
#define USB_PRODUCT_ID 	0x0994 //08cc 0994

static struct usb_device_id usb_device []= {
	{USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID)},
	{},

};
MODULE_DEVICE_TABLE (usb,usb_device);

static int ele784_probe(struct usb_interface *intf, const struct usb_device_id *id);
static void ele784_disconnect(struct usb_interface *intf);
static int ele784_open(struct inode *inode, struct file *filp);
static ssize_t ele784_ioctl(struct file *filp, unsigned int cmd, unsigned long args);
static ssize_t ele784_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);/*
static void __exit cam_exit(void);
static int __init cam_init(void);*/

static struct usb_driver udriver ={
	.name="Camera_driver_orbit",
	.probe=ele784_probe,
	.disconnect=ele784_disconnect,
	.id_table =usb_device,
	
};

static struct file_operations fops = {
	.owner 		=	THIS_MODULE,
	.read 		=	ele784_read,
	.open 		=	ele784_open,
	.unlocked_ioctl	= 	ele784_ioctl,
};

static struct usb_class_driver class_driver = {
	.name ="etsele_cdev%d",
	.fops=&fops,
	.minor_base =DEV_MINOR,
};

struct my_usb_struct {
    struct usb_interface *intf;
};

module_usb_driver(udriver)
