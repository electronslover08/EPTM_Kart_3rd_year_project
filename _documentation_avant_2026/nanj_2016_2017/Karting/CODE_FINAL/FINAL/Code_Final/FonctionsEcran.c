int reinitialise[3] = {0xAA, 0x21};
int ick_tab [4] = {0xAA, 0x43, 0x4B};
int ig_tab[3] = {0xAA, 0x47};
int itw_tab[4] = {0xAA, 0x54, 0x57};
int ith_tab[4] = {0xAA, 0x54, 0x48};
int ita_tab[5] = {0xAA, 0x54, 0x41, 0xFF};
int iaf_tab[4] = {0xAA, 0x41, 0x46};
int idt_tab[4] = {0xAA, 0x44, 0x54};
int iew_tab[4] = {0xAA, 0x45, 0x57};
int ier_tab[4] = {0xAA, 0x45, 0x52};
int iit_tab[5] = {0xAA, 0x49, 0x3F, 0x54};
int iid_tab[5] = {0xAA, 0x49, 0x3F, 0x44};
int ici_tab[4] = {0xAA, 0x43, 0x49};
int icb_tab[4] = {0xAA, 0x43, 0x42};
int iDe_tab[4] = {0xAA,0x44, 0x65};
int iDC_tab[4] = {0xAA, 0x44, 0x43};

int GraphVoltage_tab[15] = {255,250,245,240,230,220,210,200,180,150,130,110,90,70,50};
int ACK=1;
int index;
#define I2CADDRESS 0X88						//Adresse de l'écran
int DataRecu[5] = {5,5,5,5,5};

//==========================================================================
//Tableaux de commandes pour iLCD :									========
//---------------------------------									========
int Tab_Phrase[80];
//==========================================================================

