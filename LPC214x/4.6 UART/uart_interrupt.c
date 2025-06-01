#include<lpc214x.h>

volatile char rx;  //volatile cause interrupt and hardware changable

void ser_init()  //serial innit
{
  PINSEL0=0X5;
  U0LCR=0X83;  // 1 stop bit, 8-bit character length
  U0DLL=135;  //din formula asa iasa
  U0DLM=1;		//
  U0LCR=0X03;  //(Divisor Latch Access Bit)DLAB = 0 (lock baud rate registers)
	U0IER = 0x00000003;	/* Enable THRE and RBR interrupt */
}



//Transmitter Holding Register Empty (THRE)
//THRE is set immediately upon detection of an empty UART0 THR and is
//cleared on a U0THR write.
//U0TSR - TX Shift Register
//__irq: Marks this function as an interrupt handler for Keil/ARM compilers
__irq void UART0_Interrupt(void)
{
	int iir_value;
	iir_value = U0IIR;  //Interrupt Identification Register 
	//while ( !(iir_value & 0x01) );
	if( iir_value & 0x00000004 )  //010 2a - Receive Data Available (RDA)
	{
		rx = U0RBR;  //Receiver Buffer Register
	}
		U0THR = rx;  //Transmit Holding Register - U0THR
		while( (U0LSR & 0x40) == 0 );  //TEMT is set when both U0THR and U0TSR are empty
	VICVectAddr = 0x00;
}

int main(void)
{
	ser_init();
	VICVectAddr0 = (unsigned) UART0_Interrupt;	// UART0 interrupt service routine address
	VICVectCntl0 = 0x00000026;	//interrupt source with index 6 (UART0) is enabled as the one with priority 0 (the highest)
	/*
	bit 5 - IRQslot_en  - When 1, this vectored IRQ slot is enabled, and can produce a unique ISR
	address when its assigned interrupt request or software interrupt is enabled,
	classified as IRQ, and asserted.
	*/
	
	VICIntEnable = 0x00000040;	// UART0 corresponds to bit 6: 1 << 6 = 0x40
	/*
	Interrupt Enable Register. This register controls which of the
	32 interrupt requests and software interrupts are enabled to
	contribute to FIQ or IRQ
	*/
	
	//IRQ - Interrupt Request
	//FRQ - Fast Interrupt Request
	VICIntSelect = 0x00000000;	/* UART0 configured as IRQ */
	/*
	Interrupt Select Register. This register classifies each of the
	32 interrupt requests as contributing to FIQ or IRQ
	*/
	while(1);
}

