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
#include "dht_data.h"
#include <linux/ioctl.h>

char *Device;
char Dev_name[40]="/dev/etsele_cdev0";
state currentstate=menu_open;

int main(void) {
	Device=Dev_name;

	int fd;
	unsigned long io_args=0;
	char choix='c';
	int open_flag = O_RDWR ;
	FILE *foutput;
	unsigned char * inBuffer;
	unsigned char * finalBuf;
	int mySize =0;;
	inBuffer = malloc((42666)* sizeof(unsigned char));
	finalBuf = malloc((42666 * 2)* sizeof(unsigned char));
	if((inBuffer == NULL) || (finalBuf == NULL)){
		return -1;
	}
	while (choix!='q'){
		if(currentstate==menu_msg){
			switch(choix)
			{

				case 'r':
					foutput = fopen("fichier.jpg", "wb");
					if(foutput != NULL){
						// Etape #2
						printf ("IOCTL_STREAMON\n");
						if((ioctl(fd,IOCTL_STREAMON))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
						// Etape #3
						printf("IOCTL_GRAB\n");
						if((ioctl(fd,IOCTL_GRAB))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
						break;
						// Etape #4
						mySize=read(fd,inBuffer,42666);
						if(mySize<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
						// Etape #5


						printf ("IOCTL_STREAMOFF\n");
						if((ioctl(fd,IOCTL_STREAMOFF))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
						memcpy (finalBuf, inBuffer, HEADERFRAME1);
						memcpy (finalBuf + HEADERFRAME1, dht_data, DHT_SIZE);
						memcpy (finalBuf + HEADERFRAME1 + DHT_SIZE,inBuffer + HEADERFRAME1,(mySize -HEADERFRAME1));
						fwrite (finalBuf, mySize + DHT_SIZE, 1, foutput);
						fclose(foutput);
					}
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

				case 'r':
						printf ("Resetting tilt\n");
						if((ioctl(fd,IOCTL_PANTILT_RESEST))<0){
							printf("ERROR: %s\n", strerror(errno));
						}
						else{
							printf ("done\n");
						}
						break;
				case 't':
						printf ("0:HAUT \n1:BAS\n2:GAUCHE\n3:DROITE\n");
						scanf("%ld",&io_args);
						if(io_args == HAUT){
							printf ("tilt HAUT\n");
							if((ioctl(fd,IOCTL_PANTILT,io_args))<0){
								printf("ERROR: %s\n", strerror(errno));
							}
							else{
								printf ("done tilt HAUT\n");
							}
						}
						else if(io_args == BAS){
							printf ("tilt BAS\n");
							if((ioctl(fd,IOCTL_PANTILT,io_args))<0){
								printf("ERROR: %s\n", strerror(errno));
							}
							else{
								printf ("done tilt BAS\n");
							}

						}
						else if(io_args == GAUCHE){
							printf ("tilt GAUCHE\n");
							if((ioctl(fd,IOCTL_PANTILT,io_args))<0){
								printf("ERROR: %s\n", strerror(errno));
							}
							else{
								printf ("done tilt GAUCHE\n");
							}
						}
						else if(io_args == DROITE){
							printf ("tilt DROITE\n");
							if((ioctl(fd,IOCTL_PANTILT,io_args))<0){
								printf("ERROR: %s\n", strerror(errno));
							}
							else{
								printf ("done tilt DROITE\n");
							}
						}

						break;
				case 'e':
					currentstate=menu_msg;
					break;
				case 'g':
					printf("IOCTL_GRAB");
					if((ioctl(fd,IOCTL_GRAB))<0){
						printf("ERROR: %s\n", strerror(errno));
					}
					else{
						printf ("done\n");
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
