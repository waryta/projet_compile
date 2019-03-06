#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <fcntl.h>
#include "automate.h"


void aff_auto(AFN A)
{
	int i=0,j=0,k=0,t;
	printf("\n affichage des états \n");
	for(i=0;i<A.taille[0];i++)
	{
		printf(" \t l'état n° %d est: %d \n",i,A.Q[i]);
	}
	printf("\n affichage de l'aphabet \n");
	for(j=0;j<A.taille[1];j++)
	{
		printf("\t le caractère %d de l'alphabet est: %c \n",j,A.P[j]);
	}
	printf("\n affichage de l'état initial \n");
	printf("\t l'état initial est: %d \n",A.s);

	printf("\n affichage des états accepteurs \n");
    for(k=0;k<A.taille[2];k++)
	{
		printf("\t l'etat accepteur  n° %d est: %d \n",k,A.F[k]);
	}
	printf("\n affichages des transitions \n");
	for(t=0;t<A.taille[3];t++)
	{
		printf("\t la transition n° %d est :(%d, %c, %d) \n",t,A.tab_transi[t].etat_in,A.tab_transi[t].cons,A.tab_transi[t].etat_fin);
	}
	printf("\n");
	printf("************fin de l'automate **************************************\n");

	printf("\n");
	

}
//automate reconnaissnat le language vide
AFN empty_langage()
{
	printf("*************automate reconnaissnat le language vide ***************************\n ");
	AFN A;
	A.taille[0]=1;//L'Automate a un seul etat
	A.taille[1]=0;
	A.taille[2]=0;
	A.taille[3]=0;
	A.Q=calloc(A.taille[0],sizeof(int));
	A.Q[0]=0;
	A.P=NULL;
	A.s=A.Q[0];
	A.F=NULL;
	A.tab_transi=NULL;

	return A ;
	
	
}

//automate reconnaissant le language composé du seul mot vide
AFN mot_vide()
{
	
	printf("\n **********automate reconnaissant le language composé du seul mot vide********** \n");
	AFN A;
	A.taille[0]=1;
	A.taille[1]=0;
	A.taille[2]=1;
	A.taille[3]=0;
	A.Q=calloc(A.taille[0],sizeof(int));//permet d'initialiser à 0 les cases du tableau
	A.Q[0]=0;
	A.P=NULL;
	A.s=A.Q[0];
	A.F=calloc(A.taille[2],sizeof(int));
	A.F[0]=A.Q[0];
	A.tab_transi=NULL;

	return A;
}

AFN one_char(char word)
{
	printf("\n **********automate reconnaissant le language composé du seul mot********* \n");

	AFN A;
	
	printf("Mot donne en parametre : %c \n", word);
	A.taille[0]= 2;
	A.taille[1]=1;
	A.taille[2]=1;
	A.taille[3]=1;
	A.Q = malloc(2*sizeof(int));
	A.Q[0] = A.s = 0;
	A.Q[1] = 1;
	A.P = malloc(A.taille[1]*sizeof(char));
	A.P[0] = word;
	A.F = malloc(A.taille[2]*sizeof(int));
	A.F[0] = A.Q[1];
	A.tab_transi = malloc(A.taille[3]*sizeof(transition));
	A.tab_transi[0].etat_in = A.Q[0];
	A.tab_transi[0].cons = word;
	A.tab_transi[0].etat_fin = A.Q[1];
	
	return A ;
	
}

