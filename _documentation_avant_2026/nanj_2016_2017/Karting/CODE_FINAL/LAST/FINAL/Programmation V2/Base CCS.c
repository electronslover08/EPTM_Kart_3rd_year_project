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
==== Nom du programme                  : projet_kart	                ====
==== Version                           : V2.0                           ====
==== Nom du(des) fichier(s) source     : 18F_register.h & Base CCS.c    ====
====--------------------------------------------------------------------====
==== Créateur (Nom, Prénom, Localité)  : Sierro Romain, Troistorrents   ====
====									 Sidler Laura, Sion				====
==== Classe                            : EL3							====
==== Date de création                  : ??.??.2015                     ====
==== Date de la dernière modification  : 02.06.2016                     ====
====--------------------------------------------------------------------====
==== Description succincte du programme:                                ====
==== ----------------------------------                                 ====
====Programme pour faire fonctionner l'écran tactile du projet du kart  ====
====électrique. Doit afficher la distance captée par des capteurs ultra ====
====son. Doit afficher le niveau des batteries du karting ainsi que la  ====
====vitesse à laquel nous roulons. 										====	
============================================================================
*///------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <18F4520.h>
#include "18F_register.h"

#device HIGH_INTS=TRUE
#device ICD=TRUE
#use delay(clock=20000000)
#use i2c(master,fast,sda=PIN_C4,scl=PIN_C3,FORCE_HW)	//Force à utiliser le Hardware du microcontrôleur
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT

//--Assignation des ports qui gèrent le capteur--//
#BIT Trigger=PORTD.1	//Pin du capteur où est envoyée une impulsion
#BIT Echo=PORTB.2		//Pin du capteur où revient l'echo du signal envoyé
#BIT ADGO=ADCON0.1		//Bit pour le fonctionnement de convertisseur A/D
#BIT RD7=PORTD.7
#BIT RD6=PORTD.6
//#BIT Commande2=PORTD.4
#DEFINE RI 1
#DEFINE AnalCourant 0
#DEFINE AnalTension 1

#define I2CADDRESS 0X88						//Adresse de l'écran
int reinitialise[3] = {0xAA, 0x21};
int ick_tab [4] = {0xAA, 0x43, 0x4B};
int ig_tab[3] = {0xAA, 0x47};
int itw_tab[4] = {0xAA, 0x54, 0x57};
int ith_tab[4] = {0xAA, 0x54, 0x48};
int ita_tab[5] = {0xAA, 0x54, 0x41, 0xFF};
int iaf_tab[4] = {0xAA, 0x41, 0x46};
int idt_tab[4] = {0xAA, 0x44, 0x54};
int iew_tab[4] = {0xAA, 0x45, 0x57};
int ier_tab[4] = {0xAA, 0x45, 0x52};
int iit_tab[5] = {0xAA, 0x49, 0x3F, 0x54};
int iid_tab[5] = {0xAA, 0x49, 0x3F, 0x44};
int ici_tab[4] = {0xAA, 0x43, 0x49};
int icb_tab[4] = {0xAA, 0x43, 0x42};
int iDe_tab[4] = {0xAA,0x44, 0x65};
int iDC_tab[4] = {0xAA, 0x44, 0x43};
int indexTIME;
int index;
int GraphVoltage_tab[15] = {255,250,245,240,230,220,210,200,180,150,130,110,90,70,50};
int ACK=1;

//==========================================================================
//Tableaux de commandes pour iLCD :									========
//---------------------------------									========
int Tab_Phrase[80];
//==========================================================================

void StartConnection(int NbrCharSend);	//commence la connection, dit le nbr de byte à envoyer
void reinit(void);	//réinitialise l'écran
void iCK(int xH, int xL, int yH, int yL);	//définis un point de départ
void iG(int img);	//affiche un image
void iTW(int yH, int yL);	//définis la largeur du champ tactile depuis le point de départ
void iTH(int xH, int xL);	//définis la longueur du champ tactile depuis le point de départ
void iTA(int nom);	//défini le nom du champ tactile
void iAF(int ChoixEcritH, int ChoixEcritL);	//dit avec quelle écriture on écrit le futur texte
void iDT(int phrase[]);	//écris un texte
void iDe(int end_xh,int end_xl, int end_yh, int end_yl); //Efface une zone de l'écran
void writeLine(int end_xh,int end_xl, int end_yh, int end_yl); //Fais une ligne
void iDC(int RadiusHigh,int RadiusLow); //Cercle
void init(void); //Initialise tous les ports
void speedScreen(void);
int etat=0; //Permet de choisir quel image a afficher dans le timer, elle se change dans le main
int DataRecu[5] = {5,5,5,5,5};

