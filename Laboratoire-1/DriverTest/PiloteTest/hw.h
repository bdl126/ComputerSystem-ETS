#ifndef HW_H
#define HW_H


#define SerialHardware_Base_Addr0	0xc030
#define SerialHardware_Base_Addr1	0xc020

#define SerialHardware_IRQ_Addr0	20
#define SerialHardware_IRQ_Addr1	21


#define Number_of_Reg			0x08

// reg IER
#define IER_ERFBI	0x01
#define IER_ETFBEI	0x02
// reg IER

//reg FCR
#define FCR_RCVRTRM 	0x80
#define FCR_RCVRTRL 	0x40
#define FCR_RCVRRE 	0x02
#define FCR_FIFOEN 	0x01
//reg FCR

//reg LCR
#define LCR_DLAB 	0x80
#define LCR_EPS 	0x10
#define LCR_PEN 	0x08
#define LCR_STB 	0x04
#define LCR_WLSH 	0x02
#define LCR_WLSL 	0x01
//REG LCR


//REG LSR
#define LSR_TEMT 	0x40
#define LSR_THRE 	0x20
#define LSR_FE 		0x08
#define LSR_PE 		0x04
#define LSR_OE 		0x02
#define LSR_DR 		0x01
//REG LSR

typedef struct {
	uint8_t RBR_THR_DLL_REG;
	uint8_t	IER_DLM_REG;
	uint8_t	FCR_IIR_REG;
	uint8_t	LCR_REG;
	uint8_t	MCR_REG;
	uint8_t	LSR_REG;
	uint8_t MSR_REG;
	uint8_t	SCR_REG;
}Serial_reg;

#endif /* HW_H */

