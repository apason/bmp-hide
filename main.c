/*
 * main.c
 */

#include "bmp_hide.h"

int main(int argc,char *argv[]){

	// pointteri asetuksille
	OPTIONS *options;
	// asetusten oikeellisuuden tarkastamiseen käytettävä muuttuja
	int integrity;

	// jos komentoriviparametreja ei saada niin suoritetaan ohjelma
	// interaktiivisesti, muuten aseukset luetaan parametreista
	if(argc>1)
		options=check_options(argc,argv);
	else
		options=interactive_options();

	// tarkastetaan asetusten oikeellisuu
	integrity=check_options_integrity(options);

	// jos virheitä niin keskeytetään ohjelma ja vapautetaan muisti
	if(integrity!=0&&integrity!=1){
		free_options(options);
		return integrity;
	}
	// tässä tapauksessa muisti on jo vapautettu
	if(integrity!=0)
		return integrity;

	// luetaan tai kirjoitetaan .bmp tiedostoon riippuen asetuksista
	if(options->direction==READ)
		read_message(options);
	else
		if(write_message(options)==5)
			return 5;

	// vapautetaan muisti ja suljetaan tiedostot
	free_options(options);

	return 0;
}