AFN Kleene(AFN A)
{
		printf("\n **********automate faisant la mise à l'étoile ************ \n");

	AFN Kleene_A;
	Kleene_A.taille[0]=A.taille[0];
	Kleene_A.taille[1]=A.taille[1];
	Kleene_A.P=malloc(Kleene_A.taille[1]*sizeof(char));
	int check=0;
	int j=0,i=0,k=0;
	for(i=0;i<Kleene_A.taille[1];i++)
	{
		Kleene_A.P[i]=A.P[i];
	}
	//etats accepteurs
	Kleene_A.F=calloc(Kleene_A.taille[2],sizeof(int));
	
	//verifions si l'etat initial est deja accepteur ou pas
	for(j=0;j<A.taille[2];j++)
	{
		if(A.s==A.F[j])
		{
			check+=1;
		}
	}
	if(check!=0)
	{

		Kleene_A.taille[2]=A.taille[2];//car l'etat initial n'est pas   accepteur

		for(i=0;i<Kleene_A.taille[2];i++)
			{				
				Kleene_A.F[i]=A.F[i];
			}
	}
	else
	{

		Kleene_A.taille[2]=A.taille[2]+1;//car l'etat initial devient accepteur
		Kleene_A.F[0]=A.F[0];
		for(i=1;i<Kleene_A.taille[2];i++)
		{
			Kleene_A.F[i]=A.F[i];
		}
	}
	Kleene_A.taille[3]=A.taille[3];


	//remplissage du tab des etats 
	Kleene_A.Q=calloc(A.taille[0],sizeof(int));
	for(i=0;i<Kleene_A.taille[0];i++)
	{
		Kleene_A.Q[i]=A.Q[i];
	}
	//etat initial
	Kleene_A.s=A.s;


	//LES TRANSITIONS
	if(check==0)
	{
		int trans_ajout=0;
		Kleene_A.tab_transi=malloc(A.taille[3]*sizeof(transition));
		
			int cpt=0;//permet de compter le nombre de transition qui partent de l'état initial

			for(i=0;i<A.taille[3];i++)//gestion des trasition qui reboucle sur elle meme
			{
				
						if(A.tab_transi[i].etat_in==A.s)
						cpt+=1;
					
			}
			Kleene_A.tab_transi=calloc((Kleene_A.taille[3]+A.taille[2]*cpt),sizeof(transition));
			Kleene_A.taille[3]=A.taille[3]+A.taille[2]*cpt;


			for(i=0;i<A.taille[3];i++)
				{
					Kleene_A.tab_transi[i]=A.tab_transi[i];
				}
				int var=A.taille[3];
			for(j=0;j<A.taille[2];j++)
				{
					for(i=0;i<cpt;i++)
					{
						
							Kleene_A.tab_transi[var].etat_in=A.F[j];
							Kleene_A.tab_transi[var].cons=A.tab_transi[i].cons;
							Kleene_A.tab_transi[var].etat_fin=A.tab_transi[i].etat_fin;
						
							var+=1;
						
					}
				}
	}
	else
	{
		int trans_ajout=0;
		Kleene_A.tab_transi=malloc(A.taille[3]*sizeof(transition));

		
			int cpt=0;//permet de compter le nombre de transition qui partent de l'état initial

			for(i=0;i<A.taille[3];i++)//gestion des trasition qui reboucle sur elle meme
			{
				
						if(A.tab_transi[i].etat_in==A.s)
						cpt+=1;
					
			}
			Kleene_A.tab_transi=calloc((Kleene_A.taille[3]+A.taille[2]*cpt),sizeof(transition));
			Kleene_A.taille[3]=A.taille[3]+(A.taille[2]-1)*cpt;


			for(i=0;i<A.taille[3];i++)
				{
					Kleene_A.tab_transi[i]=A.tab_transi[i];
				}
				int var=A.taille[3];
			for(j=1;j<A.taille[2];j++)
				{
					for(i=0;i<cpt;i++)
					{
						
							Kleene_A.tab_transi[var].etat_in=A.F[j];
							Kleene_A.tab_transi[var].cons=A.tab_transi[i].cons;
							Kleene_A.tab_transi[var].etat_fin=A.tab_transi[i].etat_fin;
						
							var+=1;
						
					}
				}
	}
	return Kleene_A;
}

