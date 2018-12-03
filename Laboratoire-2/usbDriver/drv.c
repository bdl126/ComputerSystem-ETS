#include "drv.h"
#include "ioctlcmd.h"


MODULE_AUTHOR("Bruno De Lafontaine");
MODULE_LICENSE("Dual BSD/GPL");
unsigned int myStatus=0;
unsigned int myLength=42666;
unsigned int myLengthUsed=0;
char * myData;
struct completion UrbComplete[5];
struct	my_usb_struct myStruct;
struct usb_interface tata;
struct urb *myUrb[5];
int nbUrbs =5;
//wait_queue_head_t UrbComplete;


module_init(pilote_init);
module_exit(pilote_exit);

static int __init pilote_init (void) {

	int result ;
	int i =0;
	for(i=0;i<5;i++){
		init_completion(&(UrbComplete[i]));
	}
	//init_waitqueue_head(&UrbComplete);
	myData=kmalloc(sizeof(char)*myLength, GFP_KERNEL);
	result= usb_register(&udriver);
	printk(KERN_WARNING "ELE784 -> pilote_init \n");

	return result;
}

static void __exit pilote_exit (void) {
	 usb_deregister(&udriver);
}


static void complete_callback(struct urb *urb){

	int ret;
	int i;	
	unsigned char * data;
	unsigned int len;
	unsigned int maxlen;
	unsigned int nbytes;
	void * mem;

	if(urb->status == 0){
		
		for (i = 0; i < urb->number_of_packets; ++i) {
			if(myStatus == 1){
				continue;
			}
			if (urb->iso_frame_desc[i].status < 0) {
				continue;
			}
			data = urb->transfer_buffer + urb->iso_frame_desc[i].offset;
			if(data[1] & (1 << 6)){
				continue;
			}
			len = urb->iso_frame_desc[i].actual_length;
			if (len < 2 || data[0] < 2 || data[0] > len){
				continue;
			}
		
			len -= data[0];
			maxlen = myLength - myLengthUsed ;
			mem = myData + myLengthUsed;
			nbytes = min(len, maxlen);
			memcpy(mem, data + data[0], nbytes);
			myLengthUsed += nbytes;
	
			if (len > maxlen) {				
				myStatus = 1; // DONE
			}
	
			if ((data[1] & (1 << 1)) && (myLengthUsed != 0)) {						
				myStatus = 1; // DONE
			}					
		}
	
		if (!(myStatus == 1)){				
			if ((ret = usb_submit_urb(urb, GFP_ATOMIC)) < 0) {
				//printk(KERN_WARNING "");
			}
		}else{
			complete(urb->context);
			printk(KERN_WARNING "ELE784 ->count ");
		}			
	}else{
		printk(KERN_WARNING "ELE784 ->complete_callback: urb->status != 0");
	}
}


int ele784_probe(struct usb_interface *intf, const struct usb_device_id *id){
	int result =0;
	struct usb_host_interface 	*iface_desc;
	struct usb_device *dev=interface_to_usbdev(intf);

	iface_desc = intf->cur_altsetting;
	printk(KERN_WARNING "ELE784 -> Probe:video interface \n");	

	if((iface_desc->desc.bInterfaceClass == CC_VIDEO) && 
		(iface_desc->desc.bInterfaceSubClass == SC_VIDEOSTREAMING)){
		usb_set_interface(dev,1,4);
		usb_set_intfdata(intf, &(myStruct.intf));
		myStruct.udev = kmalloc(sizeof(usb_device), GFP_KERNEL);
		myStruct.udev = usb_get_dev(dev);
		//init_completion(&UrbComplete);
		usb_register_dev(intf,&class_driver);
		printk(KERN_WARNING "ELE784 -> Probe:iface_desc->desc.bInterfaceNumber 0x%x\n", iface_desc->desc.bInterfaceNumber);

	}

	return result;
}

void ele784_disconnect(struct usb_interface *intf){
	struct usb_host_interface 	*iface_desc;	

	iface_desc = intf->cur_altsetting;
	if(iface_desc->desc.bInterfaceClass == CC_VIDEO && 
		iface_desc->desc.bInterfaceSubClass == SC_VIDEOSTREAMING){
		usb_deregister_dev(intf,&class_driver);
		kfree(myData);
		printk(KERN_WARNING "ELE784 -> Disconnect :video interface disconnect\n");
	}

	printk(KERN_ALERT"ELE784 -> Disconnect \n\r");
	
}

static ssize_t ele784_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos){
	struct usb_interface *interface = filp->private_data;
	struct usb_device *udev = interface_to_usbdev(interface);
	int i = 0;
	unsigned int nbbyte=0;
	unsigned int transferbyte=0;
	printk(KERN_ALERT"ELE784 -> read \n\r");

	for (i=0;i<nbUrbs;i++){
		wait_for_completion(&UrbComplete[i]);
	}

	nbbyte=raw_copy_to_user(buf, myData, myLengthUsed);
	for(i=0;i<nbUrbs;i++){
		usb_kill_urb(myUrb[i]);
		usb_free_coherent(udev,myUrb[i]->transfer_buffer_length,myUrb[i]->transfer_buffer,myUrb[i]->transfer_dma);
		usb_free_urb(myUrb[i]);
	}
	transferbyte=myLengthUsed-nbbyte;
	printk(KERN_ALERT"ELE784 -> read %d \n\r",transferbyte);
	return transferbyte;
}

