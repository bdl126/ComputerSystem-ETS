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
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

char *Device="/dev/etsele_cdev0";
int main(void) {
	int fd;
	int n=0;
	int ret=0;
	char choix='c';
	char Buffout[5]="Bruno";
	char Buffin[5];
	fd=open(Device,O_RDWR);

	if(fd<0){
		printf("%d",fd);
		return -1;
	}
	while (choix!='q'){
		switch(choix)
		{
						case 'w':
							write(fd,&Buffout,5);
							choix=' ';
							break;
						case 'r':
							read(fd,&Buffin,5);
							printf ("%s\n",&Buffin);
							choix=' ';
							break;
						case 's':
							printf ("enter message:");
							scanf("%s\n",&Buffout);
							choix=' ';
							break;
						case 'h':
							printf("h:help\n");
							printf("w:write\n");
							printf("r:read\n");
							printf("s:set message\n");
							printf("q:quit\n");
							choix=' ';
							break;
						default:choix=getchar();
		}
	}


//	write(fd,&Buffout,5);
//	read(fd,&Buffin,5);
//	printf("Buffin:%s\n",Buffin);
	close(fd);
	return EXIT_SUCCESS;
}