AFN reunion(AFN afn1,AFN afn2)
{
	printf("********automate reconnaissant la reunion de deux langages ************** \n");
	int i=0,j=0,k=1,q=0,n=0,o=0;
	int t=0,p=0;
	int verif=0,v=0,v1=0,cpt_A=0,cpt_afn1=0,cpt_afn2=0;
	int nb_acp_afn1=0;
	AFN A;
	A.taille[0]=afn1.taille[0]+afn2.taille[0]-1;
	A.taille[1]=afn1.taille[1];
	A.P=malloc(A.taille[1]*sizeof(char));
	for(t=0;t<A.taille[1];t++)
	{
		A.P[t]=afn1.P[t];
	}
	for(i=0;i<afn1.taille[0];i++)
	{
		if(afn1.Q[0]==afn1.F[i])
		v+=1;
	}

	for(j=0;j<afn2.taille[0];j++)
	{
		if(afn2.Q[0]==afn2.F[j])
		v1+=1;
	}

	if((v!=0) && (v1!=0))
	{
		A.taille[2]=afn1.taille[2]+afn2.taille[2];//si les deux deux états initiaux sont accepteurs on remplace ces deux etats par un seul nouvel etats accepteur d'ou le -1
	}
	else
	{
		A.taille[2]=afn1.taille[2]+afn2.taille[2];
	}
	A.taille[3]=afn1.taille[3]+afn2.taille[3];//le nombre de transition est la somme - le nombre de transition supprimé de chaque automate + ce meme nombre partant du nouvel état

	A.Q=calloc(A.taille[0],sizeof(int));//permet d'initialiser à 0 les cases du tableau

	A.Q[0]=0;
	for(o=1;o<afn1.taille[0];o++)
	{
		A.Q[o]=afn1.Q[o];
	}
	for(o=afn1.taille[0];o<A.taille[0];o++)
	{
		
		A.Q[o]=afn2.Q[o-afn1.taille[0]+1];
	}
	
	A.s=0;
	
	A.F = calloc(A.taille[2],sizeof(int));//tableau de transition de taille dependant de l'appartenence ou pas des etats initiaux aux tableaux des états finaux
	if((v!=0) && (v1!=0))
	{

		for(j=0;j<afn1.taille[2];j++)
		{
			A.F[j]=afn1.F[j];
		}
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[j]=afn2.F[j-afn1.taille[2]];
		}
	 }
	else if(v!=0)
	{
		A.F[0]=0;
		for(j=1;j<afn1.taille[2];j++)
		{
			A.F[j]=afn1.F[j];
		}
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[j]=afn2.F[j-afn1.taille[2]];
		}
	 }
	else if(v1!=0)
	{
		A.F[0]=0;
		for(j=0;j<afn1.taille[2];j++)
		{
			A.F[j+1]=afn1.F[j];
		}
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[j+1]=afn2.F[j-afn1.taille[2]];
		}
	 }
	
	
	else
	{
		for(j=0;j<afn1.taille[2];j++)
		{
			A.F[j]=afn1.F[j];
			nb_acp_afn1+=1;
		}
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[nb_acp_afn1]=afn2.F[j-nb_acp_afn1];
		}
		
	}
	A.tab_transi=calloc(afn1.taille[3]+afn2.taille[3],sizeof(transition));

	for(t=0;t<afn1.taille[3];t++)
	{
		
		if(afn1.tab_transi[cpt_afn1].etat_in==afn1.Q[0])
		{
			A.tab_transi[t].etat_in=0;
			A.tab_transi[t].cons=afn1.tab_transi[cpt_afn1].cons;
			A.tab_transi[t].etat_fin=afn1.tab_transi[cpt_afn1].etat_fin;
			cpt_A+=1;
			cpt_afn1+=1;
		}
	}
	
	for(q=0;q<afn2.taille[3];q++)
	{
		if(afn2.tab_transi[cpt_afn2].etat_in==afn2.Q[0])
			{
				A.tab_transi[cpt_A].etat_in=0;
				A.tab_transi[cpt_A].cons=afn2.tab_transi[cpt_afn2].cons;
				A.tab_transi[cpt_A].etat_fin=afn2.tab_transi[cpt_afn2].etat_fin;
				cpt_afn2+=1;
				cpt_A+=1;
			}
	}

	for(t=cpt_afn1;t<afn1.taille[3];t++)
		{
			A.tab_transi[cpt_A]=afn1.tab_transi[cpt_afn1];
			cpt_afn1+=1;
			cpt_A+=1;

	    }

	for(t=cpt_afn2;t<A.taille[3];t++)
		{
			A.tab_transi[cpt_A]=afn2.tab_transi[cpt_afn2];
			cpt_afn2+=1;
			cpt_A+=1;
	    }
	      
	return A;
}



