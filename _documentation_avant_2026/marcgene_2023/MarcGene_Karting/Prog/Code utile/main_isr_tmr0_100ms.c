//*************************************************************************
//*  TestPotAileron
//*************************************************************************
//*  EMVS
//*  Utilise la carte led en sortie
//*  Auteur:Marco Genet
//*  Date:23.03.23
//*************************************************************************
// Test de la recup de l'entr�e an et convertissage enpulse de 5v
// 
//
//*************************************************************************


#include <18F45K22.h>
#include "18F45K22_register.h"

#DEVICE HIGH_INTS=TRUE
#device ICD=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH
#BIT RD0 =  PORTD.0 
#BIT RD1 =  PORTD.1 
#BIT RD2 =  PORTD.2
int1 on_off ;
//-----------------------------------------------------------------------------------
// Routine d'initialisation des ports du microcontr�leur
Init_uC()
{	
   enable_interrupts(INT_EXT);
   enable_interrupts(INT_TIMER0);       // autorise l'interruption timer0
   enable_interrupts(GLOBAL);          // autorise globalement les interruptions
   T0CON =0b10000010;
                                       //bit0,1,2   :010	PS=8
                                       //bit3   	:0 	PS utilis�
                                       //bit4		:x 	
                                       //bit5   	:0	Transition de clock
                                       //bit6		:0	Timer 16 bits   
				       //bit7		:1	Timer0 on
   TRISA = 0x3B;		// RA0: Potentiomètre entrée analogique 
	LATA = 0;			// RA1, RA3..RA5: Entrées digitales du Dip Switch
	PORTA = 0;			// RA2 sortie HP
	ANSELA = 1;			// RA6..RA7: non implémentée car utilisée pour l'oscillateur
	//PORT B en entrées digitales sauf RB3 (sortie pour 6/2G Accelerometer), RB6 et RB7 en sortie utilisée pour le debug
	TRISB = 0x37;
	LATB = 0;
	PORTB = 0;
    ANSELB = 0;
	//PORT C
	TRISC = 0x00;		// RC1..RC2: choix de la colonne en sortie digitale
	LATC = 0;			// RC3, RC4: utilisée pour SCL et SDA pour I2C laissé en sortie digitale
	PORTC = 0;			// RC5: NC et réserve laissée en sortie digitale
	ANSELC = 0;			// RC6-> RGB0, RC7-> RGB1: choix de la couleur en sorties digitales
	//PORT D en sorties digitales
	TRISD = 0x00;		// RD0..RD7: valeur des lignes de la matrice en sorties
	LATD = 0;
	PORTD = 0;
    ANSELD = 0;
	//PORT E en sorties digitales
	TRISE = 0x07;		// Entrées pour accéléromètre ou autre capteur. Ces entrées sont branchées directement sur les sorties d'un ampli OP.
	LATE = 0;
	PORTE = 0;
    ANSELE = 7;
	//Configuration du convertisseur A/D
	ADCON0=0b00000001;	// module CAD actif,channel 0: AN0 multiplexé sur l'A/D
	ADCON1=0b00000000;	// Vref+ sur VDD, Vref- sur VSS
	ADCON2=0b00011101;	// TAD = Fosc/16 car fCPU = 20MHz, ADFM=0 =>> ADRESH
}


//------------------------------------------------------------------------------------
// Routine d'interruption qui s'ex�cute chaque 100ms, 
// Inverse la sortie RD0 chaque 100ms

//====================================================================================
main(void)
{

      Init_uC();				// initialise les I/O
      do					// attend l'interruption
      {
      
      }while(1);
  
}



  
  
