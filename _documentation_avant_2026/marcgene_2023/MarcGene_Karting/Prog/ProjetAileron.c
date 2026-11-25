//*************************************************************************
//*  EMVS
//*  Utilise la cible EPTM avec le 18F45K22
//*  Auteur: 
//*  Date: 
//*************************************************************************
//* 
//*
//*
//*************************************************************************

#include <18F45K22.h>
#include "18F45K22_register.h"

 
 
#DEVICE HIGH_INTS=TRUE
#device ICD=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

// déclaration des bits

#BIT RE0 = PORTE.0
#BIT RD0 = PORTD.0
#BIT RB1 = PORTB.1

#BIT ADGO = ADCON0.1
#BIT ADFM = ADCON2.7
//******************************************************************************
// Déclaration des fonctions
//******************************************************************************


//*************************************************************************
//*  Fonction:	Init()
//*************************************************************************
//*  Auteur: nicegg
//*  Date: 23.11.2018
//*************************************************************************
//*  Description: Configuration basique des ports du uC pour la valise uP		
//*************************************************************************
void Init(void)
{
	// ANSELx permet de choisir l'entrée, en entrée analogique ou entrée digitale
	// sur le PORTx correspondant. L'entrée est digitale avec un 0 à la place
	// correspondante sur le port, analogique avec un 1.
	// En sortie digitale, le TRISx prime et ANSELx n'est pas pris en compte.
	
    ADCON0 = 0b00000001;
    ADCON1 = 0b00000000;
    ADCON2 = 0b00011101;
	ANSELA = 0b00101111;
	ANSELB = 0b00000000;
	ANSELC = 0b00000000;
	ANSELD = 0b00000000;
	ANSELE = 0b00000000;
	
	
	//Init PORTS	
	TRISC=0b11111111;			// pour les 8 interrupteurs
    TRISB=0b11111111;			//pour les 2 poussoirs
	TRISA=0b00101111;			// Met le PORTA en entrée
	
    TRISD= 0b00000000;       // pour les 8 LEDs
    TRISE= 0b000;        		// pour les 3 LEDs
    
    LATD=0b00000000;         // 8 LEDs à 0
    LATE=0b000;         	// 3 LEDs à 0
    
    enable_interrupts(INT_TIMER0);  // autorise l'interruption timer0
    enable_interrupts(INT_TIMER1);  // autorise l'interruption timer1
    enable_interrupts(GLOBAL);  // autorise globalement les interruptions$
           
    T0CON = 0b10000000; // timer0 en mode 16 bits et activé
    TMR0L = 0xB0;
    TMR0H = 0x3C;
   //T1CON =0b00000011; //0.5ms
                                      
    	
}	

void Adc_read(unsigned char channel)
{
    TMR0L = 0xB0;
    TMR0H = 0x3C;
    
	ADCON0 = (channel << 2) + 0b00000001;
	ADGO = 1;
	while(ADGO==1)
		continue;
    //PORTD = ADRESH;
    //PORTE = ADRESH;

}
	
#INT_TIMER0 			

TIMER0_isr()
{ 
    int mamen = 0;
    
    Adc_read(0);
    mamen = (ADRESH   / 12.75);
    
        RD0 = 1;
        delay_ms(1);
        
        if(mamen > 0)
        {
        do
            {
                
                
                delay_us(50);
                mamen --;
     
                
            }while (mamen >= 1);
        }
        
        RD0 = 0;
        
       

}

/*#INT_TIMER1    
TIMER1_isr()
{
    TMR1L = F6;
    TMR1H = 3C; //0.5ms 
}
*/


//*************************************************************************
// main
//
//*************************************************************************
void main(void)
{
   
	Init();
	while(1)
	{
    
	}
}



