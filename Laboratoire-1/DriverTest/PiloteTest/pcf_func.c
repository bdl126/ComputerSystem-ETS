
#include "pcf_func.h"

extern unsigned int set_datasize(unsigned long size, myModuleTag * device){
	uint8_t data;
	
	data=ioread8(&(device->SerialPCF->LCR_REG));
	switch(size){
		case 5:
			data = data & (~LCR_WLSH & ~LCR_WLSL);
			break;
		case 6:
			data = (data & ~LCR_WLSH) | LCR_WLSL;
			break;

		case 7:
			data = (data & ~LCR_WLSL) | LCR_WLSH;
			break;

		case 8:
			data = data | (LCR_WLSH | LCR_WLSL);
			break;
		default:
			return -EOVERFLOW;
			break;
	}
	iowrite8(data,&(device->SerialPCF->LCR_REG));
	
	return 0;
}



extern unsigned int set_parity_en(unsigned long state, myModuleTag * device){
	uint8_t data;
	data=ioread8(&(device->SerialPCF->LCR_REG));
	if(state==0){
		data = data & ~LCR_PEN;
	}
	else if(state==1){
		data = data | LCR_PEN;
	} 
	else{
		return -EOVERFLOW;
	}
	iowrite8(data,&(device->SerialPCF->LCR_REG));

	return 0;
}

extern unsigned int set_parity_sel(unsigned long type, myModuleTag * device){
	uint8_t data;
	data=ioread8(&(device->SerialPCF->LCR_REG));
	if(type==0){
		data = data & ~LCR_EPS;
	}
	else if(type==1){
		data = data | LCR_EPS;
	} 
	else{
		return -EOVERFLOW;
	}
	iowrite8(data,&(device->SerialPCF->LCR_REG));

	return 0;
}

extern unsigned int set_baudrate(unsigned long baudrate, myModuleTag * device){
	uint8_t data_dlab;
	uint8_t data_low;
	uint8_t data_high;
	uint32_t temp=0;


	//CALCUL BAUDRATE AND SPLIT VALUES FOR DLL ET DLM
	if((49 < baudrate) && (baudrate < 115201) ){

		//SET DLAB TO 1
		data_dlab = ioread8(&(device->SerialPCF->LCR_REG));
		data_dlab = data_dlab | LCR_DLAB;
		iowrite8(data_dlab,&(device->SerialPCF->LCR_REG));
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
	iowrite8(data_low,&(device->SerialPCF->RBR_THR_DLL_REG));
	iowrite8(data_high,&(device->SerialPCF->IER_DLM_REG));
	//

	//SET DLAB TO 0
	data_dlab = ioread8(&(device->SerialPCF->LCR_REG));
	data_dlab = data_dlab & ~LCR_DLAB;
	iowrite8(data_dlab,&(device->SerialPCF->LCR_REG));
	//


	return 0;
}
extern unsigned int set_fifo(unsigned long depth, myModuleTag * device){
	uint8_t data;
	//enable fifo before changing it's depth
	data=ioread8(&(device->SerialPCF->FCR_IIR_REG));
	data = data | FCR_FIFOEN;
	iowrite8(data,&(device->SerialPCF->FCR_IIR_REG));

	switch(depth){
	case 1:
		data = data & (~FCR_RCVRTRM & ~FCR_RCVRTRL);
		break;
	case 4:
		data = (data & ~FCR_RCVRTRM) | FCR_RCVRTRL;
		break;

	case 8:
		data = (data & ~FCR_RCVRTRM) | FCR_RCVRTRL;
		break;

	case 14:
		data = data | (FCR_RCVRTRM | FCR_RCVRTRL);
		break;
	default:
		return -EOVERFLOW;
		break;
	}
	iowrite8(data,&(device->SerialPCF->FCR_IIR_REG));

	return 0;
}



/*
irqreturn_t my_interrupt_dev(int irq, void *device){


	return IRQ_HANDLED;
}*/



