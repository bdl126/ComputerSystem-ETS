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
#include <linux/device.h>
#include <linux/capability.h>
 
#include <asm/atomic.h> 
#include <asm/uaccess.h>
#include <linux/semaphore.h>
 
#include "ioctl_cmds.h"


#define DEV_MINOR		0x00
#define DEV_MINORS		0x01

MODULE_AUTHOR("Shan Meunier Bernard");
MODULE_LICENSE("Dual BSD/GPL");

// Init and Exit
int buf_init(void); 
void buf_exit(void); 

module_init(buf_init);
module_exit(buf_exit);

// Access
int buf_open (struct inode *inode, struct file *filp);
int buf_release (struct inode *inode, struct file *filp);
ssize_t buf_read (struct file *filp, char __user *ubuf, size_t count, loff_t *f_ops);
ssize_t buf_write (struct file *filp, const char __user *ubuf, size_t count, loff_t *f_ops);

// Parameters
long buf_ioctl (struct file *filp, unsigned int cmd, unsigned long arg);
 
struct buf_dev_t { 
  dev_t dev; 
  struct cdev cdev;
  struct class *cclass;
  int major;
} my_dev;
 
struct file_operations BufFops = {
  .owner = THIS_MODULE, 
  .open = buf_open, 
  .release = buf_release, 
  .read = buf_read, 
  .write = buf_write, 
  .unlocked_ioctl = buf_ioctl, 
};

