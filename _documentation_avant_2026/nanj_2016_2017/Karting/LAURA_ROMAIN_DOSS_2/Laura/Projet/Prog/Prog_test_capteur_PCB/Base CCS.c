//--------------------------------------------
//Ce programme permet de mesurer une distance à l'aide d'un capteur à ultrasons
//Auteur : Sidler Laura, projet 3è, 15.02.2016
//--------------------------------------------
#include <18F4520.h>
#include "18F_register.h"

#device ICD=TRUE
#device HIGH_INTS=TRUE	//sauvegarde minimale du contexte
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)	//Force à utiliser le Hardware du microcontrôleur
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST

//--Assignation des ports qui gèrent le capteur--//
#BIT Trigger = PORTD.0
#BIT Echo = PORTB.0
#BIT Capteur_ON = PORTD.1

//------------Variables globales------------//
int indexEcho=0;
int distance=0;

int Centaine=0;
int Dizaine=0;
int Unit=0;

//-----------------Fonctions-----------------//
void sendPulse(void);
void sendLength(void);

//---Fonction d'initialisation du hardware---//
void Config_uP(void)
{	
	//Ports hardware
	TRISB=0xFF;
	PORTB=0xFF;							//Interrupt Echo sur RB0
	TRISD=0X00;
	PORTD=0x00;							//PORTD en sortie pour le capteur

	//Intterupts
	enable_interrupts(INT_EXT);			//Autorise l'interruption RB0
	enable_interrupts(INT_RB);			//Autorise l'interruption RB
	disable_interrupts(INT_TIMER0);		//Désactive l'interruption timer
	ext_int_edge(0,L_to_H);				//Configure RB0 sur le flanc montant
	enable_interrupts(GLOBAL);			//Autorise globalement les interruptions

	T0CON = 0x88;

	//Bus i2c
	i2c_start();		//Démarre la communication
	i2c_write(0x50);	//Adresse de l'écran pour aller écrire dessus
	i2c_write(0xFE);	//Code pour faire une commande
	i2c_write(0x52);	//Code de la commande contraste
	i2c_write(0x1E);	//Réglage du contraste
	i2c_write(0xFE);
	i2c_write(0x53);	//Code de la commande balcklight
	i2c_write(8);		//Niveau blacklight à 8
	i2c_stop();			//Arrête la communication
}

//------Fonction principale du code------//
void Main(void)
{
	Config_uP();

	while(1)
	{
		sendPulse();		//Envoie une impulsion
		sendLength();		//Calcule la distance et l'envoie sur l'écran
	}	
}

//-----------Fonctions------------//

void sendPulse(void)
{
	Capteur_ON=1;	//Commande la base du transistor qui gère l'allumage du capteur
	Trigger=1;		//Envoie une impulsion de 10us
	delay_us(10);
	Trigger=0;
	Capteur_ON=0;	//Eteint le capteur
}

void sendLength(void)
{
	//Calculs pour ressortir chaque chiffre
	Centaine=distance/100;
	Dizaine=(distance/10)%10;
	Unit=distance%10;
	
	//Envoi de la distance en i2c sur l'écran		
	i2c_start();		
	i2c_write(0x50);	//Adresse de l'écran pour aller écrire dessus
	i2c_write(0xFE);	//Code pour faire une commande	
	i2c_write(0x51);
	i2c_write(Centaine+0x30);
	i2c_write(Dizaine+0x30);
	i2c_write(Unit+0x30);
	i2c_stop();
	delay_ms(50)		//Le delay est important pour éviter d'envoyer des pulses à la chaîne
}

//--------------------------------------------
//Chaque fois que le signal arrive sur l'echo, cette interruption s'active
//--------------------------------------------
#INT_EXT
void Echo_isr()
{
	TMR0H=0xFF;						//Charge le timer
	TMR0L=0x06;
	enable_interrupts(INT_TIMER0);	//Active le timer
}

//--------------------------------------------
//Le timer est activé lors de l'interruption
//Il permet de calculer la longueur de la pulse d'écho
//--------------------------------------------
#INT_TIMER0
void TIMER0_isr()
{
	if(Echo==1)
	{
		indexEcho++;	//Tant que Echo est à 1, incrémente une valeur
	 	TMR0H=0xFF;		//et recharge le timer
		TMR0L=0x06;
	}
	else
	{
		disable_interrupts(INT_TIMER0);					//Désactive le timer lorsque Echo vaut 0
		distance = (((indexEcho*0.000050)*340)/2)*100;	//Calcul de la distance en cm
		indexEcho=0;
	}
}




