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
==== Date de création                  : 09.05.2019                     ====
==== Date de la dernière modification  : 09.05.2019                     ====
====------------------------------------------------------------------	====
==== Description succincte du programme: Code servant à faire tourner   ====
==== la direction du kart en fonction des valeurs du volant et la       ====
==== direction.                                                         ====
==========================================================================*/

#include <18F45K22.h>
#include "18FK_register.h"

#device ICD=TRUE
//#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)

#fuses NOWDT, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN, NOPLLEN, HSH

//Attribution des PINS
//Fin de course volant/direction
#BIT Cont_Dir2 = PORTA.5
#BIT Cont_Dir1 = PORTA.4
#BIT Cont_Sup = PORTB.0                 //Supplémentaire
#BIT Cont_Vol1 = PORTB.1
#BIT Cont_Vol2 = PORTB.2
//Entrée driver HIP4081
#BIT Bridge_BHI = PORTC.2
#BIT Bridge_AHI = PORTD.4
#BIT Bridge_ALI = PORTD.5
#BIT Bridge_BLI = PORTD.6
#BIT Bridge_Dis = PORTD.7
//Convertisseur A/D
#BIT ADGO =  ADCON0.1                   //Sert pour convertir en digital
//Autres
#BIT LED = PORTC.0                      //Led d'affichage
#BIT Current = PORTD.3                  //Entrée mesure courant

//Déclaration variable
int Val_dir_G;
int Val_dir_D;
int Val_vol_G;
int Val_vol_D;
int A = 1;                                  //Variable de passage
int B = 1;
int init1;
int Dir;
int Vol;
unsigned char channel=0;
unsigned char var1=0;
//Déclaration fonction
void Init(void);                        //Initialise les ports en I/O
void Direction_G(void);                 //Permet de tourner à gauche
void Direction_D(void);                 //Permet de tourner à droite
void Stop_Moteur(void);                 //Arrête le moteur
void Init_Vol(void);
void Init_Dir(void);
unsigned char Adc_read(unsigned char channel);  //Converti les valeur 
                                        //analogique en valeur digitale
/* =========================================================================
====Fonction     : Init                                                 ====
====--------------------------------------------------------------------====
==== Initialise les PORTs d'entrées/sorties pour faire fonctionner ce   ====
==== code.                                                              ====
====--------------------------------------------------------------------==== */
//Initialisation des PORTs
void Init(void)
{   
    ANSELA = 0x08;
    ANSELB = 0x08;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    //PORTA.0-1-2 = Non-utilisés                            OUT
    //PORTA.3 = Valeur capteur d'angle direction            IN
    //PORTA.4-5 = Contacte fin de course direction          IN
    TRISA = 0b00111000;
    //PORTB.0-1-2 = Contacte fin de course volant           IN
    //PORTB.3 = Valeur potentiomètre volant                 IN
    //PORTB.4 = Contacte supplémentaire                     IN
    //PORTB.5 = Non-utilisé                                 OUT
    //PORTB.6-7 = Sortie com. avec ICD3                      OUT
    TRISB = 0b00011111;
    //PORTC.0 = LED d'affichage                             OUT
    //PORTC.1-3-4-5-6-7 = Non-utilisés                      OUT
    //PORTC.2 = Sortie driver BHI                           OUT
    TRISC = 0b00000000;
    //PORTD.0-1-2 = Non-utilisés                            "IN"
    //PORTD.3 = Mesure de courant                           OUT
    //PORTD.4-5-6 = Sortie driver AHI - ALI - BLI           OUT
    //PORTD.7 = Sortie driver disable                       OUT
    TRISD = 0b00000000;
    PORTD = 0b00000000;
    //PORTE.0 = Contacte supplémentaire                     IN
    //PORTE.1-2 = Non-utilisés                              OUT
    TRISE = 0b00000001;
    ADCON0=0b00000001;
    ADCON1=0b00001110;
    ADCON2=0b00011101;
    enable_interrupts(INT_AD);
    enable_interrupts(GLOBAL);
}

/* =========================================================================
====Fonction     : Adc_Read                                             ====
====--------------------------------------------------------------------====
==== Va convertir en entrée analogique en une valeur digitale.          ====
====--------------------------------------------------------------------====
==== channel     :unsigned char:  Variable qui sélectionne la canal     ==== 
====--------------------------------------------------------------------====
==== return      ADRESH :   Retourne la valeur convertie                ====
============================================================================ */
/*unsigned char Adc_read(unsigned char channel)
{         
	ADCON0 = (channel << 2) + 1;    // Attribution du channel
    delay_us(10);                   // Delay
	ADGO = 1;                       // Début de la conversion
	while(ADGO==1) {
    }                  // Tant que la conversion n'est pas finie 
	return (ADRESH);                // on retourne la valeur convertie
}*/

