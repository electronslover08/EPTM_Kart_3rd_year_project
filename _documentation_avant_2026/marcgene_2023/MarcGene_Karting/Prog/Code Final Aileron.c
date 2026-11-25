/* =========================================================================
====                                                                    ====
====               EMVs - Ecole des métiers du Valais                   ====
====                                                                    ====
============================================================================
====                                                                    ====
====                     Programmation en langage C                     ====
====                     --------------------------                     ====
====                                                                    ====
====--------------------------------------------------------------------====
==== Nom du programme                  : Realisation 43                 ====
==== Version                           : V1.0                           ====
==== Nom du(des) fichier(s) source     : Programation.c                 ====
====--------------------------------------------------------------------====
==== Créateur (Nom, Prénom, Localité)  : Deferr Gabriel/ Marco Genet    ====
==== Classe                            : El3                            ====
==== Date de creation                  : 29.03.23	                    ====
==== Date de la dernière modification  :                                ====
====--------------------------------------------------------------------====
==== Description succincte du programme:                                ====
==== ----------------------------------                                 ====
==== Ce code permet de gerer l'affichage de la vitesse venant d'un 		====
==== capteur reed. Il permet aussi de jouer avec l'affichage de la 		====
==== tensions des batteries. L'i2c sera utilisé pour parler avec 		====
==== l'écran.                                                           ====
==== Permet a l'écrn de s'ouvrir par apport a la valeur du              ====
==== potentiomètre                                                      ====
========================================================================= */
#include <18F4520.h>
#include "18F_register.h"

//#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST
#BIT RC0 = PORTC.0
#BIT RB0 = PORTB.0
#BIT RB3 = PORTB.3
#BIT RB5 = PORTB.5
#BIT RD0 = PORTD.0
#BIT ADGO = ADCON0.1
#BIT ADFM = ADCON2.7
//Déclaration des variables globales
double temps = 0;					// variable pour la base de temps
int vitesse = 0;					// variable contenant la vitesse du kart
int valHexa = 0;					// variable avec les chiffres 0 à 9 en ascii
	
int16 volt = 0;						// variable contenant la tension 0 à 5v
int16 Affichage = 0;				// variable contenant la vrai valeur de tension
int16 volt_FIN = 0;					// unité des volts
int16 volt_FIN2 = 0;				// dizaine des volts

//Prototipes
void Init_uC(void);			//fonction d'initialisation 
void compteur(int comp);	//fonction qui affiche la vitesse
void voltmetre(void);		//fonction qui affiche les volts
void affiche0(void);		//fonction qui affiche 0 km/h



//=================================================================================
//Description de la fonction:
//Cette fonction convertit la valeur analogique 
//du potentiomètre en valeur digitale 8bits
//=================================================================================
//Paramètres:
//en entrée: int channel : qui définit le channel à convertir
//			 
//en sortie: ADRESH : registre 8bits qui contient la valeur convertie du pot
//=================================================================================
int Adc_read(int channel)
{
    TMR1L = 0xB0;
    TMR1H = 0x3C;
	ADCON0 = (channel << 2) + 0b00000001;
	ADGO = 1;
	while(ADGO)
		continue;
	return(ADRESH);
}//------------------------------------------------------------------------------------

// Routine d'interruption qui s'execute chaque 1ms, 
// Incremenent la variable temps
#INT_TIMER0 			//Quick save -> minimal contexte saving
void TIMER0_isr()
{
// 1ms (TMR0 = 100'd, 64'h)
TMR0H  = 0x00;
TMR0L = 0x64;

temps = temps+1; 		//incrémente de 1 chaque 1mS

if(temps > 2000)		// si le temps de deux secondes est dépassé
{
affiche0();				// l'écran affichera 0
temps=0;
}

	
}

#INT_TIMER1 			
void TIMER1_isr()
{ 
    int mamen = 0;
    
    Adc_read(1);  //Lecture de l'entré AN1
    mamen = (ADRESH   / 12.75);  // 20 seuilles diférnet
    
        RB5 = 1;  //Sortie aileron a 1
        delay_ms(1);  //Attente base servo moteur
        
        if(mamen > 0)  
        {
        do
            {
                
                
                delay_us(50);  //Fait plusieurs ois l'action (0-20x donc de 1ms a 2ms)
                mamen --;
     
                
            }while (mamen >= 1);
        }
        
        RB5 = 0;   //Sortie aileron à 0
        
       

}

