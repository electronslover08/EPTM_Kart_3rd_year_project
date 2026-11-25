/*--------------------------------------------------------------*/
// 	Programmation_écran_test
//	Auteur : Sidler Laura
//	Date : 11.12.2015
//	Hardware : Carte i2c et écran tactile
//	Description : Test sur écran tactile
/*--------------------------------------------------------------*/
#include <18F4520.h>
#include "18F_register.h"

#device ICD=TRUE
#device HIGH_INTS=TRUE										//sauvegarde minimale du contexte
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C1, scl=PIN_C2, FORCE_HW)	//Force à utiliser le Hardware du microcontrôleur 
															//J'ai utilisé les pin RC1 et RC2 car ils sont hardwarement à côté
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN

#define I2CADDRESS 0X88						//Adresse de l'écran

//Variables globales
int oldData;
int newData;
int i=0;
int index=0;
int backlight=7;
int oldBacklight=7;  						//Les deux valeurs doivent être égales, au début.
int ACK=1;									//L'état par défaut est 1

//Tableaux de commandes iLCD
int  dataRecuTab[5] = {5,5,5,5,5};
int  initTab[3] = {0xAA, 0x21}; 			//Init écran
int  startPointTab[4] = {0xAA, 0x43, 0x4B};	//Set cursor
int  writeTextTab[4] = {0xAA, 0x44, 0x54};	//Ecrire un texte - idt_tab
char textTab[70];							//Tableau de caractère pour écrire sur l'écran

//Fonctions
void Config_uP(void);						//Configuration du uP
void startConnection(int charToSend);		//Démarre la connection avec l'écran
void initScreen(void);						//Réinitialise l'écran
void startPointScreen(int xH, int xL, int yH, int yL);		//Défini le point de départ du curseur - iCK
void setImg(int img);						//Affiche une image	- iG
void writeEeprom(int adWrite, int Data);	//Ecris dans l'EEPROM, les paramètres sont l'adresse et la data - iEW 
void readEeprom(int adRead);				//Lis l'EEPROM, le paramètre est l'adresse de la case à lire - iER
void fontChoice(int font);					//Choisi avec quelle police on écrit sur l'écran - iAF
void writeText(int textTab[]);				//Ecris un texte - iDT

void main(void)
{
	Config_uP();

	while(1)     // une boucle infinie
	{			
	}
}

/*==========================================================================
====Fonction     :Config_uP                                             ====
====Auteur : Sidler Laura 												====
====Date : 16.12.2014                                    				====
====------------------------------------------------------------------	====
==== Configure les ports du microprocesseur								====
====------------------------------------------------------------------	====
==== ADCON : registres du convertisseur AD/DA							====
==== TRIS  : ports d'entrée ou sortie du PIC 							====
====------------------------------------------------------------------	====
==== return : void												        ====
============================================================================*/
void Config_uP(void)
{
	//Timer
	enable_interrupts(GLOBAL);
	ext_int_edge(L_to_H);		//Configure RB0 sur le flanc montant	
    enable_interrupts(INT_EXT); // autorise l'interruptionRB0 (INTERRUPT0)

	//PORTS
	TRISA = 0x00;
	TRISB = 0b00000111;
	TRISC = 0x00;
	TRISD = 0x00;

	//Convertisseur AD /!\ changer la configuration d'une fois que j'aurais fait le schéma ou utiliser la même configuration hardware /!\
	ADCON0=0x01;		//Configuration des registre de conversion A/D	
	ADCON1=0x0E;		//PORTA en analogique (pot)
	ADCON2=0x1D;

	/*Lire la valeur de l'EEPROM
	startConnection(5);
	readEeprom(0x00);					//iER
	i2c_stop();
	//lis l'information désirée
	backlight = dataRecuTab[2];			//savoir la valeur
	oldBackLight = backlight;*/	
}//------------------------------------------------------------------------------

//Chaque transition H->L de l'entrée RB0, la routine suivante est exécutée
#INT_EXT
void RB0_isr(void)
{
	startConnection(40);
	startPointScreen(0x00,0xAA,0x00,0x96);
	strcpy(textTab, "ce code marche !");
	writeText(textTab);
	i2c_stop();
}

//Fonction : défini un point de départ sur l'écran
void startPointScreen(int xH, int xL, int yH, int yL) //x, y = axe.
{
	for(index=0; index<3; index++)
		{i2c_write(startPointTab[index]);}
	i2c_write(xH);
	i2c_write(yH);
	i2c_write(xL);
	i2c_write(yL);
}

//Fonction : réinitialise l'écran
void initScreen(void)
{
	for (index=0; index<2; index++)
		{i2c_write(initTab[index]);}
}

//Fonction : démarre la connection avec l'écran
void startConnection(int charToSend)
{
	i2c_start();
	do{ACK = i2c_write(I2CADDRESS);}	while(ACK==1);	//Atteindre l'écran
	do{ACK = i2c_write(0x05);}			while(ACK==1);	//Dire à l'écran que l'on veut lui envoyer des char
	do{ACK = i2c_write(charToSend);}	while(ACK==1);	//Dire à l'écran combien de char on lui envoie 	
}

/*Fonction : choix de la police d'écriture
void fontChoice(int font)
{
	for(index=0; index<3; index++)
		{i2c_write(}
}*/

//Fonction : écrire une phrase
void writeText(int textTab[])
{
	for(index=0; index<3; index++)
		{i2c_write(writeTextTab[index]);}
	index=0;
	while(textTab[index]!=0)
		{i2c_write(textTab[index]);
		 index++;}
	i2c_write(0x00);
}