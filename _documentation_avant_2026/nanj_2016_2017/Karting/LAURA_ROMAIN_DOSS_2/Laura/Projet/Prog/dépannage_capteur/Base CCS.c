#include <18F4520.h>
#include "18F_register.h"


#device ICD=TRUE
#use delay(clock=20000000)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST

#BIT Trigger = PORTB.1
#BIT Echo = PORTB.0

void init(void)
{	
	TRISB=PORTB=0x00;
}

void Main(void)
{
	init();

	while(1)
	{
		Trigger = 1;
		delay_us(10);
		Trigger = 0;
		delay_ms(1);
	}	
}
