//=====================================================================================
//
//	Author				: Cytron Technologies		
//	Project				: Interface Free Controller-Main Board (IFC-MB00)
//	Project description	: Template1 for IFC-MB00. This template will help IFC user
//						  to start writing program for IFC system 
//  Date				: 21 April 2008
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_cp.h"

//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================


//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
void uart_write(unsigned char data);
unsigned char uart_read(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	
	//--------------------------------------------------------------------------------
	//	User is advised not to change or remove the initialization function called 
	// 	call initialization function of the setting in program
	init();						
	//--------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write the program here
	//--------------------------------------------------------------------------------



}

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	unsigned char temp;
	
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	// the setting for UART communication 
	// User is advised NOT to change the setting here
	// uart
	TRISCbits.TRISC6=0;		// transmit pin
	TRISCbits.TRISC7=1;		// receive pin
	TXSTAbits.TX9=0;		// select 8-bit transmission
	TXSTAbits.TXEN=1;		// transmit enable
	TXSTAbits.SYNC=0;		// asynchoronous mode
	TXSTAbits.SENDB=0;		// sync Break transmission completed
	TXSTAbits.BRGH=0;		// low speed baud rate
	RCSTAbits.SPEN=1;		// serial port enable
	RCSTAbits.RX9=0;		// select 8-bit reception
	RCSTAbits.CREN=1;		// enable receiver
	SPBRG=64;				// SPBRG = Fosc/(64*baudrate) - 1  [Fosc=40MHz]
							// SPBRG = 64 (9600) (default)
	temp=RCREG;				// clear receive buffer			
	temp=RCREG;				// clear receive buffer
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}

void uart_write(unsigned char data)
{
	// this is uart_write function to write a character to PC over the serial port
	// DB9 connector.
	// this function can be call by type : uart_write(xxxx);
	// user can replace the 'xxxx' with a 8-bit data that will be send to PC
	// make sure the TX jumper on IFC-MB00 is set to PC to enable connection from 
	// microcontroller to PC
	while (PIR1bits.TXIF == 0);
	TXREG = data;	
}

unsigned char uart_read(void)
{
	// this is uart_write function to read a character from PC over the serial port
	// DB9 connector.
	// this function can be call by type : temp=uart_read();
	// this function will wait for a byte of data from PC and return the byte to the 
	// "temp" variable
	// make sure the RX jumper on IFC-MB00 is set to PC to enable connection from 
	// microcontroller to PC
	while(PIR1bits.RCIF==0);	
	return RCREG;
}	


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

