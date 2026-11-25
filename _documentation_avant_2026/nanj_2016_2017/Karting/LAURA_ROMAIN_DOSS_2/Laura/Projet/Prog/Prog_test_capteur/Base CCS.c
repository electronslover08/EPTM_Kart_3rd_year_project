#include <18F4520.h>
#include "18F_register.h"

#device ICD=TRUE
#use delay(clock=20000000)
#fuses NOWDT, HS, NOPUT, NOPROTECT, DEBUG, NOBROWNOUT, NOLVP, NOCPD, NOWRT, NOXINST
#use i2c(master,slow,sda=PIN_C4,scl=PIN_C3,FORCE_HW)	//Force à utiliser le Hardware du microcontrôleur 
#BIT Trigger = PORTB.1
#BIT Echo = PORTB.0
#BIT Capteur_ON = PORTD.0

#define I2CADDRESS 0X88						//Adresse de l'écran

//Variables globales
int indexEcho=0;
int distance=0;
int Centaine=0;
int Dizaine=0;
int Unit=0;

//Prototypes de fonctions
void sendPulse(void);

void init(void)
{	
	TRISA=PORTA=0b00010000;	//capteur
	TRISB=PORTB=0b11110001;

	enable_interrupts(INT_EXT);			//Autorise l'interruption RB0
	enable_interrupts(INT_RB);			//Autorise l'interruption RB
	disable_interrupts(INT_TIMER0);		//Désactive l'interruption timer
	ext_int_edge(0,L_to_H);				//Configure RB0 sur le flanc montant
	enable_interrupts(GLOBAL);			//Autorise globalement les interruptions

	T0CON = 0x88;

	//Initialisation de l'écran

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

	//Afficher bonjour sur l'écran
	i2c_start();
	i2c_write(0x50);
	i2c_write(0xFE);
	i2c_write(0x45);
	i2c_write(0x00);
	i2c_write(0x42);	//B
	i2c_write(0x6F);	//o
	i2c_write(0x6E);	//n
	i2c_write(0x6A);	//j
	i2c_write(0x6F);	//o
	i2c_write(0x75);	//u
	i2c_write(0x72);	//r
	i2c_stop();	
}


void Main(void)
{
	init();

	while(1)
	{
		sendPulse();
		distance=(((indexEcho*0.000050)*340)/2)*100;
		Centaine=distance/100;
		Dizaine=(distance-100)/10;
		Unit=(distance-100)-(10*Dizaine);

		if(distance>=100)
		{
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
	
			i2c_start();
			i2c_write(0x50);
			i2c_write(0xFE);
			i2c_write(0x45);
			i2c_write(0x00);
			i2c_write(48+Centaine);
			i2c_write(48+Dizaine);
			i2c_write(48+Unit);
			i2c_stop();
		}
		else
		{
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

			i2c_start();
			i2c_write(0x50);
			i2c_write(0xFE);
			i2c_write(0x45);
			i2c_write(0x00);
			i2c_write(48+Dizaine);
			i2c_write(48+Unit);
			i2c_stop();	
		}
	}	
}

void sendPulse(void)
{
	Capteur_ON=1;
	Trigger=1;
	delay_us(10);
	Trigger=0;
	Capteur_ON=0;	
}

#INT_EXT
void RB0_isr()
{
	TMR0H=0xFF;
	TMR0L=0x06;
	enable_interrupts(INT_TIMER0);
}

#INT_TIMER0
TIMER0_isr()
{
	if(Echo==1)
	{
		indexEcho++;
	 	TMR0H=0xFF;
		TMR0L=0x06;
	}
	else
	{
		disable_interrupts(INT_TIMER0);
		indexEcho=0;
	}
}




