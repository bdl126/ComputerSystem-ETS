
#define SerialHardware_Base_Addr	0xc030//0xc020

#define SerialHardware_IRQ_Addr		0x20//0x21


#define Number_of_Reg			0x08
/*
#define RBR_REG_OFFSET			0x00
#define THR_REG_OFFSET			0x00

#define DLL_REG_OFFSET			0x00 	// (1.8432 MHz)/(16*BAUD)= valeur
						// mettre valeur en hex dans DLL

#define DLM_REG_OFFSET			0x01 	// not use

#define IER_REG_OFFSET			0x01 	// interrupts
						// etfbei transmitter holding reg empty 1
						// erbfi received data available 0
#define ERFBI	0x00
#define ETFBEI	0x01


#define IIR_REG_OFFSET			0x02 	// Not use

#define FCR_REG_OFFSET			0x02 	// RCVRTRM:RCVRTRL 7:6 FIFO DEPTH
						// RCVRRE  1	FIFO RESET
						// FIFOEN FIFO ENABLE 0 
#define RCVRTRM 0x07
#define RCVRTRL 0x06
#define RCVRRE 0x01
#define FIFOEN 0x00


#define LCR_REG_OFFSET			0x03 	// DLAB 7
						// EVEN PARITY SELECT 4
						// PARITY ENABLE 3
						// STOP BIT 2
						// WORD LENGHT 1:0
						// 00: 5 BITS -- 01: 6 BITS 
						// 10: 7 BITS -- 11: 8 BITS

#define DLAB 0x07
#define EPS 0x04
#define PEN 0x03
#define STB 0x02
#define WLSH 0x01
#define WLSL 0x00


#define MCR_REG_OFFSET			0x04 //Not use

#define LSR_REG_OFFSET			0x05	//tramsitter empty 6
						//thramistter holding register empty 5
						// RECV ERRORS 3:1
						// DATA READY 0
#define TEMT 0x06
#define THRE 0x05
#define FE 0x03
#define PE 0x02
#define OE 0x01
#define DR 0x00

#define MSR_REG_OFFSET			0x05 //Not use

#define SCR_REG_OFFSET			0x05 //Not use

*/
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