AFN concat(AFN A,AFN B)
{   
	printf("********automate reconnaissant la concatenation de deux langages ************** \n");
		AFN C ; // Automate reconnaissant la concatenation de A et B

	int i=0, j =0, val =0 , deb=0,cpt=0,check=0; 
	//etat accepteur
	C.s=A.s;
	//l'alphabet
	C.taille[1]=A.taille[1];
	C.P=malloc(C.taille[1]*sizeof(char));
	int *new_tab=malloc(C.taille[1]*sizeof(char));
	for(i=0 ;i< A.taille[1];i++)
	{ 
		C.P[i] =A.P[i];
	}
	
				
	//les etats
	
	// Changement des numéros d'états de l'automate B
	B.s += A.taille[0]-1;
	for(i=0 ; i< B.taille[0];i++)
	{ 
		B.Q[i] +=A.taille[0]-1;
	}

    for(i=0 ; i< B.taille[2];i++)
	{ 
		B.F[i] +=A.taille[0]-1;
	}

	for(i=0; i< B.taille[3];i ++)
	{

		 B.tab_transi[i].etat_in  += A.taille[0]-1;
		 B.tab_transi[i].etat_fin += A.taille[0]-1;

	}
	C.taille[0]=A.taille[0]+B.taille[0]-1;
	C.Q=malloc(C.taille[0]*sizeof(int));
	for(i=0 ; i< A.taille[0];i++)
	{ 
		C.Q[i] =A.Q[i];
	}
	for(i=A.taille[0]; i< C.taille[0];i++)
	{ 
		C.Q[i] =B.Q[i-A.taille[0]+1];
	}

	//les états accepteurs
	for(i=0; i< B.taille[3];i ++)
	{

		 if(B.s==B.F[i])
		 check+=1;

	}
	
	if(check!=0)
	{
		C.taille[2]=A.taille[2]+B.taille[2]-1;
		C.F=malloc(C.taille[2]*sizeof(int));
		for(i=0;i<A.taille[2];i++)
		{
			C.F[i]=A.F[i];
		}
		for(i=A.taille[2];i<C.taille[2];i++)
		{
			C.F[i]=B.F[i+1-A.taille[2]];
		}
	}
	else
	{
		C.taille[2]=B.taille[2];
		C.F=malloc(C.taille[2]*sizeof(int));
		for(i=0;i<C.taille[2];i++)
		{
			C.F[i]=B.F[i];
		}
	}
	//gestion des transitions
	C.tab_transi=malloc(C.taille[3]*sizeof(transition));
    for(i=0;i<A.taille[3];i++)
		{
			C.tab_transi[i]=A.tab_transi[i];
		}
	
	C.tab_transi[A.taille[3]].etat_in=A.tab_transi[A.taille[3]-1].etat_fin;
	C.tab_transi[A.taille[3]].cons=B.tab_transi[0].cons;
	C.tab_transi[A.taille[3]].etat_fin=B.tab_transi[0].etat_fin;

	int u=1;
    for(i=A.taille[3]+1;i<C.taille[3];i++)
		{
			C.tab_transi[i]=B.tab_transi[u];
			u+=1;
		}

	return C;

}



//~ AFN determinisation(AFN A)
//~ {
	//~ printf("determinisation \n");

	//~ int i=0,j=0,k=0;
	//~ int cpt=0,t=0;
	//~ int taille=1;
	//~ int **tab_des_etats=malloc(taille*sizeof(int*));
	//~ tab_des_etats[0]=malloc(1*sizeof(int));
	//~ tab_des_etats[0]=0;
	
	
	//~ for(j=0;j<A.taille[0];j++)
	//~ {
			//~ for(i=0;i<A.taille[3];i++)
			//~ {
				//~ for(k=0;k<A.taille[1];k++)
					//~ {
						//~ if((A.tab_transi[i].etat_in==A.Q[j]) && (A.tab_transi[i].cons==A.P[k]))
						//~ {
							//~ cpt+=1;
							//~ printf("par état %d en consommant %c on arrive en %d _n",j,A.P[k],A.tab_transi[i].etat_fin);
							//~ taille+=1;
							//~ tab_des_etats=realloc(taille,sizeof(int*));

						//~ }
						//~ if((A.tab_transi[i].etat_in==A.Q[j]) && (A.tab_transi[i].cons==A.P[k]))
						//~ {
							//~ tab_des_etats[i]=malloc(cpt*sizeof(int));
							//~ tab_des_etats[i][=malloc(cpt*sizeof(int));
							
						//~ }
					//~ }
			//~ }
	//~ }
					
	//~ }
	//~ return Det;
//~ }


int f_transi(int etat,char c, AFD D)
{
	int etat_f = -1;
	int i =0;
	
	for(i=0;i<D.taille[3];i++)
	{
	  if((D.tab_transiD[i].etat_in == etat)&&(D.tab_transiD[i].cons == c))
	  {
		  etat_f = D.tab_transiD[i].etat_fin;
		  printf("\n en partant de %d en lisant %c on arrive à %d (%d,%c,%d) ",etat,c,etat_f,etat,c,etat_f);
	  }	
	}
	
	return etat_f;
}

