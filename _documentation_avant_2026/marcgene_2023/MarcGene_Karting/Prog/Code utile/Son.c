//*************************************************************************
//*  EMVS
//*  Pour la carte affichage 6 digits 7 segments
//*  Auteur: Kevin Pochon
//*  Date: 26.01.2022
//*************************************************************************
// Communiquation I2C, carte affichage 6 digits 7 segments en slave
//*************************************************************************

   
#include <18F45K22.h>
#include "18F45K22_register.h"

#DEVICE HIGH_INTS=TRUE
#device ICD=TRUE
#use delay(clock=20000000)
#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

// Déclration des fonctions
void init(void);

//interuption sur une récéption sur l'addesse 

void main(void)
{	
    init();
   
	while(1)     // une boucle infinie
	{  
       PORTD = 1;
       delay_us(1);
       PORTD = 0;
       delay_us(1);
       PORTD = 1;
	}
}

void init(void)
{
	PORTD=0x00;
	enable_interrupts(GLOBAL);  
}