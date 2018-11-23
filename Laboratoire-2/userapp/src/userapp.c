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
#include "userapp.h"
#include <linux/ioctl.h>

char *Device;
char Dev_name[40]="/dev/etsele_cdev0";
state currentstate=menu_open;

int main(void) {
	Device=Dev_name;

	int fd;
	unsigned long io_args=0;
	char choix='c';
	char *Buffin;
	int open_flag = O_RDWR ;

	while (choix!='q'){
		if(currentstate==menu_msg){
			switch(choix)
			{

				case 'r':
					/*Buffin=NULL;
					if(read(fd,&Buffin,size_to_rw)<0){
						printf("ERROR READING\n");
						printf("ERROR: %s\n", strerror(errno));
					}
					else
					{
						printf ("Buffin:%s\n",&Buffin);
					}*/
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
					fd=open(Dev_name,open_flag);
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
				case 's':
					printf ("1:streamon \n 0:streamoff\n");
					scanf("%ld",&io_args);
					if(io_args == 1){
						printf ("IOCTL_STREAMON\n");
						if((ioctl(fd,IOCTL_STREAMON))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
					}
					else {
						printf ("IOCTL_STREAMOFF\n");
						if((ioctl(fd,IOCTL_STREAMOFF))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}

					}
					break;
				case 'e':
					currentstate=menu_msg;
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