#INT_AD
void Adc_read(void)
{
    if (channel==3) {
        Dir=ADRESH;
        var1=1;
    }
    if (channel==9) {
        Vol=ADRESH;
        var1=0;
    }
}

//Main 
void main(void)
{
    Init();                         // Initialise les PORTs
    Stop_Moteur();
    //Init_Vol();
    //();
    while(1)                        // Boucle infinie
    {
        /*Dir = Adc_read(3);          
        Vol = Adc_read(9);*/
        if ((ADGO==0)&&(var1==0)){
            channel = 3;
            ADCON0 = 13;
            delay_us(10);
            ADGO=1; //start conversion
        }
        //var1=1;
        if ((ADGO==0)&&(var1==1)){
            channel = 9;
            ADCON0 = 37;
            delay_us(10);
            ADGO=1; //start conversion
        }
        
        if(Dir == Vol)              // Si les 2 val sont égales
        {
            Stop_Moteur();          // Empêche de moteur de changer de direction
        }
        if(Dir >= Vol)               // Si la direction est au dessus du volant
        {
            Direction_G();          // Tourne les roues à gauche
        }
        if(Dir <= Vol)               // Si le volant est au dessus de la direction
        {
            Direction_D();          // Tourne les roues à droite
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
    while(!Cont_Vol1); // ou 2    // Attends que la fin de course soit poussée
    Val_vol_G = Adc_read(9);        // Récupération de la valeur maximale sur le volant
    LED = 1;
    delay_ms(1000);
    LED = 0;
    while(!Cont_Vol2); // ou 1    // Attendre que la fin de course soit poussée
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
    while(Cont_Dir1 == 0);        // ou 2    // Attends que la fin de course soit poussée
    //Stop_Moteur();                  // Arrête de tourner
    Val_dir_G = Adc_read(3);        // Récupération de la valeur maximale du coté
    LED = 1;
    delay_ms(1000);
    LED = 0;
    do
    {
        Direction_D();                  // Fait tourner le moteur vers la droite
    }
    while(Cont_Dir2 == 0); // ou 1    // Attends que la fin de course soit poussée 
    Val_dir_D = Adc_read(3);        // Récupération de la valeur minimale du coté
    init1 = 0;
}
/* =========================================================================
====Fonction     : Direction_G                                          ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Permet au moteur de tourner à gauche en modifiant les entrées du   ====
==== pont-H. Stop le moteur lors du premier passage grâce au 1er test.  ====
==== Une led nous indique si l'on va à gauche ou à droite.              ====
====--------------------------------------------------------------------==== */
void Direction_G(void)              // Fait tourner le moteur à gauche
{ 
    if(A == 1)                       // Test de passage (Protection c-c)
    {
        Stop_Moteur();              // Stop le moteur avant de le faire tourner dans l'autre sens
        A++;                        // Incrémente variable de passage
        B = 1;                      // Remet la variable de passage à 1
    }
    LED = 1;                        // Led d'indication
    Bridge_AHI = 1;                 // Active les sorties pour le faire tourner à gauche
    delay_ms(100);                  // Protection contre les courts-circuits
    Bridge_BLI = 1;
}

/* =========================================================================
====Fonction     : Direction_D                                          ====
====by Gspd 03.04.2019                                                  ====
====--------------------------------------------------------------------====
==== Permet au moteur de tourner à droite en modifiant les entrées du   ====
==== pont-H. Stop le moteur lors du premier passage grâce au 1er test.  ====
==== Une led nous indique si l'on va à gauche ou à droite.              ====
====--------------------------------------------------------------------==== */
void Direction_D(void)              // Fait tourner le moteur à droite
{
    if(B == 1)                       // Test de passage (Protection c-c)
    {
        Stop_Moteur();              // Stop le moteur avant de le faire tourner dans l'autre sens
        B++;                        // Incrémente variable de passage
        A = 1;                      // Remet la variable de passage à 1

    }
    LED = 0;                        // Led d'indication
    Bridge_BHI = 1;                 // Activation des sorties pour le faire tourner à droite
    delay_ms(100);                  // Protection contre les courts-circuits
    Bridge_ALI = 1;                 
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
    Bridge_Dis = 1;                 // Désactive le moteur
    Bridge_AHI = 0;                 // Met toutes les commandes du moteur à 0
    Bridge_ALI = 0;                 // = sert de protection pour
    Bridge_BHI = 0;                 // les commandes futures
    Bridge_BLI = 0;                 
    LED=!LED;                       // Inverse la LED
    A = 1;                          // Remet les variables de passage
    B = 1;                          // à 1 pour le passage dans le test
    delay_cycles(1);                // Attends pendant une instruction
    Bridge_Dis = 0;                 // Réactive le moteur
}
