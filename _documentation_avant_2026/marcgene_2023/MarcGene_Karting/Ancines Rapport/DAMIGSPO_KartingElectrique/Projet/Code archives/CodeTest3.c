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
==== Version                           : V3.0                           ====
==== Nom du(des) fichier(s) source     : TestPontH.C                    ====
====------------------------------------------------------------------  ====
==== Créateur (Nom, Prénom, Localité)  : Gsponer Damien Sion            ====
==== Classe                            : EL 3A	            			====
==== Date de création                  : 29.03.2019                     ====
==== Date de la dernière modification  : 04.04.2019                     ====
====------------------------------------------------------------------	====
==== Description succincte du programme: Code servant à faire tourner   ====
==== la direction du kart en fonction des valeurs du volant et la       ====
==== direction.                                                         ====
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

// Fonctions
void Init_Vol(void);                 // Fonction d'initialisation du volant
void Init_Dir(void);                    // Fonction d'initialisation de la direction
int Pot_Volant(void);                   // Prise et modulation de la valeur pot du volant
int Pot_Direction(void);                // Prise et modulation de la valeur pot de la direction
void Direction(int Dir, int Vol);  // Fonction de déchiffrement des valeurs volant et direction pour commander le moteur
void Direction_G(void);                 // Commande de moteur vers la gauche
void Direction_D(void);                 // Commande de moteur vers la droite
void Stop_Moteur(void);                 // Commande d'arret du moteur
unsigned char Adc_read(unsigned char channel);  // lecture ADC
void Send_Val(void);                            // Envoi les valeur à l'EEPROM
void Read_Val(void);                    // Lecture ou réécriture EEPROM

// Variables
int Pot_Vol = 0;                        // Valeur actuelle du volant
int Pot_Dir = 0;                        // Valeur actuelle de la direction
int Val_vol_G = 0;                      // Valeur gauche du volant
int Val_vol_D = 0;                      // Valeur droite du volant
int Val_dir_G = 0;                      // Valeur gauche de la direction
int Val_dir_D = 0;                      // Valeur droite de la direction
unsigned char Passage = 0;              // Variable de passage pour la
unsigned char Stop_Passage = 0;         // détection de courant

// Variables de passage qui permettent au moteur de s'arrêter une seule fois
// avant un changement de direction
int A = 1;
int B = 1;
int init1 = 1;

/* =========================================================================
==== Fonction     : Interruption RB0                                    ====
==== 04.04.2019                                                         ====
====--------------------------------------------------------------------====
==== Initialise la direction lors de l'activation de l'interruption.    ====
==== Il va récupérer les valeurs max / min du volant et de la direction.====
========================================================================= */
/*#INT_EXT
void RB0_isr()
{
    Stop_Moteur();         // Arrête le moteur s'il tourne
    Init_Vol();            // Initialise les valeurs max et min du vol
    Init_Dir();            // Initialise les valeurs max et min de la direction
    Send_Val();            // Envoie les valeurs à l'EEPROM
    Read_Val();            // Lecture de l'EEPROM
}*/

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
    
    unsigned char Current;          // Variable qui va recevoir la val du courant
    
    Current = Adc_read(23);         // Channel 23 = RD3 = current sense
    
    if(Current >= 250)              // Si le courant dépasse ou est égal à 250
    {
        Passage ++;                 // Incrémente la variable
    }
    else                            // sinon
    {
        if(Passage == 1)            // Si on est à 1 passage
        {
            Passage = 0;            // Passe la variable à 0
        }
    }
    if(Passage == 2)                // Si on est à 2 passage
    {
//        Stop_Moteur();              // Appel de la fonction pour stopper le moteur
        Stop_Passage  ++;           // Incrémente la variable qui va stopper le passage
        
        if(Stop_Passage == 2)       // Si 2 stop_passage sont incémenté
        {
            Passage = 0;            // Passe les 2 variables à 0
            Stop_Passage = 0;       
        }
    }
}

