#include <18F4520.h>
#include "18F_register.h"

#device HIGH_INTS=TRUE
#device ICD=TRUE
#use delay(clock=20000000)
#use i2c(master,fast,sda=PIN_C4,scl=PIN_C3,FORCE_HW)	//Force à utiliser le Hardware du microcontrôleur
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT

//--Assignation des ports qui gèrent le capteur--//
#BIT Trigger = PORTD.2
#BIT Echo = PORTB.0
#BIT Capteur_ON = PORTD.0

#define I2CADDRESS 0X88						//Adresse de l'écran

//------------Variables globales------------//
int indexEcho=0;
int distance=0;
int index=0;
int ACK=1;

int Centaine=0;
int Dizaine=0;
int Unit=0;

//------Tableaux de commandes pour iLCD------//
int  initTab[3] = {0xAA, 0x21}; 			//Init écran
int  setCursorTab[4] = {0xAA, 0x43, 0x4B};	//Set cursor
int  writeTextTab[4] = {0xAA, 0x44, 0x54};	//Ecrire un texte - idt_tab
int	 capDistTab[20] = {0x43,0x61,0x70,0x74,0x65,0x75,0x72,0x20,0x64,0x65,0x20,0x64,0x69,0x73,0x74,0x61,0x6E,0x63,0x65};

//-----------------Fonctions-----------------//
void Config_uP(void);						//Configuration du uP
void startConnection(int charToSend);		//Démarre la connection avec l'écran
void initScreen(void);						//Réinitialise l'écran
void setCursor(void);						//Défini le point de départ du curseur - iCK
void writeEeprom(int adWrite, int Data);	//Ecris dans l'EEPROM, les paramètres sont l'adresse et la data - iEW 
void readEeprom(int adRead);				//Lis l'EEPROM, le paramètre est l'adresse de la case à lire - iER
void writeText(int textTab[]);				//Ecris un texte - iDT
void sendPulse(void);						//Envoie une pulse sur le trigger du capteur
void writeCapDistChar(void);
void writeText(void);


//------Fonction principale du code------//
void Main(void)
{
	Config_uP();

	while(1)
	{
		sendPulse();
		
		//Calculs pour ressortir chaque chiffre
		Centaine=distance/100;
		Dizaine=(distance/10)%10;
		Unit=distance%10;
		
		//Envoi de la distance en i2c sur l'écran		
		startConnection(26);
		initScreen();

		//Commande : set cursor				/!\ à remplacer par une fonction avec comme paramètres les positions
		i2c_write(0xAA);
		i2c_write(0x43);
		i2c_write(0x4B);

		//Paramètres : position x y		
		i2c_write(0x00);
		i2c_write(0xFA);	//x = 250
		i2c_write(0x00);
		i2c_write(0xDC);	//y = 220

		//Commande : write text				/!\ à remplacer par une fonction comme paramètre un tableau contenant les char (en ascii)
		i2c_write(0xAA);
		i2c_write(0x44);
		i2c_write(0x54);

		//Paramètres : chiffres à afficher + 0x00 pour terminer la transmission de caractères
		i2c_write(0x44);
		i2c_write(0x69);
		i2c_write(0x73);
		i2c_write(0x74);
		i2c_write(0x61);
		i2c_write(0x6E);
		i2c_write(0x63);
		i2c_write(0x65);
		i2c_write(0x20);
		i2c_write(0x3A);
		i2c_write(Centaine+0x30);
		delay_ms(10);
		i2c_write(Dizaine+0x30);
		delay_ms(10);
		i2c_write(Unit+0x30);
		delay_ms(10);
		i2c_write(0x00);

		i2c_stop();
		delay_ms(100);
	}	
}

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

	//Initialisation de l'écran 
	startConnection(31);
	initScreen();

	//Commande : Set cursor
	i2c_write(0xAA);
	i2c_write(0x43);
	i2c_write(0x4B);

	//Paramètres : position x y
	i2c_write(0x00);
	i2c_write(0xD7);	//x = 215
	i2c_write(0x00);
	i2c_write(0xDC);	//y = 220

	//Commande : Write text
	writeText();
		
	//Paramètres : char ascii à afficher + 0x00 pour terminer la transmission de caractères
	//Texte : "Capteur de distance"
	writeCapDistChar();
	delay_ms(2500);
	i2c_stop();

}

//Fonction : démarre la connection avec l'écran
void startConnection(int charToSend)
{
	i2c_start();
	do{ACK = i2c_write(I2CADDRESS);}	while(ACK==1);	//Atteindre l'écran
	do{ACK = i2c_write(0x05);}			while(ACK==1);	//Dire à l'écran que l'on veut lui envoyer des char
	do{ACK = i2c_write(charToSend);}	while(ACK==1);	//Dire à l'écran combien de char on lui envoie 	
}

//Fonction : réinitialise l'écran
void initScreen(void)
{
	for (index=0; index<2; index++)
		{i2c_write(initTab[index]);}
}

//Fonction : écrire un texte
void writeText(void)
{
	for (index=0; index<3; index++)
		{i2c_write(writeTextTab[index]);}
}

//Fonction : écrire "Capteur de distance" à l'allumage
void writeCapDistChar(void)
{
	for (index=0; index<19; index++)
		{i2c_write(capDistTab[index]);}
}

//Fonction : Défini un point de départ
void setCursor(void)
{
	for (index=0; index<4; index++)
		{i2c_write(setCursorTab[index]);}
}

//Fonction : envoie une impulsion sur le trigger du capteur
void sendPulse(void)
{
	Capteur_ON=1;
	Trigger=1;
	delay_us(10);
	Trigger=0;
	Capteur_ON=0;	
}

//Interruption : s'active à chaque fois qu'un signal arrive sur la pin Echo
#INT_EXT FAST
void Echo_isr()
{
	TMR0H=0xFF;
	TMR0L=0x06;
	enable_interrupts(INT_TIMER0);
}

//Timer : sert à mesurer la longueur de la pulse d'Echo et ainsi, à calculer la distance
#INT_TIMER0
void TIMER0_isr()
{
	if(Echo==1)
	{
		indexEcho++;
	 	TMR0H=0xFF;
		TMR0L=0x06;
	}
	else
	{
		disable_interrupts(INT_TIMER0);
		distance = (((indexEcho*0.000058)*340)/2)*100;		//0.000058 sert à avoir un distance en unité européenne 
		indexEcho=0;
	}
}




