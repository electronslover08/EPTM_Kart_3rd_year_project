//*************************************************************************
//*  EMVS
//*  Utilise la carte LEDs en sortie
//*  Auteur: Barman Gael
//*  Date: 17.01.2008
//*************************************************************************
// Charge le PORTD avec le PORTC
//*************************************************************************
   
#include <18F4520.h>
#include "18F_register.h"

#use delay(clock=20000000)
#use i2c(master, slow, sda=PIN_C4, scl=PIN_C3, FORCE_HW)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST, NOPBADEN

#BIT LED_NO_ACK = PORTD.0

int v_inc = 0x00;

//*****************************************************************
// Routine d'initialisation des ports du uP
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
// Interruption EXT0: chaque impulsion sur RB0 provoque une écriture
// de l'EEPROM
//*****************************************************************
#INT_EXT
RB0_isr()
{
	int v_lue;
	//écriture dans l'EEPROM
	v_lue = 0x30+(PORTA&0x0F);
	i2c_start();
	i2c_write(0xA0);
	i2c_write(v_inc);
	i2c_write(v_lue);
	i2c_stop();

	v_inc++;
}
//*****************************************************************
// Interruption EXT1: une impulsion sur RB1 provoque la lecture
// de l'EEPROM
//*****************************************************************
#INT_EXT1
RB1_isr()
{
	int i;
	int data;
	int tbl_char[];
	//***********************************************************
	//lecture de l'EEPROM & chargement dans tbl_char[i] 
	i2c_start();
	i2c_write(0xA0);
	i2c_write(i);
	i2c_start();
	i2c_write(0xA1);
	for(i=0;i<v_inc-1;i++)
	{
		tbl_char[i] = i2c_read(1);	//doit continuer à envoyer
	}
	tbl_char[v_inc] = i2c_read(0);	//termine la communication
	i2c_stop();
	
	//***********************************************************
	//écriture sur l'écran LCD du tableau tbl_char[i]
	i2c_start();
	i2c_write(0x50);

	for(i=0;i<v_inc;i++)
	{
		i2c_write(tbl_char[i]);
		
	}
	i2c_stop();
	v_inc = 0;
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