/* =========================================================================
====Fonction     : Init                                                 ====
====--------------------------------------------------------------------====
==== Initialise les PORTs d'entrées/sorties pour faire fonctionner ce   ====
==== code.                                                              ====
====--------------------------------------------------------------------==== */
//Initialisation des PORTs
void Init(void)
{       
    PORTA = 0xFF;                    // Entrée RA3/4/5
	TRISA = 0xFF;
    ANSELA = 0;                      // PORTA

    PORTB = 0xFF;                    // Entrée RB0/1/2
    TRISB = 0xFF;    
    ANSELB = 0;                      // PORTB
    
    PORTC = 0x00;                    // Sortie RC0/2
	TRISC = 0x00;	
    ANSELC = 0;                      // PORTC
    
    PORTD = 0x08;                    // Entrée RD3 + Sortie RD4/5/6/7
	TRISD = 0x08;	
    ANSELD = 0x0;                    // PORTD
   
    PORTE = 0xFF;                    // Connecteur sup
	TRISE = 0xFF; 	
    ANSELE = 0;                      // PORTE
    
    enable_interrupts(INT_EXT);         // Activation de l'interruption RB0
    ext_int_edge(0, H_to_L);            // Interruption RB0 en flanc descendant
    enable_interrupts(INT_TIMER0);      // Activation de l'interruption timer
    enable_interrupts(GLOBAL);          // Activation globale des interruptions
    T0CON = 0b10001000;                 // Parametrage du timer 0
    
    Bridge_Dis = 1;                     // Stop le moteur
    
    //Read_Val();                         // Lit les valeurs de l'EEPROM
}

/* =========================================================================
====Fonction     : Adc_Read                                             ====
====--------------------------------------------------------------------====
==== Va convertir en entrée analogique du PIC en une valeur digitale.   ====
====--------------------------------------------------------------------====
==== channel     :unsigned char:  Variable qui sélectionne la canal     ==== 
====--------------------------------------------------------------------====
==== return      ADRESH :   Retourne la valeur convertie                ====
============================================================================ */
unsigned char Adc_read(unsigned char channel)
{         
	ADCON0 = (channel << 2) + 1;    // Attribution du channel
    delay_us(10);                   // Delay
	ADGO = 1;                       // Début de la convertion
	while(ADGO) {}                  // Tant que la convertion n'est pas finie 
	return (ADRESH);                // on retourne la valeur convertie
}
//Main 
void main(void)
{
    Init();                         // Initialise les PORTs
    
    while(1)                        // Boucle infinie
    {
        if(init1 == 1)
        {
            Stop_Moteur();         // Arrête le moteur s'il tourne
            Init_Vol();            // Initialise les valeurs max et min du vol
            Init_Dir();            // Initialise les valeurs max et min de la direction
            //Send_Val();            // Envoie les valeurs à l'EEPROM
            //Read_Val();            // Lecture de l'EEPROM*/
        }
        else
        {
        Pot_Dir = Pot_Direction();          // Val de la direction donnée à la variable
        Pot_Vol = Pot_Volant();             // Val du volant donnée à la variable
        Direction(Pot_Dir, Pot_Vol);      // Appel la fonction qui gère la direction
        }
    }
}

/* =========================================================================
====Fonction     : Init_Vol                                             ====
====--------------------------------------------------------------------====
==== Lorsque l'utilisateur le veux, il peut choisir de réinitialiser    ====
==== les valeurs de fin de course (conseillé).                          ====
==== Grâce à ces valeurs on connait le champ d'action complet du volant.====
====--------------------------------------------------------------------==== */
void Init_Vol(void)
{
    while(!ContactVol1); // ou 2    // Attends que la fin de course soit poussée
    Val_vol_G = Adc_read(9);        // Récupération de la valeur maximale sur le volant
    delay_ms(1000);
    while(!ContactVol2); // ou 1    // Attendre que la fin de course soit poussée
    Val_vol_D = Adc_read(9);        // Récupération de la valeur minimale sur le volant
}

/* =========================================================================
====Fonction     : Init_Dir                                             ====
====--------------------------------------------------------------------====
==== Lorsque l'utilisateur le veux, il peut choisir de réinitialiser    ====
==== les valeurs de fin de course (conseillé).                          ====
==== Grâce à ces valeurs on connait le champ d'action complet du de la  ====
==== direction.                                                         ====
====--------------------------------------------------------------------==== */
void Init_Dir(void)
{
    do  
    {
        Direction_G();                  // Fait tourner le moteur vers la gauche
    }
    while(ContactDir1 == 0);        // ou 2    // Attends que la fin de course soit poussée
    //Stop_Moteur();                  // Arrête de tourner
    Val_dir_G = Adc_read(3);        // Récupération de la valeur maximale du coté
    do
    {
        Direction_D();                  // Fait tourner le moteur vers la droite
    }
    while(ContactDir2 == 0); // ou 1    // Attends que la fin de course soit poussée 
    Val_dir_D = Adc_read(3);        // Récupération de la valeur minimale du coté
    init1 = 0;
    /*while(ContactDir1 == 0)
    {
        Direction_G();
    }
    Val_dir_G = Adc_read(3);
    while(ContactDir2 == 0)
    {
        Direction_D();
    }
    Val_dir_D = Adc_read(3);*/
}

