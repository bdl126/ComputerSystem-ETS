
#include "pcf_func.h"

extern unsigned int set_datasize(unsigned long size){
	uint8_t data;
	data=ioread8(&(SerialPCF->LCR_REG));
	switch(size){
		case 5:
			data = data & (!LCR_WLSH & !LCR_WLSL);
			break;
		case 6:
			data = (data & !LCR_WLSH) | LCR_WLSL;
			break;

		case 7:
			data = (data & !LCR_WLSL) | LCR_WLSH;
			break;

		case 8:
			data = data | (LCR_WLSH | LCR_WLSL);
			break;
		default:
			return -EOVERFLOW;
			break;
		iowrite8(data,&(SerialPCF->LCR_REG));

	}
	
	return 0;
}



extern unsigned int set_parity_en(unsigned long state){
	uint8_t data;
	data=ioread8(&(SerialPCF->LCR_REG));
	if(state==0){
		data = data & !LCR_PEN;
	}
	else if(state==1){
		data = data | LCR_PEN;
	} 
	else{
		return -EOVERFLOW;
	}
	iowrite8(data,&(SerialPCF->LCR_REG));

	return 0;
}

extern unsigned int set_parity_sel(unsigned long type){
	uint8_t data;
	data=ioread8(&(SerialPCF->LCR_REG));
	if(type==0){
		data = data & !LCR_EPS;
	}
	else if(type==1){
		data = data | LCR_EPS;
	} 
	else{
		return -EOVERFLOW;
	}
	iowrite8(data,&(SerialPCF->LCR_REG));

	return 0;
}

extern unsigned int set_baudrate(unsigned long baudrate){
	uint8_t data_dlab;
	uint8_t data_low;
	uint8_t data_high;
	uint32_t temp=0;


	//CALCUL BAUDRATE AND SPLIT VALUES FOR DLL ET DLM
	if((49 < baudrate) && (baudrate < 115201) ){

		//SET DLAB TO 1
		data_dlab = ioread8(&(SerialPCF->LCR_REG));
		data_dlab = data_dlab | LCR_DLAB;
		iowrite8(data_dlab,&(SerialPCF->LCR_REG));
		//
		temp=(F_CLK_CST/baudrate);
		data_low=(uint8_t)(temp & 0x00FF);
		data_high=(uint8_t)((temp & 0xFF00)>>8);
	} 
	else{
		return -EOVERFLOW;
	}
	//

	// WRITE DLL AND DLM VALUE	
	iowrite8(data_low,&(SerialPCF->RBR_THR_DLL_REG));
	iowrite8(data_high,&(SerialPCF->IER_DLM_REG));
	//

	//SET DLAB TO 0
	data_dlab = ioread8(&(SerialPCF->LCR_REG));
	data_dlab = data_dlab & !LCR_DLAB;
	iowrite8(data_dlab,&(SerialPCF->LCR_REG));
	//


	return 0;
}


/*extern unsigned int get_bufsize(myModuleTag * dev){

	return dev->roundTXbuf.bufferSize;
}


unsigned int set_bufsize(unsigned int newSize, myModuleTag * dev){

	resizeBuff(newSize,dev->roundTXbuf);
	resizeBuff(newSize,dev->roundRXbuf);
	return 0;
}

*/
