//*************************************************************************
//*  EMVS
//*  Utilise la carte I2C 
//*  Auteur: 
//*  Date: 23.04.2013
//*************************************************************************
// Avec RB0 mémorise sur L'EEPROM les valeurs entrées sur RA1 à RA4 (switch)
// Avec RB1 affiche toutes les valeurs mémorisées sur le LCD
//*************************************************************************
   
#include <18F4520.h>
#include "18F_register.h"

#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN

#BIT LED_NO_ACK = PORTD.0

int v_inc = 0x00;

//*****************************************************************
// Routine d'initialisation des ports du uP et de l'affich. LCD i2C
//*****************************************************************
void Init(void)
{
	ADCON1 = 0x0F;
	TRISB = 0xFF;
	TRISA = 0xFF;

	enable_interrupts(INT_EXT);
	enable_interrupts(INT_EXT1);
	ext_int_edge(0,H_to_L);
	ext_int_edge(1,H_to_L);
	enable_interrupts(GLOBAL);

	i2c_start();
	i2c_write(0x50);
	i2c_write(0xFE);
	i2c_write(0x51);
	i2c_write(0xFE);
	i2c_write(0x52);
	i2c_write(0x1E);
	i2c_write(0xFE);
	i2c_write(0x53);
	i2c_write(8);
	i2c_stop();
}
//*****************************************************************
// Interruption 0
// Lors de chaque impulsion sur le poussoir RB0 une nouvelle valeur
// est mémorisée dans l'EEPROM
//*****************************************************************
#INT_EXT
void RB0_isr(void)
{
	int v_lue;
	v_lue = 0x30+((PORTA&0x1F)>>1);

	i2c_start();
	i2c_write(0xA0);
	i2c_write(v_inc);
	i2c_write(v_lue);
	i2c_stop();

	v_inc++;
}
//*****************************************************************
// Interruption 1
// Après une impulsion sur RB1 toutes les valeurs en EEPROM s'affichent
// Sur l'affichage LCD
// la variable globale v_inc contient le nombre de valeurs mémorisées
//*****************************************************************
#INT_EXT1
void RB1_isr(void)
{
	int i;
	int data;
	// ----------Mise à 0 de l'affichage---------------
	
	i2c_start();
	i2c_write(0x50);
	i2c_write(0xFE);
	i2c_write(0x51);	//Clear display
	i2c_write(0xFE);
	i2c_write(0x46);	//Cursor Home
	i2c_stop();

	for(i=0;i<v_inc;i++)
	{
		i2c_start();
		i2c_write(0xA0);
		i2c_write(i);
		i2c_start();
		i2c_write(0xA1);
		data = i2c_read(0);
		i2c_stop();
	
		i2c_start();
		i2c_write(0x50);
		i2c_write(data);
		i2c_stop();
	}
	v_inc = 0;		// Mise à 0 du nombre de valeurs mémorisées
}

//*****************************************************************
// Main
//*****************************************************************
void main(void)
{
	Init();
	while(1)
	{

	}
}
