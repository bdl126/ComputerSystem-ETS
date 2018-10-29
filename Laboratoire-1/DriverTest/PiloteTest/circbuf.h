#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <linux/slab.h> 

typedef struct { 
	unsigned int bufferFull; 
	unsigned int bufferEmpty; 
  	unsigned int idOut;
  	unsigned int idIn;
  	unsigned int bufferSize;
  	unsigned char *buffer_data; 
}rbuf;


extern int  writeRoundbuff (char x, rbuf*);
extern int  readRoundbuff (char *x, rbuf*); 
extern int  initRoundbuff (unsigned int size, rbuf*);
extern unsigned int get_bufsize(rbuf * roundbuf);
extern unsigned int set_bufsize(rbuf * roundbuf, unsigned int newSize);
extern int resizeBuff (unsigned int newSize, rbuf *roundbf);


#endif /* CIRCBUF_H */

