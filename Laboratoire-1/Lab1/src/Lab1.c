/*
 ============================================================================
 Name        : Lab1.c
 Author      : Bruno DL
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "iocmd.h"
#include "Display.h"
#include "Lab1.h"
#include <linux/ioctl.h>

char *Device;
char Dev_name[40]="/dev/etsele_cdev0";
state currentstate=menu_open;

int main(void) {
	Device=Dev_name;

	int fd;
	unsigned long io_args=0;
	char choix='c';
	char Buffout[8]="bru";
	char *Buffin;
	unsigned int size_to_rw=3;
	int open_flag = O_RDWR | O_NONBLOCK;
	int tmp=0;

	while (choix!='q'){
		if(currentstate==menu_msg){
			switch(choix)
			{
				case 'w':
					printf ("writting:%s\n",Buffout);
					if(write(fd,&Buffout,size_to_rw)<0){
						printf("ERROR WRITTING\n");
						printf("buffer might be full\n");
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 'r':
					Buffin=NULL;
					if(read(fd,&Buffin,size_to_rw)<0){
						printf("ERROR READING\n");
						printf("ERROR: %s\n", strerror(errno));
					}
					else
					{
						printf ("Buffin:%s\n",&Buffin);
					}
					break;
				case 'z':
					printf ("enter size to read:");
					scanf("%d",&size_to_rw);
					printf ("done\n");
					break;
				case 's':
					printf ("enter message: ");
					scanf("%s",&Buffout);
					break;
				case 'i':
					currentstate=menu_conf;
					break;
				case 'h':
					displayhelp(currentstate);
					break;
				case 'c':
					currentstate=menu_open;
					close(fd);
					break;
				default:
					displayhelp(currentstate);
					break;
			}


		}
		else if(currentstate==menu_open){
			switch (choix)
			{
				case 'o' :
					printf ("open file :%s\n ",Dev_name);
					fd=open(Dev_name,open_flag);
					printf("open_flag:0x%x\n",open_flag);
					if(fd<0){
						printf("\nfailed to open : %s , Error Value: %d\n\n",Dev_name,fd);
						printf("ERROR: %s\n", strerror(errno));

					}
					else{
						currentstate=menu_msg;
					}
					break;
				case 's':
					printf ("enter new device name:");
					scanf("%s",&Dev_name);
					break;
				case 'f':
					printf ("Choose flag to open this file \n");
					printf ("0 : O_RDONLY\n");
					printf ("1 : O_WRONLY \n");
					printf ("2 : O_RDWR\n");
					scanf("%d",&tmp );
					if((tmp == 1) || (tmp == 2) || (tmp == 0))
					{
						open_flag=(int)tmp;
					}
					else
					{
						printf ("WRONG VALUE ENTER, TRY AGAIN\n");
					}
					break;
				case 'b':
					printf ("block or noblock \n");
					printf ("0 : O_NONBLOCK \n");
					printf ("1 :O_BLOCK\n");
					scanf("%d",&tmp );
					if(tmp){
						open_flag= open_flag & ~O_NONBLOCK;
					}
					else
					{
						open_flag= open_flag | O_NONBLOCK;
					}
					break;

				case 'h':
					displayhelp(currentstate);
					break;
				default:
					displayhelp(currentstate);
					break;

			}

		}
		else if (currentstate==menu_conf){
			switch (choix)
			{
				case 'b':
					printf ("enter desired baudrate :");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_BAUD,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 'd':
					printf ("enter desired datasize :");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_DATASIZE,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 'p':
					printf ("enter state of parity bit enable :");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_PARITY_EN,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 's':
					printf ("enter desired parity selection value (odd:0, Even:1):");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_PARITY_SEL,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 'e':
					currentstate=menu_msg;
					break;
				case 'z':
					printf ("enter desired buf size:");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_BUF_SIZE,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;
				case 'g':
					if((io_args=ioctl(fd,SERIAL_GET_BUF_SIZE,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					else
					{
					printf("SERIAL_GET_BUF_SIZE:%ld\n\n",io_args);
					}
					break;
				case 'f':
					printf ("enter desired fifo size:");
					scanf("%ld",&io_args);
					if((ioctl(fd,SERIAL_SET_FIFO,io_args))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					break;

				case 'h':
					displayhelp(currentstate);
					break;
				default:
					displayhelp(currentstate);
					break;
			}

		}

		choix = getchar();
		choix = tolower(choix);
	}
	close(fd);
	return EXIT_SUCCESS;
}
