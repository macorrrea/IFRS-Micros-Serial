#include <p18f87j10.h>
#include "config_bits.h"


//RBPU=0;

//Led's
#define PIN_LED0   			PORTBbits.RB0
#define PIN_LED1   			PORTBbits.RB1
#define PIN_LED2   			PORTBbits.RB2
#define PIN_LED3   			PORTBbits.RB3

//Estados dos leds
#define LED_ACIONADO			0
#define LED_DESACIONADO			1

//Botoes
#define PIN_BOT_AUMENTA		PORTBbits.RB4
#define PIN_BOT_DIMINUI		PORTBbits.RB5

//Estados dos botoes
#define PIN_BOT_AUMENTA_ACIONADO		0
#define PIN_BOT_AUMENTA_DESACIONADO		1
#define PIN_BOT_DIMINUI_ACIONADO		0
#define PIN_BOT_DIMINUI_DESACIONADO		1

unsigned char serial_rx_byte;

//protótipos de funções
void io_int (void);
void serial_int (void);
//void Config_Duty_Cycle (char x);

#pragma code high_vector=0x8
void high_int (void)
{
	_asm goto io_int _endasm
}
#pragma code

#pragma code low_vector=0x18
void low_int (void)
{
	_asm goto serial_int _endasm
}
#pragma code

int bt1=0;

#pragma interrupt io_int
void io_int (void)
{
	if (bt1!=0)
		bt1=0;
	else
		bt1=1;
	// Operador ternário
 // bt1 = (bt1 !=0 ? 0 : 1);
}

#pragma interrupt serial_int
void serial_int (void)
{
	if (PIR1bits.RC1IF==1)
		{
			serial_rx_byte=RCREG1;
			PIR1bits.RC1IF=0;
			if (bt1==0)
				bt1=1;
			else
				bt1=0;
		}
}

int serial_setup(void)
{
	RCSTA1bits.SPEN=0; //Desabilita a serial.
	TRISCbits.TRISC7=1;
	TRISCbits.TRISC6=0;
	TXSTA1bits.TX9=0;
	TXSTA1bits.SYNC=0;
	TXSTA1bits.BRGH=1;
	RCSTA1bits.RX9=0;
	BAUDCON1bits.WUE=0;
	BAUDCON1bits.ABDEN=0;
	BAUDCON1bits.BRG16=0;
	SPBRGH1=0;
	SPBRG1=64;
	RCSTA1bits.CREN=1;
	RCSTA1bits.SPEN=1;
	
	PIR1bits.RC1IF=0;
	IPR1bits.RC1IP=0;
	PIE1bits.RC1IE=1; // Desliga a interrupcao
	
}


void main (void)
{


	//I/O's
	PORTB = 0x30;
	TRISB = 0x31;	

#if 1
	
	INTCONbits.GIE=0;
	RCONbits.IPEN=1;
	INTCONbits.PEIE=1;
	//INTCONbits.INT0IE=1;
	//INTCONbits.INT0IF=0;

	/* Inicializa enquanto as interrupções estão desabilitadas.  */
	bt1=0;
	
	INTCONbits.GIE=1;
	
#endif

	serial_setup();

	// TODO: USER CODE!!
    while(1)
    {
		{
		}
		if (bt1==1)
			PIN_LED1=LED_ACIONADO;
		else
			PIN_LED1=LED_DESACIONADO;
	}
}