/* =========================================================================
====Fonction     : Pot_Volant                                           ====
====--------------------------------------------------------------------====
==== Va convertir les valeurs récupérer avec les initialisations et les ====
==== valeur réels données en directe par le volant. On transforme la    ====
==== valeur analogique du volant en angle que l'on peut comparer avec   ====
==== la direction. Les fins de courses jouent aussi un rôle important   ====
==== dans le calcul de l'angle.                                         ====
====--------------------------------------------------------------------==== */
int Pot_Volant(void)
{
    unsigned char Val_vol = 0;                // Variable pour contenir la valeur actuelle du volant
    int angle = 0;                  // Variable pour avoir la valeur/100 du volant
    
    Val_vol = Adc_read(9);          // Prise de la valeur du potentiomètre
    angle = (long)(100*(long)(Val_vol - Val_vol_D)) / (long)(Val_vol_G - Val_vol_D);
                                    // Calcul de la valeur/100 du potentiomètre
    // Sécurité contre le dépacement de valeur
    if(angle >= 95)                 // Laisse une petite marge d'erreur avec la
    {                               // valeur min et la valeur max
        angle = 95;                 // Si elle dépasse 95 ==> Reste à 95 au max
    }                               
    if(angle <= 5)                  // Si elle va en dessous de 5
    {                               
        angle = 5;                  // Reste à 5 au minimum
    }                               
    
    return angle;                   // Retourne la valeur de l'angle
}

/* =========================================================================
====Fonction     : Pot_Direction                                        ====
====--------------------------------------------------------------------====
==== Va convertir les valeurs récupérer avec les initialisations et les ====
==== valeur réels données en directe par la direction. On transforme la ====
==== valeur analogique de la direction en angle que l'on peut comparer  ====
==== avec le volant. Les fins de courses jouent aussi un rôle           ====
==== important dans le calcul de l'angle.                               ====
====--------------------------------------------------------------------==== */
int Pot_Direction()
{
    unsigned char Val_dir ;         // Variable pour contenir la valeur actuelle de la direction
    int angle = 0;                  // Variable pour avoir la valeur/100 de la direction
    
    Val_dir = Adc_read(3);          // Prise de la valeur du potentiomètre
    angle = (long)(100*(long)(Val_dir - Val_dir_D)) / (long)(Val_dir_G - Val_dir_D);
                                    // Calcul de la valeur/100 du potentiomètre
    // Sécurité contre le dépacement de valeur
    if(angle >= 95)                 // Laisse une petite marge d'erreur avec la
    {                               // valeur min et la valeur max
        angle = 95;                 // Si elle dépasse 95 ==> Reste à 95 au max
    }                               
    if(angle <= 5)                  // Si elle va en dessous de 5
    {                               
        angle = 5;                  // Reste à 5 au minimum
    }                               
    return angle;                   // Retourne la valeur de l'angle
}

