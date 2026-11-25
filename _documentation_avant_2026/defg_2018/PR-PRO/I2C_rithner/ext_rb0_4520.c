//*************************************************************************
//*  EMVS
//*  Utilise la carte led en sortie
//*  Auteur: eggn
//*  Date: 17.01.2008
//*************************************************************************
// Inverse la sortie RD0 après chaque impulsion sur RB0
//*************************************************************************

   
#include <18F4520.h>
#include "18F_register.h"


#device ICD=TRUE
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST

#BIT RD0 =  PORTD.0 


//------------------------------------------------------------------------------
// Routine d'initialisation des ports du microcontrôleur
Init_uC()
{
   enable_interrupts(INT_EXT);      	// autorise l'interruptionRB0 (INTERRUPT0)
   ext_int_edge(H_to_L);               // configure RB0 sur flanc H->L
   enable_interrupts(GLOBAL);          // autorise globalement les interruptions

  
   PORTD=TRISD=0;			// configure le PORTD en sortie sur les LEDs
   TRISB=0x01;				// configure le PORTB en entrée vers le poussoir

   i2c_start();
   i2c_write(0x50);
   i2c_write(0xFE);
   i2c_write(0x52);
   i2c_write(0x1E);
   i2c_write(0xFE);
   i2c_write(0x53);
   i2c_write(8);
   i2c_stop();

}


//------------------------------------------------------------------------------
// Chaque transition H->L de l'entrée RB0, la routine suivante est exécutée
#INT_EXT
RB0_isr()
{
	i2c_start();
	i2c_write(0x50);
	i2c_write(0xFE);
	i2c_write(0x51);

		i2c_stop();
}


//===============================================================================
main(void)
{
   Init_uC();

	do
	{

		i2c_start();
		i2c_write(0x50);
/*
		i2c_write('S');
		i2c_write('a');
		i2c_write('l');
		i2c_write('u');
		i2c_write('t');
		i2c_write(' ');
		i2c_write('b');
		i2c_write('i');
		i2c_write('e');
		i2c_write('n');
		i2c_write('f');
		i2c_write('a');
		i2c_write('b');
		i2c_write('1');
		i2c_write('2');
		i2c_write('3');
		i2c_write(' ');
		i2c_write(0xFE);
		i2c_write(0x45);
		i2c_write(0x40);
		i2c_write('M');
		i2c_write('D');
		i2c_write('R');
		i2c_write(' ');
		i2c_write('L');
		i2c_write('O');
		i2c_write('L');
		i2c_write(' ');
		i2c_write('x');
		i2c_write('D');
		i2c_write(' ');
		i2c_write(' ');
		i2c_write(0xFE);
		i2c_write(0x47);
*/
		i2c_write(0b10111100);		
		i2c_stop();

		delay_ms(50);
		
		

   }while(1);
}





  
  
