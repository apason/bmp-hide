/*
 * files.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bmp_hide.h"

// lasketaan kuvatiedoston pikselien koko tavuina
size_t file_size(FILE *fp){

	long length;
	// siirretään tiedosto-osoitin kuvan loppuun
	fseek(fp,0,SEEK_END);
	// tallennetaan kuvan pituus (tiedoston koko - pikselien datan alkupaikka)
	length=ftell(fp)-read_pixel_offset(fp);
	// palautetaan koko
	return (size_t)length;
}

// luetaan viesti yhdeksi merkkijonoksi joko stdinistä tai erikseen
// määritellystä tiedostosta
void read_user_message(OPTIONS *options){

	// puskurin koko määritellään bmp_hide.h tiedostossa
	char buffer[BUFFERSIZE];
	// stdin tai optiolla -m määritelty tiedosto
	FILE *message;

	// tulostetaan ohjeteksti ja määritellään luettavaksi tiedostoksi
	// stdin jos luetaan stdinistä
	if(options->message_file==NULL){
		printf("Type the message you want to hide. (Ctrl+d on new line ends input.)\n");
		message=stdin;
	}
	// muuten asetetaan paramerin -m määrittelemä tiedosto viestin lähteeksi
	else
		message=options->message_file;

	// luetaan tiedostoa merkkijonoksi, kunnes tiedosto päättyy
	while(!feof(message)){
		fgets(buffer,BUFFERSIZE,message);
		options->message=(char*)realloc(options->message,(strlen(options->message)+strlen(buffer)+1)*sizeof(char));
		strncat(options->message,buffer,BUFFERSIZE);
		memset(buffer,'\0',1);
	}
}

// luetaan pixel offset  (bmp tiedostoissa tavut [10,13] eli [0xA,0xD])
uint32_t read_pixel_offset(FILE *fp){

	// siirrytään kohtaan 10,
	fseek(fp,PIXEL_OFFSET,SEEK_SET);

	// luetaan 4 tavun luku,
	long po = read_word(fp);

	// ja palautetaan se
	return po;
}
