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
==== Date de la dernière modification  : 03.04.2019                     ====
====------------------------------------------------------------------	====
==== Description succincte du programme: Code servant à tester le bon	====
==== fonctionnement du pont-H de la platine.                            ====
==========================================================================*/

#include <18F45K22.h>
#include "18FK_register.h"

#device ICD=TRUE
#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

// Attribution des pins
#BIT Contact_sup = PORTB.0              // Contact qui lance l'init des entrées
#BIT ContactVol1 = PORTB.1              // Fin de course Volant
#BIT ContactVol2 = PORTB.2              // Fin de course Volant
#BIT ContactDir1 = PORTA.5              // Fin de course Direction
#BIT ContactDir2 = PORTA.4              // Fin de course Direction
#BIT LED         = PORTC.0              // Led d'indication
#BIT Bridge_BHI  = PORTC.2              // Full-Bridge driver B-side-high Input
#BIT Bridge_AHI  = PORTD.4              // Full-Bridge driver A-side-high Input
#BIT Bridge_ALI  = PORTD.5              // Full-Bridge driver A-side-low  Input
#BIT Bridge_BLI  = PORTD.6              // Full-Bridge driver B-side-low  Input
#BIT Bridge_Dis  = PORTD.7              // Full-Bridge Disable PIN
#BIT RB1 = PORTB.1                      // Entrée supplémentaire
#BIT Current     = PORTD.3   // = AN23  // Current after INA122
#BIT U_Direction = PORTA.3   // = AN3   // Potentiomètre Direction
#BIT U_Volant    = PORTB.3   // = AN9   // Potentiomètre Volant
#BIT ADGO =  ADCON0.1                   // Sert pour convertir en digital

// Fonction 
void Direction_G(void);                 // Commande de moteur vers la gauche
void Direction_D(void);                 // Commande de moteur vers la droite
void Stop_Moteur(void);                 // Commande d'arret du moteur
unsigned char Adc_read(unsigned char channel);  // lecture ADC

//Variables
int Vol = 0;                            // Valeur actuelle du volant
int Dir = 0;                            // Valeur actuelle de la direction

// Variables de passage qui permettent au moteur de s'arrêter une seule fois
// avant un changement de direction
int A = 1;
int B = 1;

//Initialisation des PORTs
void Init(void)
{       
    PORTA= 0xFF;                    // Entrée RA3/4/5
	TRISA= 0xFF;
    ANSELA= 0;                      // PORTA

    PORTB= 0xFF;                    //Entrée RB0/1/2
    TRISB= 0xFF;    
    ANSELB= 0;                      // PORTB
    
    PORTC= 0x00;                    // Sortie RC0/2
	TRISC= 0x00;	
    ANSELC= 0;                      // PORTC
    
    PORTD= 0x08;                    // Entrée RD3 + Sortie RD4/5/6/7
	TRISD= 0x08;	
    ANSELD= 0x0;                    // PORTD
    
    PORTE= 0xFF;                    // Connecteur sup
	TRISE= 0xFF; 	
    ANSELE= 0;                      // PORTE
    Bridge_Dis = 1;
}

unsigned char Adc_read(unsigned char channel)
{         
	ADCON0 = (channel << 2) + 1;    // attribution du channel
    delay_us(10);                   // delay
	ADGO = 1;                       // Début de la conversion
	while(ADGO) {}                  // Tant que la conversion n'est pas finie 
	return (ADRESH);                // on retourne la valeur convertie
}

//Main 
void main(void)
{
    Init();                         // Initialise les PORTs
    
    while(1)                        // Boucle infinie
    {
        Dir = Adc_read(3);          // Val de la direction donnée à la variable
        Vol = Adc_read(9);          // Val du volant donnée à la variable
        if(Dir == Vol)              // Si les 2 val sont égales
        {
            Stop_Moteur();          // Empêche de moteur de changer de direction
        }
        if(Dir > Vol)               // Si la direction est au dessus du volant
        {
            Direction_G();          // Tourne les roues à gauche
        }
        if(Dir < Vol)               // Si le volant est au dessus de la direction
        {
            Direction_D();          // Tourne les roues à droite
        }
    }
}

void Direction_G(void)              // Fais tourner le moteur vers la gauche
{
    if(A = 1)                       // Test de passage
    {
        Bridge_Dis = 1;             // Arrêt du moteur
        Bridge_ALI = 0;             // Désactivation des sorties non voulues
        Bridge_BHI = 0;             //
        A++;                        // Incrémente variable de passage
        B = 1;                      // Remet la variable de passage à 1
    }
    LED = 1;                        // Led d'indication
    Bridge_Dis = 0;                 // Désactive l'arrêt du moteur
    Bridge_AHI = 1;                 // Activation des sorties pour 
    Bridge_BLI = 1;                 // le faire tourner à droite
}

void Direction_D(void)              // Fait tourner le moteur vers la droite
{
    if(B = 1)                       // Test de passage
    {
        Bridge_Dis = 1;             // Arrêt du moteur
        Bridge_AHI = 0;             // Desactivation des sorties non voulues
        Bridge_BLI = 0;             //
        B++;                        // Incrémente variable de passage
        A = 1;                      // Remet la variable de passage à 1
    }
    LED = 0;                        // Led d'indication
    Bridge_Dis = 0;                 // Désactive l'arrêt du moteur
    Bridge_ALI = 1;                 // Activation des sorties pour 
    Bridge_BHI = 1;                 // le faire tourner à gauche
}

void Stop_Moteur(void)              // Arrête d'alimenter le moteur
{      
    Bridge_AHI = 0;                 // Met toutes les commandes du moteur à 0 
    Bridge_ALI = 0;                 // = sert de protection pour 
    Bridge_BHI = 0;                 // les commandes futures
    Bridge_BLI = 0;                         
    
	Bridge_Dis = 1;                 // Arret du moteur
    A = 1;                          // Remet les variables de passage
    B = 1;                          // à 1 pour le passage dans le test
    delay_cycles(1);                // Attends pendant une instruction
}
