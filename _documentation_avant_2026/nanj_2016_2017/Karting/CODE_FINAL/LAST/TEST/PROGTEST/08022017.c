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
#BIT RD7=PORTD.7        //Bit de test
#BIT RD6=PORTD.6		//Bit de test
//#BIT Commande2=PORTD.4
#DEFINE RI 1 //Désigne la résistance interne de la batterie
#DEFINE AnalCourant 0 //Désigne sur quel entrée analogique l'entrée du courant est, la en l'occurence sur RA0
#DEFINE AnalTension 1 //Désigne sur quel entrée analogique l'entrée de la tension est, la en l'occurence sur RA1

#include "FonctionsEcran.c"


int indexTIME;
	int indexTimer;

int etat=0; //Permet de choisir quel image a afficher dans le timer, elle se change dans le main


void BlockRead(void);	//lis des données de l'écran
void ALERT(void);	//interruptions de l'écran quand il doit envoyer des données

int OldData;
int NewData;

void VoltageScreen(void); //Affiche la page des tensions
void TouchFieldMain(void);//Tactile main
void writeText(void); //Ecris eKarting
void startReading(void); //Demarre la lecture
void writeTabMain(void); //Ecris les tableaus du main (lignes)
void sendPulse(void);
void WriteVoltageText(void);
void TouchFieldVoltage(void);
void TouchFieldSpeed(void);
void init(void); //Initialise tous les ports
void speedScreen(void);
void DrawGraphVoltage(void);
void SaveVoltageData(void);
void DrawGraphVoltageDot(void);
void MesAndWriteVoltage(void);
void MesAndWriteCurrent(void);
void MesAndWriteUri(void);
void ClearGraphVoltage(void);
int adc_read(int channel);
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

