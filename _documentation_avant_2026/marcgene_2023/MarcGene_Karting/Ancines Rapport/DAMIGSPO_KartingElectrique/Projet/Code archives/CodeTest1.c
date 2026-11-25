/* =========================================================================
====                                                                    ====
====                     EPTM - Ecole des métiers                       ====
====                                                                    ====
============================================================================
====                                                                    ====
====                     Programmation en langage C                     ====
====                     --------------------------                     ====
====                                                                    ====
====--------------------------------------------------------------------====
==== Nom du programme                  : Code Direction du karting      ====
==== Version                           : V2.0                           ====
==== Nom du(des) fichier(s) source     : TestPontH.C                    ====
====------------------------------------------------------------------  ====
==== Créateur (Nom, Prénom, Localité)  : Gsponer Damien Sion            ====
==== Classe                            : EL 3A	            			====
==== Date de création                  : 29.03.2019                     ====
==== Date de la dernière modification  : 29.03.2019                     ====
====------------------------------------------------------------------	====
==== Description succincte du programme: Code servant utiliser le uP    ====
==== pour commander la direction du karting              				====
==== ----------------------------------                                 ====
==========================================================================*/

#include <18F45K22.h>
#include "18FK_register.h"

#device ICD=TRUE
#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

// Attribution des pins
#BIT Contact_sup = PORTB.0      
#BIT ContactVol1 = PORTB.1                      // Fin de course Volant
#BIT LED         = PORTC.0      
#BIT Bridge_BHI  = PORTC.2                      // Full-Bridge driver B-side-high Input
#BIT Bridge_AHI  = PORTD.4                      // Full-Bridge driver A-side-high Input
#BIT Bridge_ALI  = PORTD.5                      // Full-Bridge driver A-side-low  Input
#BIT Bridge_BLI  = PORTD.6                      // Full-Bridge driver B-side-low  Input
#BIT Bridge_Dis  = PORTD.7                      // Full-Bridge Disable PIN
#BIT RB0 = PORTB.0

// Fonction 
void Direction_G(void);                         // Commande de moteur vers la gauche
void Direction_D(void);                         // Commande de moteur vers la droite
void Stop_Moteur(void);                         // Commande d'arret du moteur

//Variables
int A = 1;
int B = 1;

//Init
void Init(void)
{
    PORTA= 0xFF;                        // Entrée RA3/4/5
	TRISA= 0xFF;
    ANSELA= 0;                          // PORTA

    PORTB= 0xFF;                        //Entrée RB0/1/2
    TRISB= 0xFF;    
    ANSELB= 0;                          // PORTB
    
    PORTC= 0x00;                        // Sortie RC0/2
	TRISC= 0x00;	
    ANSELC= 0;                          // PORTC
    
    PORTD= 0x08;                        // Entrée RD3 + Sortie RD4/5/6/7
	TRISD= 0x08;	
    ANSELD= 0x0;                        // PORTD
    
    PORTE= 0xFF;                        // Connecteur sup
	TRISE= 0xFF; 	
    ANSELE= 0;                          // PORTE
    Bridge_Dis = 1;
}

//Main 
void main(void)
{
    Init();                         // Appel d'initialisation des PORTs
    
    while(1)
    {
        if(RB0 == 1)
        {
            Direction_G();
            
        }
        else
        {
            Direction_D();
        }
    }
    
}

void Direction_G(void)                      // Fais tourner le moteur vers la gauhe
{
    Stop_Moteur();                          // Eteindre les sorties pour une sécuritée
    LED = 1;
    Bridge_Dis = 0;
    Bridge_AHI = 1;                         // Activation des sorties pour le faire tourner a droite
    Bridge_BLI = 1;                         //
}

void Direction_D(void)                      // Fait tourner le moteur vers la gauche
{
    Stop_moteur();                          // Eteindre les sorties pour une sécuritée
    LED = 0;
    Bridge_Dis = 0;
    Bridge_ALI = 1;                         // Activation des sorties pour le faire tourner a gauche
    Bridge_BHI = 1;                         //
}

void Stop_Moteur(void)                      // Arrete d'alimenter le moteur
{      
    Bridge_AHI = 0;                         // Met toutes les commandes du moteur à 0 
    Bridge_ALI = 0;                         // = sert de protection pour els commandes futurs
    Bridge_BHI = 0;                         //
    Bridge_BLI = 0;                         //
    
	Bridge_Dis = 1;                         // Arret du moteur
    
	delay_ms(5);
}