//*************************************************************************
//*  Fonction:	main()
//*************************************************************************
//*  Auteur: DEFG
//*  Date: 12.01.2019
//*************************************************************************
//*  Description: 
//*  -> main
//*=========================================================================
void main(void)
{

Init_uC();	// initialisation du micro controlleur

while(1)     // une boucle infinie
{
if(RB3==0)						//si le bouton de la palette est pas pressé
{
enable_interrupts(INT_EXT);
}
else							//si le bouton de la palette est pressé
{
disable_interrupts(INT_EXT);	// désactivation de l'interruption RB0
voltmetre();					// Affichage des volts
delay_ms(2000);					// attente deux deux secondes avant de repasser en km/h
enable_interrupts(INT_EXT);		// réactivation de l'interruption RB0
}}}

//fin de la fonction main====================================================
//===========================================================================



//*************************************************************************
//*  Fonction:	Init_Uc()
//*************************************************************************
//*  Auteur: DEFG
//*  Date: 12.01.2019
//*************************************************************************
//*  Description: 
//*  -> fonction d'initialisation du Uc
//*=========================================================================

void Init_Uc(void)
{

//initialisation des ports	
TRISB=0b11011111;
TRISD=0;
PORTD=0;




//enable_interrupts(INT_EXT);
disable_interrupts(INT_EXT);
ext_int_edge(L_to_H); 		//interruption sur flanc montant

// TMR0 enable, 8 bits, PS assigned at 32
T0CON = 0b11000100;
T1CON = 0b10011001; // timer0 en mode 16 bits et activé
    TMR0L = 0xB0;
    TMR0H = 0x3C;
ADCON1=0b00001101;
ADCON2=0b00011101;
TRISD= 0b00000000;       // pour les 8 LEDs
   TMR1L = 0xB0;
   TMR1H = 0x3C;
//Gérenace des interruptions et des timers	
enable_interrupts(INT_TIMER0);
enable_interrupts(INT_TIMER1);  // autorise l'interruption timer1
enable_interrupts(GLOBAL);
}

//fin de la fonction Init====================================================
//===========================================================================





//fin de la fonction du timer0===============================================
//===========================================================================



// Interruption qui s'active passe de l'aimant devant le capteur reed. Ce passage correspond
// a un tour de roue.

#INT_EXT 
void RB0_isr(void)
{

	disable_interrupts(INT_TIMER0); 	// isr seulement sur 1er bit
	
	vitesse = 1.5/(temps*0.001)*3.6;	// on divise le diamètre de la roue par la vitesse
	temps = 0;							// mise a 0 de la variable temps
	compteur(vitesse);					// affichage de la vitesse
	enable_interrupts(INT_TIMER0); 		//le timer commence a compter
}
//fin de l'interruption rb0==================================================
//===========================================================================


//*************************************************************************
//*  Fonction:	DecToHexa()
//*************************************************************************
//*  Auteur: DEFG
//*  Date: 12.01.2019
//*************************************************************************
//*  Description: 
//*  -> switch case contenant les valeurs de 0 à 9 afin de pouvoir afficher
//*		la vitesse plus simplement.
//*=========================================================================

void DecToHexa(int ValDec)
{
	switch(ValDec)
	{
		case 0: valHexa = 48;	// 0 en ascii
				break;
		case 1: valHexa = 49;	// 1 en ascii
				break;
		case 2: valHexa = 50;	// 2 en ascii
				break;
		case 3: valHexa = 51;	// 3 en ascii
				break;
		case 4: valHexa = 52;	// 4 en ascii
				break;
		case 5: valHexa = 53;	// 5 en ascii
				break;
		case 6: valHexa = 54;	// 6 en ascii
				break;
		case 7: valHexa = 55;	// 7 en ascii
				break;
		case 8: valHexa = 56;	// 8 en ascii
				break;
		case 9: valHexa = 57;	// 9 en ascii
				break;
	}
}
//fin de l'interruption rb0==================================================
//===========================================================================