/* =========================================================================
====Fonction     : Direction                                            ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Va gérer les différents cas et va diriger le karting en fonction   ====
==== de la valeur des entrées.                                          ====
====--------------------------------------------------------------------====
==== Dir :int  :  Valeur lue sur le potentiomètre                       ====
==== Vol :int  :  Valeur lue sur le potentiomètre                       ====
====--------------------------------------------------------------------==== */
void Direction(int Dir, int Vol)
{
    // Stoper le moteur
    if(Dir == Vol)          // Si les 2 valeurs sont égales
    {
        Stop_Moteur();      // Empêche de moteur de changer de direction
    }
    // Tourner à gauche
    if(Dir > Vol)           // Si la direction est au dessus du volant
    {
        Direction_G();      // Tourne les roues à gauche
    }
    // Tourner à droite
    if(Dir < Vol)           // Si le volant est au dessus de la direction
    {
        Direction_D();      // Tourne les roues à droite
    }
}
/* =========================================================================
====Fonction     : Direction_G                                          ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Permet au moteur de tourner à gauche en modifiant les entrées du   ====
==== pont-H. Stop le moteur lors du premier passage grâce au 1er test.  ====
==== Une led nous indique si l'on va à gauche ou à droite.              ====
====--------------------------------------------------------------------==== */
void Direction_G(void)              // Fais tourner le moteur vers la gauche
{
    if(A = 1)                       // Test de passage (Protection c-c)
    {
        Bridge_Dis = 1;             // Arrêt du moteur
        Bridge_ALI = 0;             // Désactivation des sorties non voulues
        Bridge_BHI = 0;             //
        A++;                        // Incrémente variable de passage
        B = 1;                      // Remet l'autre variable de passage à 1
    }
    
    LED = 1;                        // Led d'indication
    Bridge_Dis = 0;                 // Désactive l'arrêt du moteur
    Bridge_AHI = 1;                 // Activation des sorties pour 
    Bridge_BLI = 1;                 // le faire tourner à droite
}

/* =========================================================================
====Fonction     : Direction_D                                          ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Permet au moteur de tourner à droite en modifiant les entrées du   ====
==== pont-H. Stop le moteur lors du premier passage grâce au 1er test.  ====
==== Une led nous indique si l'on va à gauche ou à droite.              ====
====--------------------------------------------------------------------==== */
void Direction_D(void)              // Fait tourner le moteur vers la droite
{
    if(B = 1)                       // Test de passage (Protection c-c)
    {
        Bridge_Dis = 1;             // Arrêt du moteur
        Bridge_AHI = 0;             // Désactivation des sorties non voulues
        Bridge_BLI = 0;             //
        B++;                        // Incrémente variable de passage
        A = 1;                      // Remet l'autre variable de passage à 1
    }

    LED = 0;                        // Led d'indication
    Bridge_Dis = 0;                 // Désactive l'arrêt du moteur
    Bridge_ALI = 1;                 // Activation des sorties pour
    Bridge_BHI = 1;                 // le faire tourner à gauche
}

/* =========================================================================
====Fonction     : Stop_Moteur                                          ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Permet au moteur de s'arrête lors de changement d'état pour ne pas ====
==== entraîner un c-c sur le pont-H.                                    ====
====--------------------------------------------------------------------==== */
void Stop_Moteur(void)              // Arrête d'alimenter le moteur
{
    Bridge_AHI = 0;                 // Met toutes les commandes du moteur à 0
    Bridge_ALI = 0;                 // = sert de protection pour
    Bridge_BHI = 0;                 // les commandes futures
    Bridge_BLI = 0;                         
    
	Bridge_Dis = 1;                 // Arrêt du moteur
    
    A = 1;                          // Remet les variables de passage
    B = 1;                          // à 1 pour le passage dans le test
    
    delay_cycles(1);                // Attends pendant une instruction
}

void Send_Val(void)
{
    write_eeprom(0,Val_vol_G); //Vol_G = 0  // Ecrire dans l'eeprom la valeur
    write_eeprom(1,Val_vol_D); //Vol_D = 1  // Ecrire dans l'eeprom la valeur
    write_eeprom(2,Val_dir_G); //Dir_G = 2  // Ecrire dans l'eeprom la valeur
    write_eeprom(3,Val_dir_D); //Dir_D = 3  // Ecrire dans l'eeprom la valeur
    write_eeprom(25,0xAC);                  // Valeur que l'on ira lire à la lecture pour savoir si
                                            // c'est la première utilisation ou si la mémoire est
                                            // déja remplie.
}

void Read_Val(void)
{
    /*if(read_eeprom(25) == 0xAC)             // Est-ce que la mémoire est vide ou non ?
    {
        Val_vol_G = read_eeprom(0);         // Oui : réécrire
        Val_vol_D = read_eeprom(1);         //
        Val_dir_G = read_eeprom(2);         //
        Val_dir_D = read_eeprom(3);         //
    }
    else
    {*/
        Val_vol_G = 255;                    // Non : Ecrire les valeurs de "base"
        Val_vol_D = 0;                      //
        Val_dir_G = 255;                    //
        Val_dir_D = 0;                      //
    //}
}