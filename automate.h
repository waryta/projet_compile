#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

struct transition
{
	int etat_in;
	char cons;
	int etat_fin;
};
typedef struct transition transition;

struct AFN
{
	int * Q;//ensemble des etats
	char * P;//alphabet
	int s;//etat initial
	int * F;//l'ensemble des états accepteurs
	transition *tab_transi;
	int taille[4];

};
typedef struct AFN AFN;


void aff_auto(AFN A);
AFN empty_langage();
AFN mot_vide();
AFN reunion(AFN afn1,AFN afn2);
AFN one_char(char word);
AFN Kleene(AFN A);
AFN concat(AFN A,AFN B);
