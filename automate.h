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
	int * Q;//ensemble des etats oo
	char * P;//alphabet
	int s;//etat initial
	int * F;//l'ensemble des états accepteurs
	transition *tab_transi;// tableau contenant l'ensemble des transitions
	int taille[4];

};
typedef struct AFN AFN;

 struct AFD
{
	int *QD; // ensemble des etats
	char *PD; // l'alphabet
	int sD ;  // etat initial
	int *FD; // ensemble des états accepteurs
	transition * tab_transiD; //tableau contenant l'ensemble des transitions
	//int *tab_f_transi;// tableau à 2 dimensions contenant tous les retours possibles
	// de la foncttion f_transi pour l'automate en question 
	int taille[4];
	int (*pointeur_f_transi)(int ,char, struct AFD );
	
	
};

typedef struct AFD AFD;


AFN empty_langage();
AFN one_char(char word);
AFN Kleene(AFN A);
AFN determinisation(AFN A);

AFN concat(AFN A,AFN B);
AFN mot_vide();
int f_transi(int etat,char c, AFD D);
AFD create_AFD();
int in_AFD(AFD D, char * word, int t);
