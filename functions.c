/*
 * functions.c
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "bmp_hide.h"

// tulostetaan käyttäjälle ylimalkainen ohje ohjelman käytöstä
void print_help(void){
	printf("\nUsage: bmp_hide <options>\nAvailable options:\n");
	printf("-r: read message from file\n");
	printf("-w: write message to file\n");
	printf("-f [file] defines the bitmap file to operate\n");
	printf("-m [file] defines user message to write\n");
	printf("-h: help\n");
}

// luetaan viesti asetusten perusteella
void read_message(OPTIONS *options){

	char c;

	// asetetaan tiedosto-osoitin pixeleiden datan alkuun
	fseek(options->picture,read_pixel_offset(options->picture),SEEK_SET);

	printf("\n");

	// luetaan merkki kerrallaan
	do{
		c=read_char(options->picture);
		putchar(c);
	// kunnes vastaan tulee nollatavu
	}while(c!=0);

	printf("\n");

}

// luetaan viesti asetusten perusteella
int write_message(OPTIONS *options){

	int i;
	// lasketaan tiedoston (pikselien) koko muuttujaan size
	size_t size=file_size(options->picture);

	// tarkistetaan että kirjoitettava viesti mahtuu kuvaan
	if(size/8.0<(double)strlen(options->message)){
		printf("Error: Picture file is too small!\nExiting...\n");
		return 5;
	}

	// siirretään tiedosto-osoitin pixelien datan alkuun
	fseek(options->picture,read_pixel_offset(options->picture),SEEK_SET);

	// kirjoitetaan viesti kuvaan
	for(i=0;options->message[i]!='\0';i++)
		write_char(options->picture,options->message[i]);

	// kirjoitetaan vielä kuvan loppuun 0 jotta ohjelma osaa
	// pysähtyä luettaessa oikeaan paikkaan
	write_char(options->picture,'\0');

	return 0;
}
