//*******************************************************************************************************************
// Programme qui mémorise avec RB0 la valeur présente sur PORTA en EEPROM
// Affiche la valeur mémorisée sur l'affichage LCD
// Développé par : eggn
// Date          : 04.05.2011
//*********************************************************************************************************************/

#include <18F4520.h>
#include "18F_register.h"

#DEVICE HIGH_INTS=TRUE
#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)
#bit LED_NO_ACK = PORTD.0

#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST
int v_lue;
//------------------------------------------------------------------------------
// Routine d'initialisation des ports du microcontrôleur
void Init(void)
{
	TRISB=0xFF;
	TRISD=0;
	TRISA=0x0F;
	PORTD=0;
	ADCON1=0x0F;

	enable_interrupts(GLOBAL);
	enable_interrupts(INT_EXT);
	enable_interrupts(INT_EXT1);
	ext_int_edge(H_to_L);

	i2c_start();
	i2c_write(0x50);	//écris sur l'écran
	i2c_write(0xfe);	//commande
	i2c_write(0x52);	//contraste
	i2c_write(0x1E);	//fixe le niveau de contraste à 1E
	i2c_write(0xfe);	//commande	
	i2c_write(0x53);	//set un back light moyen
	i2c_write(8);		//niveau de back light de 1 à 8
	i2c_write(0xfe);	//commande	
	i2c_write(0x51);	//clear screen
	i2c_stop();
}

//----------------------------------------------------------------------------------
// EEPROM et écran LCD
//----------------------------------------------------------------------------------
// Interruption 0 pour stocker un caractère dans l'EEPROM
#INT_EXT
void RB0_isr(void)
{
	int v_lue;
	i2c_start();
	i2c_write(0xa0);			//écris sur L'EEPROM
	i2c_write(0x00);			//charge l'adresse 0x00
	v_lue=0x30+(PORTA&0x0F);	//récupère la valeur lue ajoute l'offset ascii 
	i2c_write(v_lue);			//mémorise la valeur lue
	i2c_stop();
}

//------------------------------------------------------------------------------
// Interruption 1 pour lire et afficher le caractère sur l'écran
#INT_EXT1
void RB1_isr(void)
{
	i2c_start();
	i2c_write(0xa0);			//écris sur L'EEPROM
	i2c_write(0x00);			//charge l'adresse 0x00
	i2c_start();				//aaaaaaaaaaatention
	i2c_write(0xa1);			//lecture EEPROM
	v_lue = i2c_read(0);		//charge la valeur lue dans v_lue
	i2c_stop();	

	i2c_start();
	i2c_write(0x50);
	i2c_write(0xfe);
	i2c_write(0x51);	//clear screen
	i2c_write(0xfe);
	i2c_write(0x46);	//cursor home
	i2c_write(v_lue);
	i2c_stop();
}

//------------------------------------------------------------------------------

void main(void)
{
	Init();
	while(1)
	{
			
				
	}
}