//*************************************************************************
//*  Fonction:	compteur (int comp)
//*************************************************************************
//*  Auteur: DEFG
//*  Date: 12.01.2019
//*************************************************************************
//*  Description: 
//*  -> fonction qui permet d'afficher la vitesse du karting
//*=========================================================================

void compteur(int comp)
{
	// séparation des dizaines et des unités
	DecToHexa(comp%10);
	int comp1 = valHexa;	// unités
	DecToHexa(comp/10);
	int comp2 = valHexa;	// dizaines
	
	
	i2c_start();				//début de la communication i2c
	i2c_write(0x70);			//adresse de l'écran
	i2c_write(0x02);			//Header
	i2c_write(0x19);			//reset
	i2c_write(0x1E);			//curseur
	i2c_write(0x10);			//curseur
	i2c_write(0x10);			//déplacement du text axe x	
	i2c_write(0x27); 			//hauteur du text axe y
	i2c_write(comp2);			//affichage des dizaines
	i2c_write(comp1);			//affichage des Unités
	i2c_write(0x20);			// espace
	i2c_write(0x4B);			// k
	i2c_write(0x6D);			// m
	i2c_write(0x2F);			// /
	i2c_write(0x68);			// h
	i2c_stop();					// fin de la communication i2c
}




//*************************************************************************
//*  Fonction:	voltmetre(void)
//*************************************************************************
//*  Auteur: DEFG
//*  Date: 12.01.2019
//*************************************************************************
//*  Description: 
//*  -> fonction qui permet de determiner la tension sur les batteries.
//*=========================================================================
void voltmetre(void)
{
	volt = Adc_read(0);				//récupere la valeur du convertisseur AD

 	Affichage = volt * 61 / 230;	//fait le calcul pour avoir la tension réel

	volt_FIN = (Affichage%10)+48;	//fait le caclul pour les unitées
	volt_FIN2 = (Affichage/10)+48;	//fait le calcul pour les dizaines
	

	i2c_start();				//début de la communication i2c
	i2c_write(0x70);			//adresse de l'écran
	i2c_write(0x02);			//Header
	i2c_write(0x19);			//reset
	i2c_write(0x1E);			//curseur
	i2c_write(0x10);			//curseur
	i2c_write(0x10);			//déplacement du text axe x	
	i2c_write(0x27); 			//hauteur du text axe y
	i2c_write((int)volt_FIN2);	//affichage des dizaines
	i2c_write((int)volt_FIN);	//affichage des unités
	i2c_write(0x20);			//éspace
	i2c_write(118);				//v
	i2c_write(111);				//o
	i2c_write(108);				//l
	i2c_write(116);				//t
	i2c_write(115);				//s
	delay_ms(10);				
	i2c_stop();					//fin de la communication i2c
}

void affiche0(void)
{
	i2c_start();				//début de la communication i2c
	i2c_write(0x70);			//adresse de l'écran
	i2c_write(0x02);			//Header
	i2c_write(0x19);			//reset
	i2c_write(0x1E);			//curseur
	i2c_write(0x10);			//curseur
	i2c_write(0x10);			//déplacement du text axe x	
	i2c_write(0x27); 			//hauteur du text axe y
	i2c_write(48);				// 0
	i2c_write(48);				// 0
	i2c_write(0x20);			// espace
	i2c_write(0x4B);			// k
	i2c_write(0x6D);			// m
	i2c_write(0x2F);			// /
	i2c_write(0x68);			// h
	delay_ms(10);
	i2c_stop();					//fin de la communication i2c
	
	if(RB3==1)						// si le poussoire des palettes est actif
    {
	disable_interrupts(INT_EXT);	// désactivation de l'interruption compteur
	voltmetre();					// affiche de la tension sur l'écran
	delay_ms(2000);					// garde deux secondes la tension
	enable_interrupts(INT_EXT);		// se remet tout seul en km/h
    }
}