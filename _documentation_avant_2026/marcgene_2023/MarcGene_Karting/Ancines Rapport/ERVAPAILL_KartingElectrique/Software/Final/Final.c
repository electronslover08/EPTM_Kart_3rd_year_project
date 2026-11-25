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
==== Version                           : V1.0                           ====
==== Nom du(des) fichier(s) source     : Test.C                         ====
====------------------------------------------------------------------  ====
==== Créateur (Nom, Prénom, Localité)  : Paillard Ervan Sion            ====
==== Classe                            : EL 3b	            			====
==== Date de création                  : 17.01.2019                     ====
==== Date de la dernière modification  : 17.01.2019                     ====
====------------------------------------------------------------------	====
==== Description succincte du programme: Code servant utiliser le uP    ====
==== pour commander la direction du karting              				====
==== ----------------------------------                                 ====
==== Code de base pour PIC18F45k22   					                ====
==========================================================================*/

#include <18F45K22.h>
#include "18FK_register.h"

#device ICD=TRUE
#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

// Attribution des pins
#BIT U_Direction = PORTA.3      // = AN3        // Potentiomètre Direction
#BIT ContactDir2 = PORTA.4                      // Fin de course Direction
#BIT ContactDir1 = PORTA.5                      // Fin de course Direction
#BIT Contact_sup = PORTB.0      
#BIT ContactVol1 = PORTB.1                      // Fin de course Volant
#BIT ContactVol2 = PORTB.2                      // Fin de course Volant
#BIT U_Volant    = PORTB.3      // = AN9        // Potentiomètre Volant
#BIT LED         = PORTC.0      
#BIT Bridge_BHI  = PORTC.2                      // Full-Bridge driver B-side-high Input
#BIT Current     = PORTD.3      // = AN23       // Current after INA122
#BIT Bridge_AHI  = PORTD.4                      // Full-Bridge driver A-side-high Input
#BIT Bridge_ALI  = PORTD.5                      // Full-Bridge driver A-side-low  Input
#BIT Bridge_BLI  = PORTD.6                      // Full-Bridge driver B-side-low  Input
#BIT Bridge_Dis  = PORTD.7                      // Full-Bridge Disable PIN$
#BIT Contact_sup_Dir = PORTE.0                  // Contact suplémentaire coté direction

#BIT ADGO =  ADCON0.1                           // Sert pour le CAD

// Fonction 
void Init_volant(void);                         // Fonction d'initialisation des valeurs pot du volant
void Init_Dir(void);                            // Fonction d'initialisation des valeurs pot de la direction
int Pot_volant(void);                           // Prise et modulation de la valeur pot du volant
int Pot_direction(void);                        // Prise et modulation de la valeur pot de la direction
void Direction(int Val_volant, int Val_direction);  // Fonction de déchiffrement des valeurs colant et direction pour commander le moteur
void Direction_G(void);                         // Commande de moteur vers la gauche
void Direction_D(void);                         // Commande de moteur vers la droite
void Stop_Moteur(void);                         // Commande d'arret du moteur
void Send_Val(void);                            // Envoi les valeur à l'EEPROM
void Read_Val(void);                            // Lecture des valeurs EEPROM et réécriture dans la variable 
unsigned char Adc_read(unsigned char channel);  // lecture ADC

// Variables Direction
int Vol = 0;                                    // Valeur actuelle du volant
int Dir = 0;                                    // Valeur actuelle de la direction
int Val_vol_G = 0;                              // Variable de la valeur gauche du volant
int Val_vol_D = 0;                              // Variable de la valeur droite du volant
int Val_dir_G = 0;                              // Variable de la valeur gauche de la direction
int Val_dir_D = 0;                              // Variable de la valeur droite de la direction

// Variables detection de courant
unsigned char Passage = 0;
unsigned char Stop_Passage = 0;

#INT_EXT
void RB0_isr()
{
    Stop_Moteur();
    Init_volant();
    Init_Dir();
    Send_Val();
    Read_Val();
}

//------------------------------------------------------------------------------
// Timer0 : s'execute chaque 25ms
//------------------------------------------------------------------------------
// Calcul du Timer
// PS = temps / (Tcpu * 2^NbitTimer) = (25 * 10^-3) / (0.2 * 10^-6 *2^16) = 1.9
// --> nous allons choisir 2 de PS
// NTimer = temps / (Tcpu * PS) = (25 * 10^-3) / (0.2 * 10^-6 *2) = 62500
// TMR0 = 2^NbitTimer - NTimer = 2^16 - 62500 = 3036(dec) = 0BDC (hex)
// --> TMR0H = 0B (hex) et TMR0L = DC (hex)
//------------------------------------------------------------------------------
#INT_TIMER0                 
void TIMER0_isr(void)               // Mesure de courrant 
{
    //Préchargement du Timer
    TMR0H = 0x0B;
    TMR0L = 0xDC;
    
    unsigned char Current;
    
    Current = Adc_read(23);         // Channel 23 = RD3 = où se trouve l'arrivée current sense sur le PCB
    
    if(Current >= 250)
    {
        Passage ++;
    }
    else
    {
        if(Passage == 1)
        {
            Passage = 0;
        }
    }
    if(Passage == 2)
    {
//        Stop_Moteur();            // Appel de la fonction pour stopper le moteur
        Stop_Passage  ++;
        
        if(Stop_Passage == 2)
        {
            Passage = 0;
            Stop_Passage = 0;
        }
    }
}

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
    
    enable_interrupts(INT_EXT);         // Activation de l'interruption RB0
    ext_int_edge(0, H_to_L);            // Interruption RB0 en flanc descendant
    enable_interrupts(INT_TIMER0);      // Activation de l'interruption timer
    enable_interrupts(GLOBAL);          // Activation globale des interruptions
    
    T0CON = 0b10001000;                 // Parametrage du timer 0
    Bridge_Dis = 1;
   
    Read_Val();                         // Lecture de l'EEPROM
}