AFD create_AFD()
{
	//int i =0;
	AFD A;
	A.taille[0]=4;
	A.taille[1]=3;
	A.taille[2]=2;
	A.taille[3]=7;
	A.QD=calloc(A.taille[0],sizeof(int));//permet d'initialiser à 0 les cases du tableau
	A.QD[0]=0;
	A.QD[1]=1;
	A.QD[2]=2;
	A.QD[3]=3;
	A.PD=calloc(A.taille[1],sizeof(char));
	A.PD[0] = 'a';
	A.PD[1] = 'b';
	A.PD[2] = 'c';
	A.sD=A.QD[0];
	A.FD=calloc(A.taille[2],sizeof(int));
	A.FD[0]=A.QD[2];
	A.FD[1]=A.QD[3];
	A.tab_transiD = calloc(A.taille[3],sizeof(transition));
	
	A.tab_transiD[0].etat_in = A.QD[0];
	A.tab_transiD[0].cons = A.PD[0];
	A.tab_transiD[0].etat_fin = A.QD[1];
	
	A.tab_transiD[1].etat_in = A.QD[0];
	A.tab_transiD[1].cons = A.PD[1];
	A.tab_transiD[1].etat_fin=A.QD[2];
	
	A.tab_transiD[2].etat_in = A.QD[1];
	A.tab_transiD[2].cons = A.PD[0];
	A.tab_transiD[2].etat_fin=A.QD[2];
	
	A.tab_transiD[3].etat_in = A.QD[2];
	A.tab_transiD[3].cons = A.PD[0];
	A.tab_transiD[3].etat_fin=A.QD[1];
	
	A.tab_transiD[4].etat_in = A.QD[2];
	A.tab_transiD[4].cons = A.PD[2];
	A.tab_transiD[4].etat_fin=A.QD[3];
	
	A.tab_transiD[5].etat_in = A.QD[3];
	A.tab_transiD[5].cons = A.PD[0];
	A.tab_transiD[5].etat_fin=A.QD[1];
	
	A.tab_transiD[6].etat_in = A.QD[2];
	A.tab_transiD[6].cons = A.PD[1];
	A.tab_transiD[6].etat_fin=A.QD[0];
	
    int (*pointeur_f_transi)(int ,char, AFD );
    
    pointeur_f_transi = f_transi;
    
       
	return A;

}

int in_AFD(AFD D, char  * word,int t)
{
	int s = D.sD ;
	int i =0;
	int etat =0;
	D.pointeur_f_transi = f_transi;
	/*int essai = (*D.pointeur_f_transi)(0,'a',G);
	AFD G = D ;*/
	
	for(i=0;i<t;i++)
	{	
	  etat = (*D.pointeur_f_transi)(s,word[i],D);
	  s = etat ;	
	}
	int verite=0;
	for(i=0;i<D.taille[2];i++)
	{
		if(s==D.FD[i])
		{
			verite=1;
		}
	}
	
	
	if(verite==0)
		{
			printf("oulaaaaaaa mot refusé car l'etat %d n'est pas accepteur \n",s);
		}
	else
	{
		printf("mot accepté \n");
	}
	return s;
	
}