void BlockRead(void);	//lis des données de l'écran
void ALERT(void);	//interruptions de l'écran quand il doit envoyer des données
void DrawGraphVoltage(void);
int OldData;
int NewData;
void VoltageScreen(void); //Affiche la page des tensions
void TouchFieldMain(void);//Tactile main
void writeText(void); //Ecris eKarting
void startReading(void); //Demarre la lecture
void writeTab(void); //Ecris les tableaus du main (lignes)
void sendPulse(void);

		double PosY;
		double PosX;
		int indexFor;
		double value;

int Cent;
int Dix;
int Unit;
int tension;
int courant;
int uri;
int idelay;
int indexEcho;
int distance;
void Main(void)

{
	init();

	OldData = 0x00;
	NewData = 0x46;
	while(1)
	{
		while(DataRecu[0] != 0){ BlockRead();}


		if(OldData != NewData){	//la valeur a changé ==> appui sur l'écran sur un autre bouton
			switch(NewData){
		//	reinit();
		//affichages de base
				case 0x46:	
					//	reinit();
							writeTab();						
							writeText();
							TouchFieldMain();
							etat=1;


					break;
				case 0x44:
							etat=2;
							VoltageScreen();
							DrawGraphVoltage();

		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x00, 0xCF);	//Positionne le curseur
		strcpy(Tab_Phrase, "URi:");
		iDT(Tab_Phrase);		
		i2c_stop();	

		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x01, 0x1F);	//Positionne le curseur
		strcpy(Tab_Phrase, "Ibatteries:");
		iDT(Tab_Phrase);		
		i2c_stop();	

		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x01, 0x6F);	//Positionne le curseur
		strcpy(Tab_Phrase, "Ub vide:");
		iDT(Tab_Phrase);		
		i2c_stop();	
					break;

					case 0x33:

					speedScreen();
					etat=3;
					break;
				default: etat=0;
			}
			
			OldData = NewData;
			
		}
	}
}

