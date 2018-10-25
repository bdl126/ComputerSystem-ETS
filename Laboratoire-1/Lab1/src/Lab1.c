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

	char choix='c';
	char Buffout[5]="bruno";
	char *Buffin;
	printf("\nh:help\n");
	printf("s:set new device name\n");
	printf("o:openfile\n");
	printf("q: quitte a tous moment");

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
									printf ("setParity: %d",SERIAL_SET_PARITY);
									ioctl(fd,SERIAL_SET_PARITY,0);
									break;
								case 'h':
									printf("\nh:help\n");
									printf("w:write\n");
									printf("r:read\n");
									printf("i:change device config\n");
									printf("s:set message\n");
									printf("c:close connection\n");
									break;
								case 'c':
									close(fd);
									currentstate==menu_open;
									printf("\nh:help\n");
									printf("s:set new device name\n");
									printf("o:openfile\n");
									break;

								default:
										break;
			}
		}
		else if(currentstate==menu_open){
			switch (choix){
						case 'o' :
							printf ("open file :%s ",Dev_name);
							fd=open(Dev_name,O_RDWR);
							if(fd<0){
								printf("faile to open %s",Dev_name);
							}
							else{
								currentstate=menu_msg;
								printf("\nh:help\n");
								printf("w:write\n");
								printf("r:read\n");
								printf("i:change device config\n");
								printf("s:set message\n");
								printf("c:close connection\n");
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

							break;

			}

		}
		else if (currentstate==menu_conf){


		}
		choix=getchar();
	}
	return EXIT_SUCCESS;
}
