/*
 ============================================================================
 Name        : Lab1.c
 Author      : Bruno DL
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "iocmd.h"

#include <linux/ioctl.h>

char *Device;
char Dev_name[40]="/dev/etsele_cdev0";
typedef enum {menu_open,menu_msg,menu_conf} state;
state currentstate=menu_open;

int main(void) {
	Device=Dev_name;

	int fd;
	unsigned long io_args=0;
	char choix='c';
	char Buffout[5]="bruno";
	char *Buffin;
/*	printf("\nh:help\n");
	printf("s:set new device name\n");
	printf("o:openfile\n");
	printf("q: quitte a tous moment\n");*/

/*	if(fd<0){
		printf("%d",fd);
		return -1;
	}*/
	Buffout[0]=(char)3;
	while (choix!='q'){
		if(currentstate==menu_msg){
			switch(choix)
			{
				case 'w':
					printf ("Buffout:%x\n",Buffout[0]);
					if(write(fd,&Buffout,1)<0){
						printf("ERROR WRITTING\n");
					}
					break;
				case 'r':
					Buffin=NULL;
					if(read(fd,&Buffin,1)<0){
						printf("ERROR READING\n");
					}
					else
					{
						printf ("Buffin:%x\n",&Buffin[0]);
					}
					break;
				case 's':
					printf ("enter message:");
					scanf("%s\n",&Buffout);
					break;
				case 'i':
					currentstate=menu_conf;
					break;
				case 'h':
					printf("\nh:help\n");
					printf("w:write\n");
					printf("r:read\n");
					printf("i:change device config\n");
					printf("s:set message\n");
					printf("c:close connection\n\n");
					break;
				case 'c':
					currentstate=menu_open;
					close(fd);
					break;
				default:
					printf("\nh:help\n");
					printf("w:write\n");
					printf("r:read\n");
					printf("i:change device config\n");
					printf("s:set message\n");
					printf("c:close connection\n\n");
					break;
			}


		}
		else if(currentstate==menu_open){
			switch (choix)
			{
				case 'o' :
					printf ("open file :%s ",Dev_name);
					fd=open(Dev_name,O_RDWR);
					if(fd<0){
						printf("\nfaile to open : %s , Error Value: %d",Dev_name,fd);
					}
					else{
						currentstate=menu_msg;
					}
					break;
				case 's':
					printf ("enter new device name:");
					scanf("%s\n",&Dev_name);
					break;
				case 'h':
					printf("\nh:help\n");
					printf("s:set new device name\n");
					printf("o:openfile\n");
					break;
				default:
					printf("\nh:help\n");
					printf("s:set new device name\n");
					printf("o:openfile\n\n");
					break;

			}

		}
		else if (currentstate==menu_conf){
			switch (choix)
			{
				case 'b':
					printf ("enter desired baudrate :");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_BAUD,io_args);
					break;
				case 'd':
					printf ("enter desired datasize :");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_DATASIZE,io_args);
					break;
				case 'p':
					printf ("enter state of parity bit enable :");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_PARITY_EN,io_args);
					break;
				case 's':
					printf ("enter desired parity selection value (odd:0, Even:1):");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_PARITY_SEL,io_args);
					break;
				case 'e':
					currentstate=menu_msg;
					break;
				case 'z':
					printf ("enter desired buf size:");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_BUF_SIZE,io_args);
					break;
				case 'g':
					io_args=ioctl(fd,SERIAL_GET_BUF_SIZE);
					printf("SERIAL_GET_BUF_SIZE:%ld\n\n",io_args);
					break;
				case 'f':
					printf ("enter desired fifo size:");
					scanf("%ld\n\n",&io_args);
					ioctl(fd,SERIAL_SET_FIFO,io_args);
					break;

				case 'h':
					printf("\nb:change baudrate of Device\n");
					printf("d:change datasize\n");
					printf("p:change Parity Enable\n");
					printf("s:change Parity selection (odd/even)\n");
					printf("z:change buffer size\n");
					printf("g:get buffer size\n");
					printf("f:change fifo depth\n");
					printf("e:end configuration and return to message menu\n");
					break;
				default:
					printf("\nb:change baudrate of Device\n");
					printf("d:change datasize\n");
					printf("p:change Parity Enable\n");
					printf("s:change Parity selection (odd/even)\n");
					printf("z:change buffer size\n");
					printf("g:get buffer size\n");
					printf("f:change fifo depth\n");
					printf("e:end configuration and return to message menu\n\n");
					break;
			}

		}

		choix=getchar();
	}
	close(fd);
	return EXIT_SUCCESS;
}