int ele784_open(struct inode *inode, struct file *file){

	struct usb_interface *intf;
	int subminor;

	printk(KERN_WARNING "ELE784 -> Open \n\r");
	subminor = iminor(inode);
	intf = usb_find_interface(&udriver, subminor);
	if (!intf) {
		printk(KERN_WARNING "ELE784 -> Open: Ne peux ouvrir le peripherique");
		return -1;
	}
	file->private_data = intf;
	return 0;

}
static ssize_t ele784_ioctl(struct file *filp, unsigned int cmd, unsigned long args){
	
	struct usb_interface *intf = filp->private_data;
	struct usb_host_interface 	*cur_altsetting;
	struct usb_endpoint_descriptor endpointDesc;

	struct usb_device *dev=interface_to_usbdev(intf);
	int retval=0;
	int j,i,ret=0;
	static char Direction[4]={0};
	int myPacketSize;
	int size;
	int nbPackets;

	cur_altsetting = intf->cur_altsetting;
	endpointDesc = cur_altsetting->endpoint[0].desc;

	nbPackets=40;
	myPacketSize = le16_to_cpu(endpointDesc.wMaxPacketSize);
	size = myPacketSize * nbPackets;

	switch(cmd){
	
	case IOCTL_GET:
		printk(KERN_ALERT"ELE784 -> IOCTL_GET \n\r");
		break;
	case IOCTL_SET:
		printk(KERN_ALERT"ELE784 -> IOCTL_SET \n\r");
		break;
	case IOCTL_STREAMON:
		printk(KERN_ALERT"ELE784 -> IOCTL_STREAMON  \n\r");
		usb_control_msg(myStruct.udev, usb_sndctrlpipe(myStruct.udev,0) ,STREAM_REQUEST,
				USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE,0x0004,0x0001,NULL,0,0);
		break;
	case IOCTL_STREAMOFF:
		printk(KERN_ALERT"ELE784 -> IOCTL_STREAMOFF \n\r");
		usb_control_msg(myStruct.udev,usb_sndctrlpipe(myStruct.udev,0) ,STREAM_REQUEST,
				USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE,0x0000,0x0001,NULL,0,0);
		break;
	case IOCTL_GRAB:
		printk(KERN_ALERT"ELE784 -> IOCTL_GRAB \n\r");
		myLengthUsed=0;
		myLength=42666;
		for (i = 0; i < nbUrbs; ++i) {
		  usb_free_urb(myUrb[i]); // Pour être certain
		  myUrb[i] = usb_alloc_urb(nbPackets,GFP_KERNEL);

		  if (&(myUrb[i]) == NULL) {
		    //printk(KERN_WARNING "");		
		    return -ENOMEM;
		  }
		  myUrb[i]->transfer_buffer = usb_alloc_coherent(dev,size,GFP_KERNEL,&(myUrb[i]->transfer_dma));

		  if (&(myUrb[i]->transfer_buffer) == NULL) {
		    //printk(KERN_WARNING "");		
		    usb_free_urb(myUrb[i]);
		    return -ENOMEM;
		  }

		  myUrb[i]->dev = dev;
		  myUrb[i]->context = &UrbComplete[i];
		  myUrb[i]->pipe = usb_rcvisocpipe(dev, endpointDesc.bEndpointAddress);
		  myUrb[i]->transfer_flags = URB_ISO_ASAP | URB_NO_TRANSFER_DMA_MAP;
		  myUrb[i]->interval = endpointDesc.bInterval;
		  myUrb[i]->complete = complete_callback;
		  myUrb[i]->number_of_packets = nbPackets;
		  myUrb[i]->transfer_buffer_length = size;

		  for (j = 0; j < nbPackets; ++j) {
		    myUrb[i]->iso_frame_desc[j].offset = j * myPacketSize;
		    myUrb[i]->iso_frame_desc[j].length = myPacketSize;
		  }								
		}

		for(i = 0; i < nbUrbs; i++){
		  if ((ret = usb_submit_urb(myUrb[i],GFP_KERNEL)) < 0) {
		    printk(KERN_ALERT "ELE784 -> usb_submit_urb ERROR");		
		    return ret;
		  }
		}

		
		break;
	case IOCTL_PANTILT:
		printk(KERN_ALERT"ELE784 -> IOCTL_PANTILT \n\r");
		if(args == HAUT){
			Direction[3]=0xff;
			Direction[2]=0x80;
			Direction[1]=0x00;
			Direction[0]=0x00;
		}
		else if(args == BAS){
			Direction[3]=0x00;
			Direction[2]=0x80;
			Direction[1]=0x00;
			Direction[0]=0x00;
		}
		else if(args == DROITE){
			Direction[3]=0x00;
			Direction[2]=0x00;
			Direction[1]=0xff;
			Direction[0]=0x80;
				
		}
		else if(args == GAUCHE){
			Direction[3]=0x00;
			Direction[2]=0x00;
			Direction[1]=0x00;
			Direction[0]=0x80;
		}
		else 
		{
			return -ENOTTY;
		}
		usb_control_msg(myStruct.udev,usb_sndctrlpipe(myStruct.udev,0),TILT_REQUEST,
				USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,0x0100,0x0900,Direction,sizeof(char)*4,0);
		break;
	case IOCTL_PANTILT_RESEST:
		Direction[3]=0x00;
		Direction[2]=0x00;
		Direction[1]=0x00;
		Direction[0]=0x03;
		retval=usb_control_msg(myStruct.udev,usb_sndctrlpipe(myStruct.udev,0),TILT_REQUEST,
				USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE,0x0200,0x0900,Direction,sizeof(char),0);
		printk(KERN_ALERT"ELE784 -> IOCTL_PANTILT_RESEST  %d \n\r",retval);
		break;

	default: 
		return -ENOTTY;
	
	}

	
	return 0;
}


