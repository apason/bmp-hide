/*
 * options.c
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_hide.h"

// luetaan komentoriviparametrit OPTIONS tyyppiseen tietueeseen
// käyttäen järjestelmäfunktiota getopt()
OPTIONS *check_options(int argc,char *argv[]){

	// muuttuja yksikirjaimiselle komentoriviparametrille
	int optch;
	// getopt():lle annettava merkkijono, mikä kuvaa
	// hyväksytyt komentoriviparametrit
	static char optstring[]="hrwf:m:";

	// pointteri asetustietueelle
	OPTIONS *options;

	// varataan tilaa kyseiselle tietueelle
	options=(OPTIONS*)malloc(sizeof(OPTIONS));
	// annetaan asetuksille arvot jotka kuvaavat
	// määrittelemättömiä asetuksia
	reset_options(options);

	// luetaan komentoriviparametrit ja tehdään joitakin virhetarkasteluja
	while((optch=getopt(argc,argv,optstring))!=-1)
		switch (optch){
			case 'h':
				print_help();
				exit(0);
				break;
			case 'r':
				if(options->direction==WRITE){
					printf("Reading and writing cannot be done simultaneously!\nExiting...\n");
					free_options(options);
					return NULL;
				}
				options->direction=READ;
				break;
			case 'w':
				if(options->direction==READ){
					printf("Reading and writing cannot be done simultaneously!\nExiting...\n");
					free_options(options);
					return NULL;
				}
				options->direction=WRITE;
				break;
			case 'f':
				if((options->picture=fopen(optarg,"r+b"))==NULL){
					printf("Error: File %s cannot be opened!\nExiting...\n",optarg);
					free_options(options);
					return NULL;
				}
				break;
			case 'm':
				if((options->message_file=fopen(optarg,"r"))==NULL){
					printf("Error: File %s cannot be opened!\nExiting...\n",optarg);
					free_options(options);
					return NULL;
				}
				break;
			default:
				printf("Ignoring: Unknown option!\n");
			}

	// jos käyttäjä haluaa kirjoittaa viestin, niin..
	if(options->direction==WRITE)
		// ..luetaan viesti
		read_user_message(options);

	// palautetaan asetukset (mainiin)
	return options;
}

// tarkistetaan onko määritellyt asetukset valideja
int check_options_integrity(OPTIONS *options){

	// tässä tapauksessa asetukset on saatu määriteltyä
	// ja tarkastellaan voidaanko ohjelma suorittaa näillä asetuksilla
	if(options!=NULL){
		if(options->picture==NULL){
			printf("Error: Picture file is not specified!\nExiting...\n");
			return 2;
		}
		if(options->direction==READ&&options->message_file!=NULL){
			printf("Error: -m can be used only when writing!\nExiting..\n");
			return 4;
		}
		if(options->direction==UNDEFINED){
				printf("Error: You must use -r or -w!\nExiting...\n");
				return 3;
		}
	}
	// tässä tapauksessa asetuksia ei saatu edes määriteltyä
	if(options==NULL)
		return 1;

	return 0;
}

// suljetaan tiedosto-osoittimet ja vapautetaan dynaamisesti varattu muisti
void free_options(OPTIONS *options){
	if(options->picture!=NULL)
		fclose(options->picture);
	if(options->message_file!=NULL)
		fclose(options->message_file);
	free(options->message);
	free(options);
}

// nollataan asetukset
void reset_options(OPTIONS *options){
	options->direction=UNDEFINED;
	options->picture=NULL;
	options->message_file=NULL;
	options->message=(char*)malloc(sizeof(char));
	strncpy(options->message,"\0",1);
}

// komentoriviparametrejä ei saatu joten luetaan
// ohjelman asetukset interaktiivisesti käyttäjältä
OPTIONS *interactive_options(void){

	OPTIONS *options;
	// kuvan nimelle varataan 128 merkkiä (vaikka kaikki tiedostojärjestelmät
	// eivät edes tue näin pitkiä nimiä. virheentarkastelu jätetään kuitenkin
	// tekemättä. puskurille tilaa bmp_hide.h tiedostossa määritellyn verran
	char c,picname[128],buffer[BUFFERSIZE];

	// varataan muistit ja nollataan asetukset
	options=(OPTIONS*)malloc(sizeof(OPTIONS));
	reset_options(options);

	// luetaan r tai w käyttäjältä
	do{
		printf("Read or Write message from/to bmp-file? (r/w): ");
		fgets(buffer,BUFFERSIZE,stdin);
		sscanf(buffer,"%c",&c);
		if(c=='r')
			options->direction=READ;
		else if(c=='w')
			options->direction=WRITE;
	}while(options->direction==UNDEFINED);

	// luetaan tiedoston nimi (virheentarkastelut yli 128 merkkiä
	// pitkille tiedostonimille jätetään tekemättä.
	printf("Name of the file: ");
	fgets(buffer,BUFFERSIZE,stdin);
	sscanf(buffer,"%s",picname);

	// tarkistetaan jos tiedoston avauksessa ilmenee ongelmia
	if((options->picture=fopen(picname,"r+d"))==NULL){
		printf("Error: file %s cannot be opened!\nExiting...\n",picname);
		return NULL;
	}

	// luetaan viesti, jos käyttäjä valitsi kirjoituksen
	// HUOM. viesti luetaan interaktiivisessa tilassa aina stdinistä
	// eikä erillistä viestitiedostoa voi valita.
	if(options->direction==WRITE)
		read_user_message(options);

	// palautetaan asetukset (mainiin)
	return options;
}