/*==========================================================================
====Fonction     : Config_uP	                                        ====
====Sierro Romain: 11.03.2016                                           ====
====--------------------------------------------------------------------====
====Lance la routine d'initialisation du microprocesseur		 		====
============================================================================*/
void init(void)
{	
	//Ports hardware
	TRISA=TRISA|0b00010011;	//RA0,1 et 4 en entrée
	TRISB=0xFF;		//PORTB en entrée
	TRISD=0X00;		//PORTD en sortie pour le capteur
	PORTD=0b00111000;	//Commande des capteurs à 1				

	//Configuration des registres
	ADCON0=0b00000001;	//module CAD actif,channel 0: AN0 multiplexé sur l'A/D
	ADCON1=0b00001101;	//AN0 et AN1 en analogique
	ADCON2=0b00011101;	//TAD = Fosc/16 car fCPU = 20MHz

	//Intterupts
	disable_interrupts(INT_EXT2);		//Autorise l'interruption RB2
	ext_int_edge(2,L_to_H);				//Configure RB2 sur le flanc montant
	enable_interrupts(INT_EXT);		//Autorise l'interruption RB2
	ext_int_edge(0,L_to_H);	
	enable_interrupts(INT_TIMER0);      // autorise l'interruption timer0		//Configure RB2 sur le flanc montant
	disable_interrupts(INT_TIMER1);
	enable_interrupts(GLOBAL);			//Autorise globalement les interruptions
	
	
	T1CON=0b10000001;	//Timer de 50µs
	T0CON=0b10000110; //Timer 1s
}
/* =========================================================================
====Fonction     : Adc_read                                             ====
====Sierro Romain: 07.01.2014                                           ====
====--------------------------------------------------------------------====
==== Fonction: int Adc_read(int channel)
==== Déscription: Convertit en digital 8 bits le canal analog.          ====
==== passé en param. Paramètres d'entrée: channel: soit le canal        ====
==== à convertir Paramètres de sortie: valeur num. 8 bits de l'entrée   ====
==== analog. AN0                                                        ====
============================================================================ */
int adc_read(int channel)
{
	ADCON0 = (channel << 2) + 0b00000001;

	ADGO = 1;
	while(ADGO==1)
		continue;
	return (ADRESH);
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: commence la connection, dit le nbr de byte à envoyer
//-------------------------------------------------------------------------
void StartConnection(int NbrCharSend)
{
	// lis la valeur de la luminosité de l'EEPROM puis on dit que c'est OldBlackLight et blacklight (les 2 doivent être égales au début)
	i2c_start();
	do{ ACK = i2c_write(I2CADDRESS);} while(ACK == 1);
	// envoie l'information du nombre de caractère que l'on va envoyer
	do{ ACK = i2c_write(0x05);} while(ACK == 1);
	do{ ACK = i2c_write(NbrCharSend);} while(ACK == 1);
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: réinitialise l'écran
//-------------------------------------------------------------------------
void reinit(void)
{
	for(index=0; index<2; index++){	i2c_write(reinitialise[index]);}
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis un point de départ
//-------------------------------------------------------------------------
void iCK(int xH, int xL, int yH, int yL)
{
	for(index=0; index<3; index++){	i2c_write(ick_tab[index]);}
	i2c_write(xH);
	i2c_write(xL);
	i2c_write(yH);
	i2c_write(yL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: affiche un image
//-------------------------------------------------------------------------
void iG(int img)
{
	for(index=0; index<3; index++){	i2c_write(ig_tab[index]);}
	i2c_write(img);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis la largeur du champ tactile depuis le point de départ
//-------------------------------------------------------------------------
void iTW(int yH, int yL)
{
	for(index=0; index<3; index++){	i2c_write(itw_tab[index]);}
	i2c_write(yH);
	i2c_write(yL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis la longueur du champ tactile depuis le point de départ
//-------------------------------------------------------------------------
void iTH(int xH, int xL)
{
	for(index=0; index<3; index++){	i2c_write(ith_tab[index]);}
	i2c_write(xH);
	i2c_write(xL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: défini le nom du champ tactile
//-------------------------------------------------------------------------
void iTA(int nom)
{
	for(index=0; index<4; index++){	i2c_write(ita_tab[index]);}
	i2c_write(nom);
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: dit avec quelle écriture on écrit le futur texte
//-------------------------------------------------------------------------
void iAF(int ChoixEcritH, int ChoixEcritL)
{
	for(index=0; index<3; index++){	i2c_write(iaf_tab[index]);}
	i2c_write(ChoixEcritH);
	i2c_write(ChoixEcritL);
}


//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: écris un texte
//-------------------------------------------------------------------------
void iDT(int phrase[])
{
	for(index=0; index<3; index++){	i2c_write(idt_tab[index]);}
	
	index = 0;
	while(phrase[index]!=0){
		i2c_write(phrase[index]);			//envoier un à un les caractères du char
		index++;
	}
	
	i2c_write(0x00);
}
/*==========================================================================
====Fonction     : writeLine		                                    ====
====Sierro Romain: 13.05.2016                                           ====
====--------------------------------------------------------------------====
====Affiche une ligne sur l'écran			 					 		====
============================================================================*/
void writeLine(int end_xh,int end_xl, int end_yh, int end_yl)
{
	i2c_write(0xAA);	//Commande de l'écran
	i2c_write(0x44);	//Commande pour dessiner
	i2c_write(0x4C);	//Commande pour dessiner un trait
	i2c_write(end_xh);	//Comme l'écran est grand, sa taille se donne sur 
	i2c_write(end_xl);	//2 Byte. C'est pour ça que les coordonnées sont en 2 fois
	i2c_write(end_yh);	//La ligne part de la position du curseur jusqu'à la position 
	i2c_write(end_yl);	//entrée dans la fonction
}


void writeTab(void)
{

	startConnection(100);				//Envoie le nombre de caractère qu'il va recevoir
	reinit();						//Efface l'écran

	iCK(0x00, 0x00, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x03, 0x1F, 0x00, 0xA0);	//Fait une ligne horizontal en travers de l'écran
	iCK(0x01, 0x0A, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la largeur
	writeLine(0x01, 0x0A, 0x01, 0xDF);	//Fait une ligne verticale à 1/3 de l'écran
	iCK(0x02, 0x16, 0x00, 0xA0);	//Positionne le curseur à 2/3 de la largeur
	writeLine(0x02, 0x16, 0x01, 0xDF);	//Fait une ligne verticale à 2/3 de l'écran

	i2c_stop();

}


#INT_TIMER0 FAST			//Quick save -> minimal contexte saving

TIMER0_isr()
{
	TMR0L=0x69;                         //Reload with:0BDC=65535-6250=3036
	TMR0H=0x67;
	int indexTimer;
if (idelay==10)
{

	idelay=0;
	GraphVoltage_tab[15]=(adc_read(AnalTension)-212)*5.93+(((adc_read(AnalCourant))/8.5)*RI);
	for (indexTimer=1;indexTimer<16;indexTimer++)
	{
		GraphVoltage_tab[indexTimer-1]=GraphVoltage_tab[indexTimer];
	}

}
else
{
idelay++;
}

if (etat==2)
	{
		startConnection(100);
		iCK(0x00, 0x28, 0x00, 0xC3);	//Positionne le curseur
		iDe(0x01,0xD6,0x01,0x04);
		i2c_stop();
		DrawGraphVoltage();



		for (indexFor=0;indexFor<15;indexFor++)
			{
				value=GraphVoltage_tab[indexFor];
				if (value>=196)
					{
						PosY=(-0.941176)*value+440;
						if (indexFor>6)
							{
								PosX=30*indexFor-204;
								startConnection(100);
								iCK(0x01, PosX, 0x00, PosY);	//Positionne le curseur à 1/3 de la largeur
								iDC(0x00,0x05);
								i2c_stop();
							}
						else
							{
								PosX=30*indexFor+51;
								startConnection(100);
								iCK(0x00, PosX, 0x00, PosY);	//Positionne le curseur à 1/3 de la largeur
								iDC(0x00,0x05);
								i2c_stop();
							}
					}
				else
					{
						PosY=(value)*(-0.93814432)+440;
						if (indexFor>6)
							{
								PosX=30*indexFor-204;
								startConnection(100);
								iCK(0x01, PosX, 0x01, PosY);	//Positionne le curseur à 1/3 de la largeur
								iDC(0x00,0x05);
								i2c_stop();
							}
						else
							{
								PosX=30*indexFor+51;
								startConnection(100);
								iCK(0x00, PosX, 0x01, PosY);	//Positionne le curseur à 1/3 de la largeur
								iDC(0x00,0x05);
								i2c_stop();
							}
					}		
	
			}
//AFFICHE LES INFOS A COTE
		tension=adc_read(AnalTension)+((adc_read(AnalCourant)/8.5)*RI);	//Lit la mesure de AN1
		Cent=tension/44.2;		//Convertit pour garder la centaine
		Dix=tension/4.42;		//Convertit pour garder la dizaine
		Dix=Dix%10;
		Unit=tension/0.442;		//Convertit pour garder l'unité
		Unit=Unit%10;



		startConnection(150);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x02, 0xAF, 0x01, 0x6F);	//Positionne le curseur
		iDe(0x00,0xCA,0x00,0xAC);

		i2c_write(0xAA);		//Commande de l'écran
		i2c_write(0x44);		//Commande pour dessiner 
		i2c_write(0x54);		//Commande pour écrire du texte
		i2c_write(Cent+0x30);	//Affiche les centaines
		i2c_write(Dix+0x30);	//Affiche les dizaine
		i2c_write(0x2E);		//.
		i2c_write(Unit+0x30);	//Affiche les unités
		i2c_write(0x5B);		//[
		i2c_write(0x56);		//V
		i2c_write(0x5D);		//]

		i2c_stop();





		courant=adc_read(AnalCourant);	//Lit la mesure de AN1
		Cent=courant/85;		
		Dix=courant/8.5;		
		Dix=Dix%10;				//-Je fais un modulo par la suite
		Unit=courant;			// 	car on ne peut pas le faire sur un double
		Unit=Unit%10;			//
	


		startConnection(150);			//Dit combien de caractère on envoi
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x02, 0xAF, 0x01, 0x1F);	//Positionne le curseur
		iDe(0x00,0xA0,0x00,0x50);
	
		i2c_write(0xAA);		//Commande de l'écran
		i2c_write(0x44);		//Commande pour dessiner 
		i2c_write(0x54);		//Commande pour écrire du texte
		i2c_write(Cent+0x30);	//Affiche les centaines
		i2c_write(Dix+0x30);	//Affiche les dizaine
		i2c_write(0x2E);		//.
		i2c_write(Unit+0x30);	//Affiche les unités
		i2c_write(0x5B);		//[
		i2c_write(0x41);		//A
		i2c_write(0x5D);		//]
		i2c_stop();



		uri=courant*RI;	//Lit la mesure de AN1
		Cent=uri/100;		
		Dix=uri/10;		
		Dix=Dix%10;				//-Je fais un modulo par la suite
		Unit=uri;			// 	car on ne peut pas le faire sur un double
		Unit=Unit%10;			//
	


		startConnection(150);			//Dit combien de caractère on envoi
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x02, 0xAF, 0x00, 0xCF);	//Positionne le curseur
		iDe(0x00,0xA0,0x00,0x50);
	
		i2c_write(0xAA);		//Commande de l'écran
		i2c_write(0x44);		//Commande pour dessiner 
		i2c_write(0x54);		//Commande pour écrire du texte
		i2c_write(Cent+0x30);	//Affiche les centaines
		i2c_write(Dix+0x30);	//Affiche les dizaine
		i2c_write(0x2E);		//.
		i2c_write(Unit+0x30);	//Affiche les unités
		i2c_write(0x5B);		//[
		i2c_write(0x56);		//V
		i2c_write(0x5D);		//]
		i2c_stop();

	
		startConnection(150);
		iCK(0x00,0x00,0x00,0x00);
		iTW(0x03,0x1F);
		iTH(0x00,0xA0);
		iTA(0x46);
		i2c_stop();
}

	if (etat==1)
	{

		tension=((adc_read(AnalTension)-212)*5.93)+((adc_read(AnalCourant))/8.5)*RI;	//Lit la mesure de AN1
		Cent=tension/25.5;		//Convertit pour garder la centaine
		Dix=tension/2.55;		//Convertit pour garder la dizaine
		Dix=Dix%10;
		Unit=tension/0.255;		//Convertit pour garder l'unité
		Unit=Unit%10;

		startConnection(100);
		iCK(0x00,0x00,0x00,0xE0);
		if(tension>=228)	//Si la tension est plus grande que 45V
		{
			iG(0x06);	//Dessine la batterie verte
		}
		else if(tension>=152)	//Si la tension est plus grande que 30V
		{
			iG(0x07);	//Dessine la batterie jaune
		}
		else if(tension>=102)	//Si la tension est plus grande que 20V
		{
			iG(0x08);	//Dessine la batterie orange
		}
		else if(tension>=51)	//Si la tension est plus grande que 10V
		{
			iG(0x09);	//Dessine la batterie rouge
		}
		else if(tension<51)	//Si la tension est plus petite que 10V
		{
			iG(0x0A);	//Dessine la batterie vide
		}

		iAF(0x00, 0x01);			//Sélectionne la police d'écriture
		iCK(0x00, 0x0F, 0x01, 0x7F);	//Positionne le curseur
		iDe(0x00,0xCA,0x00,0xAC);

		i2c_write(0xAA);		//Commande de l'écran
		i2c_write(0x44);		//Commande pour dessiner 
		i2c_write(0x54);		//Commande pour écrire du texte
		i2c_write(Cent+0x30);	//Affiche les centaines
		i2c_write(Dix+0x30);	//Affiche les dizaine
		i2c_write(0x2E);		//.
		i2c_write(Unit+0x30);	//Affiche les unités
		i2c_write(0x25);		//%

		i2c_stop();


		startConnection(100);
		iCK(0x01, 0x55, 0x00, 0xDF);	//Positionne le curseur
		iG(0x01);
		i2c_stop();






		Cent=distance/100;	
		Dix=(distance/10)%10;
		Unit=distance%10;
			
		startConnection(100);	//Dit à l'écran le nombre de caractère qu'on lui envoie
		iAF(0x00, 0x01);	//Sélectionne la police d'écriture	
		iCK(0x02, 0x1E, 0x01, 0x7F);	//Positionne le curseur
		
		i2c_write(0xAA);		//Commande de l'écran
		i2c_write(0x44);		//Commande pour dessiner 
		i2c_write(0x54);		//Commande pour écrire du texte
		i2c_write(Cent+0x30);	//Affiche les mètres
		i2c_write(Dix+0x30);	//Affiche les centimètres
		i2c_write(Unit+0x30);	//Affiche les milimètre 
		i2c_write(0x20);		//Espace
		i2c_write(0x63);		//c
		i2c_write(0x6D);		//m
		i2c_stop();






		startConnection(100);
		iCK(0x02, 0x6A, 0x00, 0xCF);
		iG(0x02);
		i2c_stop();
		TouchFieldMain();

		sendPulse(); //Doit etre ici sinon timer0 pas fini et lance le timer 1 apres
	}

}
//Envoie l'image du début du démarrage du programme
void VoltageScreen(void)
{
	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	reinit();						//Efface l'écran
	iCK(0x00, 0x00, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x03, 0x1F, 0x00, 0xA0);	//Fait une ligne horizontal en travers de l'écran
	iCK(0x00,0x00,0x00,0x00);
	iTW(0x03,0x1F);
	iTH(0x00,0xA0);
	iTA(0x46);
	i2c_stop();
	writeText();
}
void writeText(void)
{
	startConnection(150);
	iCK(0x01,0x10,0x00,0x25);
	iAF(0x00,0x01);
	strcpy(Tab_Phrase, "eKarting");
	iDT(Tab_Phrase);
	i2c_stop();
}
void TouchFieldMain(void)
{
	startConnection(150);
	iCK(0x00,0x00,0x00,0xA0);
	iTW(0x01,0x0A);
	iTH(0x01,0x3F);
	iTA(0x44);
	i2c_stop();
	startConnection(150);
	iCK(0x01, 0x0A, 0x00, 0xA0);
	iTW(0x01,0x0C);
	iTH(0x01,0x3F);
	iTA(0x33);
	i2c_stop();
}
void DrawGraphVoltage(void)
{
	startConnection(150);
	iCK(0x00, 0x32, 0x00, 0xC8);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x32, 0x01, 0xC2);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x2D, 0x01, 0x04);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x37, 0x01, 0x04);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x2D, 0x01, 0x40);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x37, 0x01, 0x40);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x2D, 0x01, 0x7C);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x37, 0x01, 0x7C);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x2D, 0x00, 0xC8);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x37, 0x00, 0xC8);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x28, 0x01, 0xB8);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0xF4, 0x01, 0xB8);	//Fait une ligne horizontal en travers de l'écran


	i2c_stop();


	startConnection(250);

	iCK(0x00, 0x50, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x50, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x6E, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x6E, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0x8C, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0x8C, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0xAB, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0xAB, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0xC8, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0xC8, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x00, 0xE6, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x00, 0xE6, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x04, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x04, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x22, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x22, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x40, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x40, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x5E, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x5E, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x7C, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x7C, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0x9A, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0x9A, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0xB8, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0xB8, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0xD6, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0xD6, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	iCK(0x01, 0xF4, 0x01, 0xBD);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x01, 0xF4, 0x01, 0xB3);	//Fait une ligne horizontal en travers de l'écran

	i2c_stop();
	startConnection(350);
	iCK(0x00, 0x0A, 0x00, 0xC1);	//Positionne le curseur à 1/3 de la hauteur	
	iAF(0x00,0x03);
	strcpy(Tab_Phrase, "100%");
	iDT(Tab_Phrase);
	iCK(0x00, 0x0A, 0x00, 0xFC);	//Positionne le curseur à 1/3 de la hauteur	
	strcpy(Tab_Phrase, "75%");
	iDT(Tab_Phrase);
	iCK(0x00, 0x0A, 0x01, 0x36);	//Positionne le curseur à 1/3 de la hauteur	
	strcpy(Tab_Phrase, "50%");
	iDT(Tab_Phrase);
	iCK(0x00, 0x0A, 0x01, 0x72);	//Positionne le curseur à 1/3 de la hauteur	
	strcpy(Tab_Phrase, "25%");
	iDT(Tab_Phrase);
	iCK(0x00, 0xC8, 0x00, 0xB4);	//Positionne le curseur à 1/3 de la hauteur	
	strcpy(Tab_Phrase, "Tension des batteries");
	iDT(Tab_Phrase);
	i2c_stop();
}


