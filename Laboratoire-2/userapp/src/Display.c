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
		printf("q:to quit\n\n");

	}
	else if (currentstate == menu_msg){
		printf("\nh:help\n");
		printf("r:read\n");
		printf("i:change device config\n");
		printf("c:close connection\n");
		printf("q:to quit\n\n");

	}
	else if (currentstate == menu_conf){
		printf("\nh:help\n");
		printf("s:Stream on/off\n");



	}

}