unsigned char Adc_read(unsigned char channel)
{         
	ADCON0 = (channel << 2) + 1;        // attribution du channel
    delay_us(10);                       // delay
	ADGO = 1;                           // Début de la conversion
	while(ADGO) {}                      // Tant que la conversion n'est pas finie 
	return (ADRESH);                    // on retourne la valeur convertie
}


//Main 
void main(void)
{
    Init();                         // Appel d'initialisation des PORTs
    
    while(1)
    {
        Vol = Pot_volant();         //Récupération de la valeur du potentiomètre du volant
        Dir = Pot_direction();      //Récupération de la valeur du potentiomètre de la direction
        Direction(Vol, Dir);        //Appel de la fonction pour commander le moteur
    }
    
}

void Init_volant()
{
    while(!ContactVol1); // ou 2    // Attendre que le capteur soit pousser
    Val_vol_G = Adc_read(9);        // Récupération de la valeur maximale du coté sur le volant
    while(!ContactVol2); // ou 1    // Attendre que le capteur soit pousser
    Val_vol_D = Adc_read(9);        // Récupération de la valeur minimale du coté sur le volant
}

void Init_Dir()
{
    Direction_G();                  // Fait tourner le moteur vers la gauche
    while(!ContactDir1); // ou 2    // Attendre que le capteur soit pousser
    Stop_Moteur();                  // Arreter le moteur
    Val_dir_G = Adc_read(3);        // Récupération de la valeur maximale du coté
    Direction_D();                  // Fait tourner le moteur vers la droite
    while(!ContactDir2); // ou 1    // Attendre que le capteur soit pousser 
    Val_dir_D = Adc_read(3);        // Récupération de la valeur minimale du coté
}

int Pot_volant()
{
    int Val_vol = 0;                // Variable pour contenir la valeur actuelle du volant
    int angle = 0;                  // Variable pour avoir la valeur/100 du volant
    
    Val_vol = Adc_read(9);          // Prise de la valeur du potentiomètre
    angle = (long)(100*(long)(Val_vol - Val_vol_D)) / (long)(Val_vol_G - Val_vol_D);
                                    // Calcul de la valeur/100 du potentiomètre
    
    if(angle >= 95)                 // Sécurité contre le dépacement de valeur
    {                               //
        angle = 95;                 //
    }                               //
    if(angle <= 5)                  //
    {                               //
        angle = 5;                  //
    }                               //
    
    return angle;
}

int Pot_direction()
{
    unsigned char Val_dir ;         // Variable pour contenir la valeur actuelle de la direction
    int angle = 0;                  // Variable pour avoir la valeur/100 de la direction
    
    Val_dir = Adc_read(3);          // Prise de la valeur du potentiomètre
    angle = (long)(100*(long)(Val_dir - Val_dir_D)) / (long)(Val_dir_G - Val_dir_D);
                                    // Calcul de la valeur/100 du potentiomètre
    
    if(angle >= 95)                 // Sécurité contre le dépacement de valeur
    {                               //
        angle = 95;                 //
    }                               //
    if(angle <= 5)                  //
    {                               //
        angle = 5;                  //  
    }                               //
    
    return angle;
}

void Direction(int Val_volant, int Val_direction)
{
    if(Val_volant == Val_direction)         // Quand les deux valeurs sont égales
    {
        Stop_Moteur();                      // Appel de fonction pour stopper le moteur
    }
    else                                    // Sinon
    {
        if(Val_volant > Val_direction)      // Si volant est plus grand que la direction
        {
            Direction_G();                  // Appel de fonction pour faire aller le moteur vers la gauche
        }
        if(Val_volant < Val_direction)      // Si volant est plus petit que la direction
        {
            Direction_D();                  // Appel de fonction pour faire aller le moteur vers la droite
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

void Send_Val()
{
    write_eeprom(0,Val_vol_G); //Vol_G = 0  // Ecrire dans l'eeprom la valeur
    write_eeprom(1,Val_vol_D); //Vol_D = 1  // Ecrire dans l'eeprom la valeur
    write_eeprom(2,Val_dir_G); //Dir_G = 2  // Ecrire dans l'eeprom la valeur
    write_eeprom(3,Val_dir_D); //Dir_D = 3  // Ecrire dans l'eeprom la valeur
    write_eeprom(25,0xAC);                  // Valeur que l'on ira lire à la lecture pour savoir si
                                            // c'est la première utilisation ou si la mémoir est
                                            // déja remplie.
}

void Read_Val()
{
    if(read_eeprom(25) == 0xAC)             // Est-ce que la mémoire est vide ou non ?
    {
        Val_vol_G = read_eeprom(0);         // Oui : réécrire
        Val_vol_D = read_eeprom(1);         //
        Val_dir_G = read_eeprom(2);         //
        Val_dir_D = read_eeprom(3);         //
    }
    else
    {
        Val_vol_G = 255;                    // Non : Ecrire les valeurs de "base"
        Val_vol_D = 0;                      //
        Val_dir_G = 255;                    //
        Val_dir_D = 0;                      //
    }
}