int main(int argc,char** argv)
{
	
	char a='a';
	char b='b';
	char c='c';
	AFN afn1,afn2,afn3,afn4;
	afn1.taille[0]=3;
	afn1.taille[1]=3;
	afn1.taille[2]=1;
	afn1.taille[3]=3;
	afn1.Q=calloc(afn1.taille[0],sizeof(int));
	afn1.Q[0]=0;
	afn1.Q[1]=1;
	afn1.Q[2]=2;
	afn1.P=malloc(afn1.taille[1]*sizeof(char));
	afn1.P[0]=a;
	afn1.P[1]=b;
	afn1.P[2]=c;
	afn1.s=afn1.Q[0];
	afn1.F=calloc(afn1.taille[2],sizeof(int));
	afn1.F[0]=2;
	
	
	
	
	afn1.tab_transi=calloc(afn1.taille[3],sizeof(transition));
	afn1.tab_transi[0].etat_in=0;
	afn1.tab_transi[0].cons=afn1.P[0];
	afn1.tab_transi[0].etat_fin=1;
	
	afn1.tab_transi[1].etat_in=1;
	afn1.tab_transi[1].cons=afn1.P[1];
	afn1.tab_transi[1].etat_fin=2;
	
	afn1.tab_transi[2].etat_in=2;
	afn1.tab_transi[2].cons=afn1.P[2];
	afn1.tab_transi[2].etat_fin=2;
	
	
	
	
	
	afn2.taille[0]=2;
	afn2.taille[1]=3;
	afn2.taille[2]=1;
	afn2.taille[3]=1;
	afn2.Q=calloc(2,sizeof(int));
	afn2.Q[0]=0;
	afn2.Q[1]=1;
	afn2.s=afn2.Q[0];
	afn2.P=malloc(afn2.taille[1]*sizeof(char));
	afn2.P[0]=a;
	afn2.P[1]=b;
	afn2.P[2]=c;
	afn2.F=calloc(afn2.taille[2],sizeof(int));
	afn2.F[0]=afn2.Q[0];
	
	afn2.tab_transi=calloc(afn2.taille[3],sizeof(transition));
	
	afn2.tab_transi[0].etat_in=0;
	afn2.tab_transi[0].cons=afn2.P[1];
	afn2.tab_transi[0].etat_fin=1;
	
	
	//aff_auto(afn1);
	//aff_auto(afn2);
	afn3.taille[0]=3;
	afn3.taille[1]=3;
	afn3.taille[2]=1;
	afn3.taille[3]=2;
	afn3.Q=calloc(afn1.taille[0],sizeof(int));
	afn3.Q[0]=0;
	afn3.Q[1]=1;
	afn3.Q[2]=2;
	afn3.P=malloc(afn3.taille[1]*sizeof(char));
	afn3.P[0]=a;
	afn3.P[1]=b;
	afn3.P[2]=c;
	afn3.s=afn3.Q[0];
	afn3.F=calloc(afn3.taille[2],sizeof(int));
	afn3.F[0]=2;
	
	
	afn3.tab_transi=calloc(afn3.taille[3],sizeof(transition));
	afn3.tab_transi[0].etat_in=0;
	afn3.tab_transi[0].cons=afn3.P[0];
	afn3.tab_transi[0].etat_fin=1;
	
	afn3.tab_transi[1].etat_in=1;
	afn3.tab_transi[1].cons=afn3.P[1];
	afn3.tab_transi[1].etat_fin=2;
	
	
	afn4.taille[0]=2;
	afn4.taille[1]=3;
	afn4.taille[2]=1;
	afn4.taille[3]=1;
	afn4.Q=calloc(afn4.taille[0],sizeof(int));
	afn4.Q[0]=0;
	afn4.Q[1]=4;
	afn4.P=malloc(afn4.taille[1]*sizeof(char));
	afn4.P[0]=a;
	afn4.P[1]=b;
	afn4.P[2]=c;
	afn4.s=afn3.Q[0];
	afn4.F=calloc(afn4.taille[2],sizeof(int));
	afn4.F[0]=4;
	afn4.tab_transi=calloc(afn4.taille[3],sizeof(transition));
	afn4.tab_transi[0].etat_in=0;
	afn4.tab_transi[0].cons=afn4.P[0];
	afn4.tab_transi[0].etat_fin=4;
	AFN E = empty_langage();
	//aff_auto(E);
	AFN M = mot_vide();
	//aff_auto(M);
	
	AFN O = one_char(a);
	//aff_auto(O);
	
	AFN k=Kleene(afn1);
	//aff_auto(k);
	
	AFN R=reunion(afn1,afn2);
	//aff_auto(R);
	
	AFN C=concat(afn1,afn2);
	aff_auto(C);
	
	//AFN d=determinisation(afn1);
	//aff_auto(d);
	
	
	
	 AFD G = create_AFD();
	 char *mot ="oula";
	 int res = in_AFD( G,mot,strlen(mot));
	 printf(" \n res = %d \n " , res);
	
	 
	 
	 AFD D = create_AFD();	 
	 D.pointeur_f_transi = f_transi;
	 int essai = (*D.pointeur_f_transi)(0,'a',D);
    
    printf("\n  essai = %d", essai); 
	
	AFN MOV = mot_vide();
	AFN ENSV = empty_langage();
	AFN UNCHAR = one_char('c');
	//printf("Affichage de l'automate  ENSV reconnaisant le langage vide");
	//printf("****************************************************\n");
	//aff_auto(MOV);
	
	//printf("Affichage de l'automate  UNCHAR reconnaisant un caractère");
	//printf("****************************************************\n");
	//aff_auto(UNCHAR);
	
	//concat(ENSV,UNCHAR);
	//AFN U = concat(UNCHAR,MOV);
	
	//~ printf(" \n Affichage de l'automate Qui concatène UNCHAR et MOV ");
	//~ printf("****************************************************\n");
	//aff_auto(U);
	
	return 0;
}

