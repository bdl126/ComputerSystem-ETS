#include "circbuf.h"

extern int  initRoundbuff (unsigned int size, rbuf *roundbf){

	roundbf->bufferFull=0;
	roundbf->bufferEmpty=1;
	roundbf->bufferSize=size;
	roundbf->buffer_data=kmalloc(sizeof(char)*size, GFP_KERNEL);
	roundbf->idOut= 0;
	roundbf->idIn= 0;
	return 0;
}

EXPORT_SYMBOL_GPL(initRoundbuff);

extern int  writeRoundbuff (char x,rbuf *roundbf){

	if(roundbf->bufferFull){
		return -1;
	}
	roundbf->bufferEmpty=0;
	roundbf->buffer_data[roundbf->idIn]=x;
	roundbf->idIn=(roundbf->idIn+1)%roundbf->bufferSize;
	if(roundbf->idIn==roundbf->idOut){
		roundbf->bufferFull=1;
	}
	printk(KERN_WARNING"roundbf->idOut:%d\n",roundbf->idOut);
	printk(KERN_WARNING"roundbf->idin:%d\n",roundbf->idIn);

	return 0 ;
}

EXPORT_SYMBOL_GPL(writeRoundbuff);

extern int  readRoundbuff (char *x,rbuf *roundbf){
	printk(KERN_WARNING"roundbf->idOut:%d\n",roundbf->idOut);
	printk(KERN_WARNING"roundbf->idin:%d\n",roundbf->idIn);

	if(roundbf->bufferEmpty){
		return -1;
	}
	roundbf->bufferFull=0;
	*x=roundbf->buffer_data[roundbf->idOut];
	roundbf->idOut=(roundbf->idOut+1)%roundbf->bufferSize;
	if(roundbf->idIn==roundbf->idOut){
		roundbf->bufferEmpty=1;
	}
	return 0 ;
}

EXPORT_SYMBOL_GPL(readRoundbuff);





