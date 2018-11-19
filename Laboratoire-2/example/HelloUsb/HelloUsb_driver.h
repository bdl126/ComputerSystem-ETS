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

#include <asm/atomic.h>
#include <asm/uaccess.h>


#define DEV_MINOR       0x00
#define DEV_MINORS      0x01

MODULE_AUTHOR("Hello Usb");
MODULE_LICENSE("Dual BSD/GPL");

static struct usb_device_id usb_device_id [] = {
    {USB_DEVICE(0x0000, 0x0000)},
    {},
};
MODULE_DEVICE_TABLE(usb, usb_device_id);

static ssize_t ele784_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos);
static int ele784_open(struct inode *inode, struct file *file);
static long ele784_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t ele784_read(struct file *file, char __user *buffer, size_t count, loff_t *f_pos);
static int ele784_probe(struct usb_interface *interface, const struct usb_device_id *id);
static void ele784_disconnect (struct usb_interface *intf);

static struct usb_driver udriver = {
  .name = "HelloWorld Usb Driver",
  .probe = ele784_probe,
  .disconnect = ele784_disconnect,
  .id_table = usb_device_id,
};

static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = ele784_read,
  .open = ele784_open,
  .unlocked_ioctl = ele784_ioctl,
};

static struct usb_class_driver class_driver = {
  .name = "Hello%d",
  .fops = &fops,
  .minor_base = DEV_MINOR,
};

//module_init(ele784_init);
//module_exit(ele784_cleanup);
module_usb_driver(udriver);