void StartConnection(int NbrCharSend);	//commence la connection, dit le nbr de byte à envoyer
void reinit(void);	//réinitialise l'écran
void iCK(int xH, int xL, int yH, int yL);	//définis un point de départ
void iG(int img);	//affiche un image
void iTW(int yH, int yL);	//définis la largeur du champ tactile depuis le point de départ
void iTH(int xH, int xL);	//définis la longueur du champ tactile depuis le point de départ
void iTA(int nom);	//défini le nom du champ tactile
void iAF(int ChoixEcritH, int ChoixEcritL);	//dit avec quelle écriture on écrit le futur texte
void iDT(int phrase[]);	//écris un texte
void iDe(int end_xh,int end_xl, int end_yh, int end_yl); //Efface une zone de l'écran
void writeLine(int end_xh,int end_xl, int end_yh, int end_yl); //Fais une ligne
void iDC(int RadiusHigh,int RadiusLow); //Cercle
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: commence la connection, dit le nbr de byte à envoyer
//-------------------------------------------------------------------------
void StartConnection(int NbrCharSend)
{
	// lis la valeur de la luminosité de l'EEPROM puis on dit que c'est OldBlackLight et blacklight (les 2 doivent être égales au début)
	i2c_start();
	do{ ACK = i2c_write(I2CADDRESS);} while(ACK == 1);
	// envoie l'information du nombre de caractère que l'on va envoyer
	do{ ACK = i2c_write(0x05);} while(ACK == 1);
	do{ ACK = i2c_write(NbrCharSend);} while(ACK == 1);
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: réinitialise l'écran
//-------------------------------------------------------------------------
void reinit(void)
{
	for(index=0; index<2; index++){	i2c_write(reinitialise[index]);}
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis un point de départ
//-------------------------------------------------------------------------
void iCK(int xH, int xL, int yH, int yL)
{
	for(index=0; index<3; index++){	i2c_write(ick_tab[index]);}
	i2c_write(xH);
	i2c_write(xL);
	i2c_write(yH);
	i2c_write(yL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: affiche un image
//-------------------------------------------------------------------------
void iG(int img)
{
	for(index=0; index<3; index++){	i2c_write(ig_tab[index]);}
	i2c_write(img);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis la largeur du champ tactile depuis le point de départ
//-------------------------------------------------------------------------
void iTW(int yH, int yL)
{
	for(index=0; index<3; index++){	i2c_write(itw_tab[index]);}
	i2c_write(yH);
	i2c_write(yL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: définis la longueur du champ tactile depuis le point de départ
//-------------------------------------------------------------------------
void iTH(int xH, int xL)
{
	for(index=0; index<3; index++){	i2c_write(ith_tab[index]);}
	i2c_write(xH);
	i2c_write(xL);
}



//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: défini le nom du champ tactile
//-------------------------------------------------------------------------
void iTA(int nom)
{
	for(index=0; index<4; index++){	i2c_write(ita_tab[index]);}
	i2c_write(nom);
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: dit avec quelle écriture on écrit le futur texte
//-------------------------------------------------------------------------
void iAF(int ChoixEcritH, int ChoixEcritL)
{
	for(index=0; index<3; index++){	i2c_write(iaf_tab[index]);}
	i2c_write(ChoixEcritH);
	i2c_write(ChoixEcritL);
}


//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: écris un texte
//-------------------------------------------------------------------------
void iDT(int phrase[])
{
	for(index=0; index<3; index++){	i2c_write(idt_tab[index]);}
	
	index = 0;
	while(phrase[index]!=0){
		i2c_write(phrase[index]);			//envoier un à un les caractères du char
		index++;
	}
	
	i2c_write(0x00);
}
/*==========================================================================
====Fonction     : writeLine		                                    ====
====Sierro Romain: 13.05.2016                                           ====
====--------------------------------------------------------------------====
====Affiche une ligne sur l'écran			 					 		====
============================================================================*/
void writeLine(int end_xh,int end_xl, int end_yh, int end_yl)
{
	i2c_write(0xAA);	//Commande de l'écran
	i2c_write(0x44);	//Commande pour dessiner
	i2c_write(0x4C);	//Commande pour dessiner un trait
	i2c_write(end_xh);	//Comme l'écran est grand, sa taille se donne sur 
	i2c_write(end_xl);	//2 Byte. C'est pour ça que les coordonnées sont en 2 fois
	i2c_write(end_yh);	//La ligne part de la position du curseur jusqu'à la position 
	i2c_write(end_yl);	//entrée dans la fonction
}
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: dis que le uP veut commencer à lire des données
//-------------------------------------------------------------------------
void startReading(void)
{
	int ack;
	i2c_start();
	
	do{ ack = i2c_write(I2CADDRESS);} while(ack == 1);
	do{ ack = i2c_write(0x04);}		  while(ack == 1);	// dis qu'on veut "Read Block"
	//-------------
	i2c_start();
	do{ ack = i2c_write(I2CADDRESS+1);}  while(ack == 1);
}
void iDe(int end_xh,int end_xl, int end_yh, int end_yl)
{
	for(index=0; index<3; index++){	i2c_write(iDe_tab[index]);}
	i2c_write(end_xh);	//Comme l'écran est grand, sa taille se donne sur 
	i2c_write(end_xl);	//2 Byte. C'est pour ça que les coordonnées sont en 2 fois
	i2c_write(end_yh);	//La ligne part de la position du curseur jusqu'à la position 
	i2c_write(end_yl);	//entrée dans la fonction
}
void iDC(int RadiusHigh,int RadiusLow)
{
	for(index=0; index<3; index++){	i2c_write(iDC_tab[index]);}
	i2c_write(RadiusHigh);
	i2c_write(RadiusLow);
}
//----------------------------------------------------------------------------
//============================================================================
//-------------------------------------------------------------------------
//	Auteur:	Berguerand Marc
//	Date:	07.02.2014
//-------------------------------------------------------------------------
//  Description: lis les données de l'écran tactile, les stocks dans un tableau
//-------------------------------------------------------------------------


void BlockRead(void)
{
	startReading();
	//récupère les valeurs de l'écran tactile
	for(index=0; index<4; index++){ DataRecu[index] = i2c_read();}

	DataRecu[4] = i2c_read(0);
	i2c_stop();
}