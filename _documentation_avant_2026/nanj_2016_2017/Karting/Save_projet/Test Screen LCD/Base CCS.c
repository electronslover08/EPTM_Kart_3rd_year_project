/* =========================================================================
====    ()_()                                                  ()_()    ====
====    (o.0)         EMVs - Ecole des métiers du Valais       (0.o)    ====
====    (m)(m)                                                (m)(m)    ====
============================================================================
====                                                                    ====
====                     Programmation en langage C                     ====
====                     --------------------------                     ====
====                                                                    ====
====--------------------------------------------------------------------====
==== Nom du programme                  : Test LCD		                ====
==== Version                           : V1.0                           ====
==== Nom du(des) fichier(s) source     : 18F_register.h & Base CCS.c    ====
====--------------------------------------------------------------------====
==== Créateur (Nom, Prénom, Localité)  : Sierro Romain, Troistorrents   ====
==== Classe                            : EL3A							====
==== Date de création                  : 10.02.2016                     ====
==== Date de la dernière modification  : 10.02.2016                     ====
====--------------------------------------------------------------------====
==== Description succincte du programme:                                ====
==== ----------------------------------                                 ====
====Programme pour tester de commander l'écran LCD avec le uC			====
============================================================================
*///------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <18F4520.h>
#include "18F_register.h"


#device ICD=TRUE
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)	//Force à utiliser le hardware du microcontrôleur
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST
#define I2CADDRESS 0X88	//Adresse de l'écran

//==========================================================================
//Prototypes : 														========
//------------														========
void init(void);	//Initialisation du uC							========
void fct_1(void);	//ma fonction 1									========	
void fct_2(void);	//ma fonction 2									========
void connection(int nbrChar);	//Début de connection				========
void init_screen(void);	//Efface l'écran							========
void pos(int xh; int xl; int yh; int yl);	//Définit le point de depart====
//==========================================================================
//Variables globales :												========
//--------------------												========

//==========================================================================

void main(void)
{
	init();

	while(1)     // une boucle infinie
	{

	}
}

/* =========================================================================
====Fonction     : init		                                            ====
====Sierro Romain: 21.01.2015                                           ====
====--------------------------------------------------------------------====
====Lance la routine d'initialisation							 		====
============================================================================*/
void init(void)
{
	//PORTS
	TRISA = 0x00;
	TRISB = 0b00000011;
	TRISC = 0x00;
	TRISD = 0x00;

	ADCON0=0b00000001;	//module CAD actif,channel 0: AN0 multiplexé sur l'A/D
	ADCON1=0b00001101;	//101 :AN0 et AN1 en analogique ; 110 :AN0 en analogique
	ADCON2=0b00011101;	//TAD = Fosc/16 car fCPU = 20MHz

   	enable_interrupts(INT_EXT); //autorise l'interruption RB0    // désactive l'interruption RB0
   	ext_int_edge(0, H_to_L);        // configure RB0 sur flanc H->L
	enable_interrupts(INT_EXT1);   	// autorise l'interruption RB1
  	ext_int_edge(1, H_to_L);       	// configure RB1 sur flanc H->L
	enable_interrupts(GLOBAL);		// Autorise globalement les interruptions
}


/* =========================================================================
====Fonction     : fct_1	                                            ====
====Sierro Romain: 10.03.2016                                           ====
====--------------------------------------------------------------------====
====		 		====
============================================================================*/
void fct_1(void)
{
	init_screen();	
	//underline cursor ON
}
/* =========================================================================
====Fonction     : fct_2	                                            ====
====Sierro Romain: 10.03.2016                                           ====
====--------------------------------------------------------------------====
====		 		====
============================================================================*/
void fct_2(void)
{
	init_screen();
}

/* =========================================================================
====Fonction     : connection                                           ====
====Sierro Romain: 10.03.2016                                           ====
====--------------------------------------------------------------------====
====Débute la connection en parametrant le nombre de byte envoyé 		====
============================================================================*/
void connection(int nbrChar)
{
	i2c_start;
	do{ACK = i2c_write(I2CADDRESS);}	while(ACK==1);	//Atteindre l'écran
	do{ACK = i2c_write(0x05);}			while(ACK==1);	//Dire à l'écran que l'on veut lui envoyer des char
	do{ACK = i2c_write(charToSend);}	while(ACK==1);	//Dire à l'écran combien de char on lui envoie 	
}	

/* =========================================================================
====Fonction     : init_screen                                          ====
====Sierro Romain: 11.03.2016                                           ====
====--------------------------------------------------------------------====
====Remet l'écran à 0											 		====
============================================================================*/
void init_screen(void)
{
	i2c_write(0xAA);
	i2c_write(0x21);
}

/* =========================================================================
====Fonction     : pos			                                        ====
====Sierro Romain: 11.03.2016                                           ====
====--------------------------------------------------------------------====
====Définit la position de départ								 		====
============================================================================*/
void pos(int xh; int xl; int yh; int yl)
{
	i2c_write(0xAA);	//Commande de l'écran
	i2c_write(0x43);	//??????
	i2c_write(0x4B);	//Blinking cursor ON
	i2c_write(xh);
	i2c_write(xl);
	i2c_write(yh);
	i2c_write(yl);
}

//Chaque transition H->L de l'entrée RB0, la routine suivante est exécutée
#INT_EXT
void RB0_isr(void)
{
	fct_1();
}

//Chaque transition H->L de l'entrée RB0, la routine suivante est exécutée
#INT_EXT1
void RB1_isr(void)
{
	fct_2();
}

