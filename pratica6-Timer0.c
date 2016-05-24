#include <p18f87j10.h>


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

//protótipos de funções
void io_int (void);
void tmr_int (void);
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
	_asm goto tmr_int _endasm
}
#pragma code

int bt1=0;

void tmr0_refresh(void)
{
	TMR0H = 0x9e;
	TMR0L = 0x58;
}

#pragma interrupt io_int
void io_int (void)
{
	INTCONbits.INT0IF=0;
	if (bt1!=0)
		bt1=0;
	else
		bt1=1;
	// Operador ternário
	//bt1 = (bt1 !=0 ? 0 : 1);
}


char tmr_500ms;


#pragma interrupt tmr_int
void tmr_int (void)
{
	INTCONbits.TMR0IF=0;
	tmr0_refresh();
	if (tmr_500ms>49)
	{
		tmr_500ms=0;
		if (bt1!=0)
			bt1=0;
		else
			bt1=1;
	}
	else tmr_500ms++;
}


void main (void)
{


	//I/O's
	PORTB = 0x30;
	TRISB = 0x31;	


	
	T0CON=0;
	T0CONbits.PSA=1;
	
	tmr0_refresh();	
	
	INTCONbits.GIE=0;
	INTCONbits.TMR0IE=1;
	INTCONbits.TMR0IF=0;
	INTCON2bits.TMR0IP=0; // prioridade baixa
	
	RCONbits.IPEN=1;

	/* Inicializa enquanto as interrupções estão desabilitadas.  */
	bt1=0;
	tmr_500ms=0;
	
	T0CONbits.TMR0ON=1;
	INTCONbits.GIE=1;
	INTCONbits.PEIE=1;
	

	// TODO: USER CODE!!
    while(1)
    {
		if (bt1==0)
			PIN_LED1=LED_DESACIONADO;
		else
			PIN_LED1=LED_ACIONADO;
	}
}