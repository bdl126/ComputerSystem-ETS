#include "buffer.h"

//function to initialize the buffer
int buf_init(void) {
  int result;

  printk(KERN_INFO "ELE784_DRIVER init\n");


  // Init driver
  // Char device allocation
  result = alloc_chrdev_region(&my_dev.dev, DEV_MINOR, DEV_MINORS, "ELE784_DRIVER");
  if (result < 0) {
    printk(KERN_WARNING "ELE784_DRIVER can't get major %d\n", result);

    return result;
  }

    printk(KERN_WARNING "ELE784_DRIVER Init: MAJOR=%d MINOR=%d", MAJOR(my_dev.dev), MINOR(my_dev.dev));
  // Device initialisation
  my_dev.cclass = class_create(THIS_MODULE, "ELE784_DRIVER");
  device_create(my_dev.cclass, NULL, my_dev.dev, &my_dev, "etsele_cdev");

  // Char Device initialisation
  cdev_init(&my_dev.cdev, &BufFops);
  my_dev.cdev.owner = THIS_MODULE;
  my_dev.cdev.ops = &BufFops;
  result = cdev_add(&my_dev.cdev, my_dev.dev, 1);
  if (result < 0) {
    printk(KERN_WARNING "ELE784_DRIVER init error: %d\n", result);
    return result;
  }

  return 0;
}

//function to free the buffer and delete the device
void buf_exit(void) {
  printk(KERN_INFO "ELE784_DRIVER exit\n");

  // Close driver
  cdev_del(&my_dev.cdev);
  device_destroy(my_dev.cclass, my_dev.dev);
  class_destroy(my_dev.cclass);
  unregister_chrdev_region(my_dev.dev, DEV_MINORS);
}

//event when the device is opened
int buf_open (struct inode *inode, struct file *filp) {
  printk(KERN_INFO "ELE784_DRIVER open\n"); 
  filp->private_data = &my_dev;
  return 0;
}

//event when the device is closed (file closed from a client)
int buf_release (struct inode *inode, struct file *filp) {
  printk(KERN_INFO "ELE784_DRIVER release\n");

  return 0;
}

//event when the client wants to read from the buffer
ssize_t buf_read (struct file *filp, char __user *ubuf, size_t count, loff_t *f_ops) {

  printk(KERN_INFO "ELE784_DRIVER read\n");


  raw_copy_to_user(ubuf, "Hello World!", min(count, 12));

  return count;
}

//event when the client wants to write to the file
ssize_t buf_write (struct file *filp, const char __user *ubuf, size_t count, loff_t *f_ops) {
  char kernel_buf[64] = {0};
  printk(KERN_INFO "ELE784_DRIVER write\n");

  
  raw_copy_from_user(kernel_buf, ubuf, min(63, count));
  printk(KERN_INFO "ELE784_DRIVER: write: %s",kernel_buf);
  return count;
}

//event in an ioctl command
long buf_ioctl (struct file *filp, unsigned int cmd, unsigned long arg) {
long retval = 0;
  switch(cmd) {
  case IOCTL_GET_NUM_DATA: //number of characters in the buffer
    retval = 33;
    break;
  default:
    retval = -ENOTTY;
    break;
}
 return retval;
}



/*int main (){
 int fd = open("/dev/etsele_cdev", O_NOBLOCK | O_RDWR);
 write(fd, "bonjour", 7);
 char buf[16]= {0};
 read(fd, buf, 12);
 printf("%s", buf);
 close(fd);
}

*/
