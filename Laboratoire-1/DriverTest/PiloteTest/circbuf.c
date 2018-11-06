#include "circbuf.h"

extern int  initRoundbuff (unsigned int size, rbuf *roundbf){

	roundbf->bufferFull=0;
	roundbf->bufferEmpty=1;
	roundbf->bufferSize=size;
	roundbf->buffer_data=kmalloc(sizeof(char)*size, GFP_KERNEL);
	roundbf->idOut= 0;
	roundbf->idIn= 0;
	roundbf->nb_data=0;
	return 0;
}

EXPORT_SYMBOL_GPL(initRoundbuff);

extern int  writeRoundbuff (char x,rbuf *roundbf){

	if(roundbf->bufferFull){
		return -1;
	}
	roundbf->idIn=(roundbf->idIn+1)%roundbf->bufferSize;
	roundbf->buffer_data[roundbf->idIn]=x;
	roundbf->nb_data++;
	if(roundbf->idIn==roundbf->idOut){
		roundbf->bufferFull=1;
	}
	else {
		roundbf->bufferEmpty=0;
	}


	return 0 ;
}

EXPORT_SYMBOL_GPL(writeRoundbuff);

extern int  readRoundbuff (char *x,rbuf *roundbf){

	if(roundbf->bufferEmpty){
		return -1;
	}
	roundbf->idOut=(roundbf->idOut+1)%roundbf->bufferSize;
	*x=roundbf->buffer_data[roundbf->idOut];
	roundbf->nb_data--;
	if(roundbf->idIn==roundbf->idOut){
		roundbf->bufferEmpty=1;
	}
	else {
		roundbf->bufferFull=0;
	}
	return 0 ;
}
EXPORT_SYMBOL_GPL(readRoundbuff);


extern int resizeBuff (unsigned int newSize, rbuf *roundbf){

	roundbf->buffer_data = krealloc(roundbf->buffer_data,sizeof(char)*newSize, GFP_KERNEL);
	roundbf->bufferSize = newSize;
	return 0;
}



extern unsigned int get_bufsize(rbuf * roundbuf){

	return roundbuf->bufferSize;
}


extern unsigned int set_bufsize(rbuf * roundbuf, unsigned int newSize){

	resizeBuff(newSize,roundbuf);
	return 0;
}











