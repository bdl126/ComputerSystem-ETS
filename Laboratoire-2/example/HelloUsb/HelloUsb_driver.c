#include "HelloUsb_driver.h"



// Event when the device is opened
int ele784_open(struct inode *inode, struct file *file) {
  struct usb_interface *intf;
  int subminor;

  printk(KERN_WARNING "ELE784 -> Open\n");

  subminor = iminor(inode);
  intf = usb_find_interface(&udriver, subminor);
  if (!intf) {
    printk(KERN_WARNING "ELE784 -> Open: Ne peux ouvrir le peripherique\n");
    return -ENODEV;
  }

  file->private_data = intf;
  return 0;
}

// Probing USB device
int ele784_probe(struct usb_interface *interface, const struct usb_device_id *id) {
  struct usb_host_interface *iface_desc;

  printk(KERN_INFO "ELE784 -> Probe\n");

  return 0;
}

void ele784_disconnect (struct usb_interface *intf) {
  printk(KERN_INFO "ELE784 -> Disconnect\n");

  usb_deregister_dev(intf, &class_driver);
}

long ele784_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  struct usb_interface *interface = file->private_data;
  struct usb_device *udev = interface_to_usbdev(interface);


  return 0;
}

ssize_t ele784_read(struct file *file, char __user *buffer, size_t count, loff_t *f_pos) {
  struct usb_interface *interface = file->private_data;
  struct usb_device *udev = interface_to_usbdev(interface);


  return 0;
}
