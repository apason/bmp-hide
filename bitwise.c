/*
 * bitwise.c
 */

#include <stdint.h>
#include <stdio.h>

#include "bmp_hide.h"

//---------------------------------
//lukufunktiot
//--------------------------------

// luetaan parametrina saadun tavun vähiten merkitsevä bitti
uint8_t read_lsb(uint8_t byte){

	int mask=1;

	return byte&mask;
}

//luetaan fp:n osoittamasta tiedostosta yksi tavu
uint8_t read_byte(FILE *fp){

	uint8_t byte=0;

	if(fp==NULL)
		return byte;

	fread(&byte,1,1,fp);

	return byte;
}

// luetaan 8 tavua ja koostetaan jokaisen tavun vähiten merkitsevistä biteistä yksi tavu
char read_char(FILE *fp){

	int i;
	uint8_t byte=0;

	for(i=0;i<8;i++)
		byte=byte|(read_lsb(read_byte(fp))<<i);

	return (char)byte;
}

// luetaan ja palautetaan 4 tavun kokonaisluku little endian järjestelmässä
uint32_t read_word(FILE *fp){

	uint32_t word=0;
	int i;

	for(i=0;i<4;i++)
		word=word|((uint32_t)read_byte(fp)>>8*i);

	return word;
}

// ----------------------------------------------------------
// kirjoitusfunktiot
// ----------------------------------------------------------

// kirjoitetaan yks merkki tiedosto-osoittimesta fp 8 tavun
// sekvenssille jokaisen tavun least significant bittiin
int write_char(FILE *fp, char c){

	int i;
	uint8_t mask=1;

	for(i=0;i<8;i++)
		write_bit(fp,(((uint8_t)c>>i)&mask));

	return 0;
}

// kirjoitetaan (vaihdetaan) tiedosto-osoittimen määräävän tavun viimeisin bitti
int write_bit(FILE *fp, uint8_t bit){

	uint8_t byte;
	uint8_t mask=254;

	fread(&byte,1,1,fp);
	fseek(fp,-1,SEEK_CUR);

	byte=(byte&mask)|bit;

	fwrite(&byte,1,1,fp);

	return 0;
}