//----------------------------------------------------------------------------
//============================================================================
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: lis les données de l'écran tactile, les stocks dans un tableau
//-------------------------------------------------------------------------


void BlockRead(void)
{
	startReading();
	//récupère les valeurs de l'écran tactile
	for(index=0; index<4; index++){ DataRecu[index] = i2c_read();}

	DataRecu[4] = i2c_read(0);
	i2c_stop();
}


//------------------------------------------------------------------------------
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: Le bit d'ALERT est activé, ce qui signifie
//  que l'écran veut envoyer des données
//-------------------------------------------------------------------------

#INT_EXT
void ALERT(void)
{
	BlockRead();
	
	if(DataRecu[1] == 0x4B){	// on appuie
		NewData = DataRecu[2];		// dis quel est l'écran à afficher
	}
/*	if(DataRecu[1] == 0x6B){	// on relâche

	}*/
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: dis que le uP veut commencer à lire des données
//-------------------------------------------------------------------------
void startReading(void)
{
	int ack;
	i2c_start();
	
	do{ ack = i2c_write(I2CADDRESS);} while(ack == 1);
	do{ ack = i2c_write(0x04);}		  while(ack == 1);	// dis qu'on veut "Read Block"
	//-------------
	i2c_start();
	do{ ack = i2c_write(I2CADDRESS+1);}  while(ack == 1);
}
void iDe(int end_xh,int end_xl, int end_yh, int end_yl)
{
	for(index=0; index<3; index++){	i2c_write(iDe_tab[index]);}
	i2c_write(end_xh);	//Comme l'écran est grand, sa taille se donne sur 
	i2c_write(end_xl);	//2 Byte. C'est pour ça que les coordonnées sont en 2 fois
	i2c_write(end_yh);	//La ligne part de la position du curseur jusqu'à la position 
	i2c_write(end_yl);	//entrée dans la fonction
}
void iDC(int RadiusHigh,int RadiusLow)
{
	for(index=0; index<3; index++){	i2c_write(iDC_tab[index]);}
	i2c_write(RadiusHigh);
	i2c_write(RadiusLow);
}
void speedScreen(void)
{
	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	reinit();						//Efface l'écran
	iCK(0x00, 0x00, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x03, 0x1F, 0x00, 0xA0);	//Fait une ligne horizontal en travers de l'écran
	iCK(0x00,0x00,0x00,0x00);
	iTW(0x03,0x1F);
	iTH(0x00,0xA0);
	iTA(0x46);
	i2c_stop();
	writeText();
}
//timer 58u
#INT_TIMER1
Timer1_isr(){

//	disable_interrupts(INT_TIMER1);	//Active le timer0
	TMR1H=0xFF;	//Préchargement du timer
	TMR1L=0x06;	

	if(Echo==1)	//Si on un toujours de l'echo
	{
		indexEcho+=1;		//On incrémente l'index de l'echo
		RD7=!RD7;
//		enable_interrupts(INT_TIMER1);	//Active le timer0

		if(indexEcho>220)	//Si l'echo à atteint la distance max
		{
			distance=220;	//On fixe la distance à 4m
			indexEcho=0;	//On remet à 0 l'indexEcho
			disable_interrupts(INT_TIMER1);	//Désactive timer0			
		}


	}
	else if (Echo==0)
	{

		distance=indexEcho;
		indexEcho=0;
		disable_interrupts(INT_TIMER1);

RD6=!RD6;


}
}



#INT_EXT2
void Echo_isr()
{
	enable_interrupts(INT_TIMER1);	//Active le timer0
	disable_interrupts(INT_EXT2);	//Désactive l'interruption RB2
}
/*==========================================================================
====Fonction     : sendPulse		                                    ====
====Sierro Romain: 11.05.2016                                           ====
====--------------------------------------------------------------------====
====Envoie une impulsion sur le trigger du capteur				 		====
============================================================================*/
void sendPulse(void)
{
	enable_interrupts(INT_EXT2);	//Autorise l'interruption RB2
	Trigger=1;		//On allume l'émetteur du capteur
	delay_us(10);	//pendant 10µs
	Trigger=0;		//Puis on l'éteint
}