/*==========================================================================
====Fonction     : Config_uP	                                        ====
====Jean Nanchen: 11.12.2016                                           ====
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
							writeTabMain();	//Fais un tableau qui sépare l'écran en 4 partie
											//////////////////////////////////////////////////
											//												//
											//												//
											//----------------------------------------------//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											///////////////////////////////////////////////////
											
							writeText(); //Ecris en haut au millieu Ekarting
											//////////////////////////////////////////////////
											//					eKarting					//
											//												//
											//----------------------------------------------//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											///////////////////////////////////////////////////										
							
							TouchFieldMain(); //Définis les zones tactiles de l'écran main
							etat=1; //Passe l'état à 1 (pour afficher les infos dans le timer)
					break;
				case 0x44:
							etat=2; //Passe l'état à 2 (pour afficher les infos dans le timer)
							VoltageScreen(); //Dessine une ligne horizontal au 2/3 et affiche eKarting en grand
											//////////////////////////////////////////////////
											//					eKarting					//
											//												//
											//----------------------------------------------//
											//												//
											//												//
											//												//
											//												//
											///////////////////////////////////////////////////
							DrawGraphVoltage();//Déssine les axes du graphique avec les légendes
							WriteVoltageText(); //Ecris les texte qui ne changent pas (Uri: , Ibatteries:, Ub vide:)
											//////////////////////////////////////////////////
											//					eKarting					//
											//												//
											//----------------------------------------------//
											//									Uri:		//
											//									Ibatteries:	//
											//									Ub vide:	//
											//												//
											///////////////////////////////////////////////////
							TouchFieldVoltage(); //Définis les zones tactiles de l'écran pour retourner au main

					break;

					case 0x33:

					speedScreen(); //Trace une ligne au 2/3 de l'ecran et ecrit en gros eKarting
											//////////////////////////////////////////////////
											//					eKarting					//
											//												//
											//----------------------------------------------//
											//												//
											//												//
											//												//
											//												//
											///////////////////////////////////////////////////
					TouchFieldSpeed(); //Définit les zone tactile pour l'écran de vitesse, pour le retour au MAIN
					etat=3; //Passe l'état à 3 (pour afficher les infos dans le timer)
					break;
				default: etat=0;
			}
			
			OldData = NewData;
			
		}
	}
}


void writeTabMain(void)
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
	iCK(0x00,0x00,0x00,0xA0);//Positionne le curseur à l'endroit X du dessin ci-dessous
											//////////////////////////////////////////////////
											//												//
											//												//
											//X ------------Y-------------------------------//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											//Z				|				|				//
											///////////////////////////////////////////////////
	iTW(0x01,0x0A); //Définit la largeur de notre zone tactile (valeur X - Y, coordonnée x)
	iTH(0x01,0x3F); //Définit la hauteur de note zone tactile (valeur X - Z, coordonnée y)
	iTA(0x44);//Définit le nom de la zone tactile, quand on appuie ALERT nous dira la valeur 0x44 = VOLTAGE
	i2c_stop();
	startConnection(150);
	iCK(0x01, 0x0A, 0x00, 0xA0);//Positionne le curseur à l'endroit X du dessin ci-dessous
											//////////////////////////////////////////////////
											//												//
											//												//
											//--------------X---------------Y---------------//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											//				Z				|				//
											///////////////////////////////////////////////////
	iTW(0x01,0x0C);//Définit la largeur de notre zone tactile (valeur X - Y, coordonnée x)
	iTH(0x01,0x3F);//Définit la hauteur de note zone tactile (valeur X - Z, coordonnée y)
	iTA(0x33);//Définit le nom de la zone tactile, quand on appuie ALERT nous dira la valeur 0x33 = SPEED
	i2c_stop();
}


void VoltageScreen(void)
{
	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	reinit();						//Efface l'écran
	iCK(0x00, 0x00, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x03, 0x1F, 0x00, 0xA0);	//Fait une ligne horizontal en travers de l'écran
	i2c_stop();
	writeText(); //Ecrit eKarting en haut au millieu
}

void DrawGraphVoltage(void)
{
	startConnection(150);
	//Déssine l'axe Y
	iCK(0x00, 0x32, 0x00, 0xC8); //Positionne le curseur au sommet de l'axe Y (des %) [X=(50;200)]
	writeLine(0x00, 0x32, 0x01, 0xC2); //Trace une ligne du cusreur jusqu'a [Y=(50;450)]
	//Déssine la graduation de 75%
	iCK(0x00, 0x2D, 0x01, 0x04); //Positionne le curseur à X=(45;260)
	writeLine(0x00, 0x37, 0x01, 0x04);	//Fait une ligne jusqu'a Y=(55;260)
	//Déssine la graduation des 50%
	iCK(0x00, 0x2D, 0x01, 0x40); //Positionne le curseur à X=(45;320)
	writeLine(0x00, 0x37, 0x01, 0x40); //Fait une ligne jusqu'a Y=(55;320)
	//Déssine la graduation des 25%
	iCK(0x00, 0x2D, 0x01, 0x7C); //Positionne le curseur à X=(45;380)
	writeLine(0x00, 0x37, 0x01, 0x7C);	//Fait une ligne jusqu'a Y=(55;380)
	//Déssine la graduation des 100%
	iCK(0x00, 0x2D, 0x00, 0xC8);	//Positionne le curseur à X=(45;200)
	writeLine(0x00, 0x37, 0x00, 0xC8);	//Fait une ligne jusqu'a Y=(55;200)
	
	i2c_stop();


	startConnection(280);

	//Déssine l'axe X
	iCK(0x00, 0x28, 0x01, 0xB8);	//Positionne le curseur au sommet de l'axe X (du temps) [X=(40;440)]
	writeLine(0x01, 0xF4, 0x01, 0xB8);	//Trace une ligne du cusreur jusqu'a [Y=(500;440)]
	//Déssine la graduation T=1
	iCK(0x00, 0x50, 0x01, 0xBD); //Positionne le curseur à X=(80;445)
	writeLine(0x00, 0x50, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(80;455)
	//Déssine la graduation T=2
	iCK(0x00, 0x6E, 0x01, 0xBD); //Positionne le curseur à X=(110;445)
	writeLine(0x00, 0x6E, 0x01, 0xB3);	//Fait une ligne jusqu'a Y=(110;455)
	//Déssine la graduation T=3
	iCK(0x00, 0x8C, 0x01, 0xBD); //Positionne le curseur à X=(140;445)
	writeLine(0x00, 0x8C, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(140;455)
	//Déssine la graduation T=4
	iCK(0x00, 0xAB, 0x01, 0xBD); //Positionne le curseur à X=(170;445)
	writeLine(0x00, 0xAB, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(170;455)
	//Déssine la graduation T=5
	iCK(0x00, 0xC8, 0x01, 0xBD); //Positionne le curseur à X=(200;445)
	writeLine(0x00, 0xC8, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(200;455)
	//Déssine la graduation T=6
	iCK(0x00, 0xE6, 0x01, 0xBD); //Positionne le curseur à X=(230;445)
	writeLine(0x00, 0xE6, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(230;455)
	//Déssine la graduation T=7
	iCK(0x01, 0x04, 0x01, 0xBD); //Positionne le curseur à X=(260;445)
	writeLine(0x01, 0x04, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(260;455)
	//Déssine la graduation T=8
	iCK(0x01, 0x22, 0x01, 0xBD); //Positionne le curseur à X=(290;445)
	writeLine(0x01, 0x22, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(290;455)
	//Déssine la graduation T=9
	iCK(0x01, 0x40, 0x01, 0xBD); //Positionne le curseur à X=(320;445)
	writeLine(0x01, 0x40, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(320;455)
	//Déssine la graduation T=10
	iCK(0x01, 0x5E, 0x01, 0xBD); //Positionne le curseur à X=(350;445)
	writeLine(0x01, 0x5E, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(380;455)
	//Déssine la graduation T=11
	iCK(0x01, 0x7C, 0x01, 0xBD); //Positionne le curseur à X=(380;445)
	writeLine(0x01, 0x7C, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(80;455)
	//Déssine la graduation T=12
	iCK(0x01, 0x9A, 0x01, 0xBD); //Positionne le curseur à X=(410;445)
	writeLine(0x01, 0x9A, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(410;455)
	//Déssine la graduation T=13
	iCK(0x01, 0xB8, 0x01, 0xBD); //Positionne le curseur à X=(440;445)
	writeLine(0x01, 0xB8, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(440;455)
	//Déssine la graduation T=14
	iCK(0x01, 0xD6, 0x01, 0xBD); //Positionne le curseur à X=(470;445)
	writeLine(0x01, 0xD6, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(470;455)
	//Déssine la graduation T=15
	iCK(0x01, 0xF4, 0x01, 0xBD); //Positionne le curseur à X=(500;445)
	writeLine(0x01, 0xF4, 0x01, 0xB3); //Fait une ligne jusqu'a Y=(500;455)

	i2c_stop();
	
	
	
	  //------------------------------------------------------//
	 // Ecrit les légendes des axes et le nom du graphique   //
	//------------------------------------------------------//
	startConnection(350);
	iCK(0x00, 0x0A, 0x00, 0xC1);	//Positionne le curseur au niveau de la graduation de 100%	
	iAF(0x00,0x03); //Sélectionne le font 3
	strcpy(Tab_Phrase, "100%"); //Ecrit 100% dans tabphrase
	iDT(Tab_Phrase); //Ecrit ce qu il y a dans tabphrase
	iCK(0x00, 0x0A, 0x00, 0xFC);	//Positionne le curseur au niveau de la graduation de 100%	
	strcpy(Tab_Phrase, "75%");//Ecrit 75% dans tabphrase
	iDT(Tab_Phrase);//Ecrit ce qu il y a dans tabphrase
	iCK(0x00, 0x0A, 0x01, 0x36);	//Positionne le curseur au niveau de la graduation de 100%	
	strcpy(Tab_Phrase, "50%");//Ecrit 50% dans tabphrase
	iDT(Tab_Phrase);//Ecrit ce qu il y a dans tabphrase
	iCK(0x00, 0x0A, 0x01, 0x72);	//Positionne le curseur au niveau de la graduation de 100%	
	strcpy(Tab_Phrase, "25%");//Ecrit 25% dans tabphrase
	iDT(Tab_Phrase);//Ecrit ce qu il y a dans tabphrase
	iCK(0x00, 0xC8, 0x00, 0xB4);	//Positionne le curseur au dessus du graphique
	strcpy(Tab_Phrase, "Tension des batteries");//Ecrit Tension des batteries dans tabphrase
	iDT(Tab_Phrase);//Ecrit ce qu il y a dans tabphrase
	i2c_stop();
}
void WriteVoltageText(void)
{
		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x00, 0xCF);	//Positionne le curseur à droite au milieu
		strcpy(Tab_Phrase, "URi:"); //Ecrit "Uri:" dans le tableau Tab_Phrase
		iDT(Tab_Phrase); //Ecrit "Uri :" à droite de l'écran
		i2c_stop();	

		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x01, 0x1F);	//Positionne le curseur
		strcpy(Tab_Phrase, "Ibatteries:"); //Ecrit "Ibatteries:" dans le tableau Tab_Phrase
		iDT(Tab_Phrase); //Ecrit "Ibatteries :" à droite de l'écran
		i2c_stop();	

		startConnection(350);
		iAF(0x00, 0x02);			//Sélectionne la police d'écriture
		iCK(0x01, 0xFF, 0x01, 0x6F);	//Positionne le curseur
		strcpy(Tab_Phrase, "Ub vide:"); //Ecrit "Ub vide:" dans le tableau Tab_Phrase
		iDT(Tab_Phrase); //Ecrit "Ub vide" à droite de l'écran	
		i2c_stop();	
}

void TouchFieldVoltage(void)
{
//--------------------------------------------//
//Définit la zone tactile pour revenir au main//
//--------------------------------------------//
	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	iCK(0x00,0x00,0x00,0x00);//Positionne le curseur à l'endroit X du dessin ci-dessous
											//////////////////////////////////////////////////
											//X											   Y//
											//												//
											//---------------------------------------------Z//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											//				|				|				//
											///////////////////////////////////////////////////
	iTW(0x03,0x1F);//Définit la largeur de notre zone tactile (valeur X - Y, coordonnée x)
	iTH(0x00,0xA0);//Définit la hauteur de note zone tactile (valeur X - Z, coordonnée y)
	iTA(0x46);//Définit le nom de la zone tactile, quand on appuie ALERT nous dira la valeur 0x46 = MAIN
	i2c_stop();
}
void speedScreen(void)
{

	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	reinit();						//Efface l'écran
	iCK(0x00, 0x00, 0x00, 0xA0);	//Positionne le curseur à 1/3 de la hauteur
	writeLine(0x03, 0x1F, 0x00, 0xA0);	//Fait une ligne horizontal en travers de l'écran
	i2c_stop();
	writeText();
}
void TouchFieldSpeed(void)
{
	startConnection(150);	//Dit à l'écran combien de caractère on lui envoie 
	iCK(0x00,0x00,0x00,0x00);
	iTW(0x03,0x1F);
	iTH(0x00,0xA0);
	iTA(0x46);
}
void SaveVoltageData(void)
{
if (idelay==10) //Rentre dans le if chaque 10secondes
{

	idelay=0; //reset
	
	GraphVoltage_tab[15]=(adc_read(AnalTension)-212)*5.93+(((adc_read(AnalCourant))/8.5)*RI);	//La première partie [(adc_read(AnalTension)-212)*5.93] prend en compte que la plage varie de 48V à 57.6V
																								//Donc si la batterie est à 48V, la valeur numérique sera de 0, et si la batterie est pleine
																								//la valeur numérique sera de 255, c'est juste une histoire d'amplification de courbe
																								//
																								//La deuxième partie sert d'additionner la tension aux borne de la résistance interne quand on tire du courant
																								//Car sinon on verrais la tension du karting chuter quand on tire du courant, la on contre cela, la formule c'est U=R*I

//Ici, on prend la valeur à l'emplacement par exemple 15 et on la décale à l'emplacement 14
//On fait cela pour toutes les valeurs ce qi permet de pouvoir faire défiler chaque 10 secondes
//La courbe.
	for (indexTimer=1;indexTimer<16;indexTimer++)
	{
		GraphVoltage_tab[indexTimer-1]=GraphVoltage_tab[indexTimer];
	}

}
else
{
idelay++; //On incrémente
}

}
void DrawGraphVoltageDot(void)
{
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
}
void MesAndWriteVoltage(void)
{
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
}
void MesAndWriteCurrent(void)
{
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
}
void MesAndWriteUri(void)
{
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
}
void ClearGraphVoltage(void)
{
		startConnection(100);
		iCK(0x00, 0x28, 0x00, 0xC3);	//Positionne le curseur
		iDe(0x01,0xD6,0x01,0x04);
		i2c_stop();
}
#INT_TIMER0 FAST			//Quick save -> minimal contexte saving

TIMER0_isr()
{
	TMR0L=0x69;                         //Reload with:6769
	TMR0H=0x67;

	
//------------------------------//
//MEMORISATION VALEUR GRAPHIQUE//
//-----------------------------//
SaveVoltageData(); //Chaque 10 secondes, on mesure la valeur de la tension, et on stock dans un tableau



if (etat==2) //Si on passe du main à voltage
	{
	//Clear la zone du graphique

		
		ClearGraphVoltage();
		DrawGraphVoltage();//Dessine l'axe et les légende


//---------------//
// DRAW GRAPHICS //
//---------------//
		DrawGraphVoltageDot(); //Désine les ronds de sorte à faire un graph

//AFFICHE LES INFOS A COTE
		MesAndWriteVoltage();
		MesAndWriteCurrent();
		MesAndWriteUri();
		TouchFieldVoltage();
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
			
		startConnection(150);	//Dit à l'écran le nombre de caractère qu'on lui envoie
		iAF(0x00, 0x01);	//Sélectionne la police d'écriture	
		iCK(0x02, 0x3A, 0x01, 0x7F);	//Positionne le curseur
		
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
			distance=220;	//On fixe la distance à 220cm
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

