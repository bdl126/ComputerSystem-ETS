/*
 * Display.c
 *
 *  Created on: Nov 2, 2018
 *      Author: AM98710
 */

#include "Display.h"




void displayhelp(state currentstate){

	if(currentstate == menu_open){
		printf("\nh:help\n");
		printf("s:set new device name\n");
		printf("o:openfile\n");
		printf("f:change open config of file\n");
		printf("b:change flag block or noblock of file\n");
		printf("q:to quit\n\n");

	}
	else if (currentstate == menu_msg){
		printf("\nh:help\n");
		printf("w:write\n");
		printf("z:change size of read and write\n");
		printf("r:read\n");
		printf("i:change device config\n");
		printf("s:set message\n");
		printf("c:close connection\n");
		printf("q:to quit\n\n");

	}
	else if (currentstate == menu_conf){
		printf("\nb:change baudrate of Device\n");
		printf("d:change datasize\n");
		printf("p:change Parity Enable\n");
		printf("s:change Parity selection (odd/even)\n");
		printf("z:change buffer size\n");
		printf("g:get buffer size\n");
		printf("f:change fifo depth\n");
		printf("e:end configuration and return to message menu\n");
		printf("q:to quit\n\n");


	}

}
