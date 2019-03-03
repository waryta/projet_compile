#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include "automate.h"


void aff_auto(AFN A)
{
	int i=0,j=0,k=0,t;
	for(i=0;i<A.taille[0];i++)
	{
		printf("l'état n° %d est: %d \n",i,A.Q[i]);
	}
	for(j=0;i<A.taille[1];j++)
	{
		printf("le caractère %d de l'alphabet est: %d \n",j,A.P[j]);
	}
	printf("l'état initial est: %d \n",A.s);
    for(k=0;k<A.taille[2];k++)
	{
		printf("l'etat accepteur  n° %d est: %d \n",k,A.F[k]);
	}
	for(t=0;t<A.taille[3];t++)
	{
		printf("la transistion n° %d est :(%d, %c, %d) \n",t,A.tab_transi[t].etat_in,A.tab_transi[t].cons,A.tab_transi[t].etat_fin);
	}
}
//automate reconnaissnat le language vide
AFN empty_langage()
{
	AFN A;
	A.taille[0]=1;//L'Automate a un seul etat
	A.taille[1]=0;
	A.taille[2]=0;
	A.taille[3]=0;
	printf("automate reconnaissnat le language vide \n");
	A.Q=calloc(1,sizeof(int));
	A.Q[0]=2;
	A.P=NULL;
	A.s=A.Q[0];
	A.F=NULL;
	A.tab_transi=NULL;
	

	return A ;
	
}

//automate reconnaissant le language composé du seul mot vide
AFN mot_vide()
{
	printf("automate reconnaissant le language composé du seul mot vide \n");
	AFN A;
	A.taille[0]=1;
	A.taille[1]=0;
	A.taille[2]=1;
	A.taille[3]=0;
	A.Q=calloc(1,sizeof(int));//permet d'initialiser à 0 les cases du tableau
	A.Q[0]=5;
	A.P=NULL;
	A.s=A.Q[0];
	A.F=calloc(1,sizeof(int));
	A.F[0]=A.Q[0];
	A.tab_transi=NULL;

	return A;
}

AFN reunion(AFN afn1,AFN afn2)
{
	
	int i=0,j=0,k=1,q=0,n=0,o=0;
	int t=0,p=0;
	int verif=0,v=0,v1=0,cpt_A=0,cpt_afn1=0,cpt_afn2=0;
	int nb_acp_afn1=0;
	printf("automate reconnaissant la reunion de deux languages \n");
	AFN A;
	A.taille[0]=afn1.taille[0]+afn2.taille[0]-3;
	A.taille[1]=2;

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
		A.taille[2]=afn1.taille[2]+afn2.taille[2]-1;//si les deux deux états initiaux sont accepteurs on remplace ces deux etats par un seul nouvel etats accepteur d'ou le -1
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
		A.F[0]=0;
		printf("ici on est dans v  et v1 \n");
		printf(" afn1.taille[2] est %d \n",afn1.taille[2]);

		for(j=1;j<afn1.taille[2];j++)
		{
			A.F[j]=afn1.F[j];
		printf("afn1 dans %d on met %d \n ",j+1,afn1.F[j]);
		}
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[j]=afn2.F[j-afn1.taille[2]+1];
			printf("afn2 dans %d on met %d \n ",j,afn2.F[j-afn2.taille[2]+1]);

		}
	 }
	else if(v!=0)
	{
		printf("ici on est dans v \n");
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
		printf("%d \n ",v1);
				printf("ici on est dans v1 \n");
		for(j=0;j<afn1.taille[2];j++)
		{
			A.F[j+1]=afn1.F[j];
		printf("afn1 dans %d on met %d \n ",j+1,afn1.F[j]);
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
		printf("%d \n ",nb_acp_afn1);
		for(j=afn1.taille[2];j<A.taille[2];j++)
		{
			A.F[nb_acp_afn1]=afn2.F[j-nb_acp_afn1];
		}
		
	}
	A.tab_transi=calloc(afn1.taille[3]+afn2.taille[3],sizeof(transition));
	for(t=0;t<afn1.taille[3];t++)
	{
		
		if(afn1.tab_transi[p].etat_in==afn1.Q[0])
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
		if(afn2.tab_transi[q].etat_in==afn2.Q[0])
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

AFN one_char(char word)
{
	
	AFN A;
	
	printf("Mot donne en parametre : %c \n", word);
	A.taille[0]= 2;
	A.taille[1]=1;
	A.taille[2]=1;
	A.taille[3]=1;
	A.Q = malloc(2*sizeof(int));
	A.Q[0] = A.s = 0;
	A.Q[1] = 1;
	A.P = malloc(1*sizeof(char));
	A.P[0] = word;
	A.F = malloc(1*sizeof(int));
	A.F[0] = A.Q[1];
	A.tab_transi = malloc(1*sizeof(transition));
	A.tab_transi[0].etat_in = A.Q[0];
	A.tab_transi[0].cons = word;
	A.tab_transi[0].etat_fin = A.Q[1];
	
	return A ;
	
}

AFN Kleene(AFN A)
{
	AFN Kleene_A;
	Kleene_A.taille[0]=A.taille[0];
	Kleene_A.taille[1]=A.taille[1];
	//etats accepteurs
	Kleene_A.F=calloc(Kleene_A.taille[2],sizeof(int));
	int check=0;
	int j=0,i=0,k=0;
	//verifions si l'etat initial est deja accepteur ou pas
	for(j=0;j<A.taille[2];j++)
	{
		if(A.s==A.F[j])
		{
			check+=1;
		}
	}
printf("le check est %d \n",check);
	if(check!=0)
	{
		printf("l'etat initial est deja accepteur \n");

		Kleene_A.taille[2]=A.taille[2];//car l'etat initial n'est pas   accepteur
		
		for(i=0;i<Kleene_A.taille[2];i++)
			{				
				Kleene_A.F[i]=A.F[i];
			}
	}
	else
	{
			printf("l'etat initial devient accepteurr \n");

		Kleene_A.taille[2]=A.taille[2]+1;//car l'etat initial devient accepteur
		Kleene_A.F[0]=A.F[0];
		for(i=1;i<Kleene_A.taille[2];i++)
		{
			Kleene_A.F[i]=A.F[i];
		}
	}
	Kleene_A.taille[3]=A.taille[3];
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
	printf("le nobre d'etat accepteur est %d \n",Kleene_A.taille[2]);
	int trans_ajout=0;
	for(j=0;j<A.taille[3];j++)
	{
		for(k=0;k<A.taille[3];k++)
		{
			if((A.tab_transi[j].etat_in==A.s) && (A.tab_transi[j].etat_fin==A.F[k]))
			{
				trans_ajout+=1;
			}
		}
	}
	trans_ajout*=A.taille[2];

	printf("on ajoute %d transitions et le nobre de depart est %d  \n",trans_ajout,A.taille[2]);
	Kleene_A.taille[3]+=trans_ajout;
	Kleene_A.tab_transi=calloc((Kleene_A.taille[3]+trans_ajout),sizeof(transition));
	for(i=0;i<A.taille[3];i++)
	{
		Kleene_A.tab_transi[i]=A.tab_transi[i];
	}
	int ind=A.taille[3];
	
		for(i=0;i<A.taille[3];i++)//gestion des trasition qui reboucle sur elle meme
		{
			for(k=0;k<A.taille[3];k++)
				{
					if((A.tab_transi[i].etat_in==A.s) && (A.tab_transi[i].etat_fin==A.F[k]))
						{
							Kleene_A.tab_transi[ind].etat_in=A.F[k];
							Kleene_A.tab_transi[ind].etat_fin=A.tab_transi[i].etat_fin;
							Kleene_A.tab_transi[ind].cons=A.tab_transi[i].cons;
							ind+=1;
						}
				}
		}
		printf("l'etat initail n'est pas accepteur  est %d  \n",ind);
		int cpt=0;//permet de compter le nombre de transition qui partent de l'état initial
		for(i=0;i<A.taille[3];i++)
		{
			for(k=0;k<A.taille[3];k++)
				{
					if((A.tab_transi[i].etat_in==A.s) && (A.tab_transi[i].etat_fin==A.F[k]))
						{
							cpt+=1;
						}
				}
		}
			printf("aff val de cpt  est %d  \n",cpt);

		transition *new_tab=calloc(cpt,sizeof(transition));

		for(i=0;i<A.taille[3];i++)
		{
			for(k=0;k<A.taille[3];k++)
				{
					if((A.tab_transi[i].etat_in==A.s) && (A.tab_transi[i].etat_fin==A.F[k]))
						{
							new_tab[i].etat_in=A.tab_transi[i].etat_in;
							new_tab[i].cons=A.tab_transi[i].cons;
							new_tab[i].etat_fin=A.tab_transi[i].etat_fin;
							printf("contenu est new est:pour i =%d %d %c %d  \n",i,new_tab[i].etat_in,new_tab[i].cons,new_tab[i].etat_fin);

						}
				}
		}
	int u=0;

		for(i=0;i<A.taille[3];i++)
		{
				printf(" a.f[i]= %d et new_tab  %d est  et la val de ind = %d\n",A.F[i],new_tab[i].etat_fin,ind);
			for(u=0;u<cpt;u++)
			{
					if(A.F[i]!=new_tab[u].etat_fin)
					{
															printf("on entre deja la  %d le cons est %c \n",A.F[i],new_tab[i].cons);

									Kleene_A.tab_transi[ind].etat_in=A.F[u];
									Kleene_A.tab_transi[ind].etat_fin=new_tab[i].etat_fin;
									Kleene_A.tab_transi[ind].cons=new_tab[i].cons;
									ind+=1;
						}
			}
		}


	return Kleene_A;
}

AFN determinisation(AFN A)
{
	printf("determinisation \n");
	AFN Det;
	Det.taille[0]=A.taille[0];
	Det.taille[1]=A.taille[1];
	Det.taille[2]=A.taille[2];
	Det.taille[3]=A.taille[3];
	//on initialise pour faire des realloc en cas de besoin
	Det.Q=calloc(Det.taille[0],sizeof(int));
	Det.P=A.P;
	Det.F=calloc(Det.taille[2],sizeof(int));
	Det.tab_transi=calloc(Det.taille[3],sizeof(transition));
	//gestion de l'etat initial
	Det.s=A.s;
	//gestion des transition
	int i=0,id_act=0,I=0,nb_t=0,p=0;
	//int tab_new[1][2];
	
	
	int * tb_new=calloc(2*A.taille[0],sizeof(int));
	for(i=0;i<Det.tab_transi;i++)
	{
		for(I=0;I<Det.taille[3];I++)
		{
			if((A.tab_transi.etat_in==A.Q[i]) && (A.tab_transi[I].cons=='A'))
			{
				nb_t+=1;
			}
		}
		tab_new
	}
	Det.tab_transi=A.tab_transi;
	//gestion des etats
	Det.Q=A.Q;
	Det.F=A.F;
	return Det;
}

int main(int argc,char** argv)
{
	AFN E = empty_langage();
	//aff_auto(E);
	AFN M = mot_vide();
	//aff_auto(M);
	char a='a';
	char b='b';
	AFN O = one_char(a);
	//aff_auto(O);
	AFN afn1,afn2;
	afn1.taille[0]=4;
	afn1.taille[1]=2;
	afn1.taille[2]=3;
	afn1.taille[3]=3;
	afn1.Q=calloc(2,sizeof(int));
	afn1.Q[0]=0;
	afn1.Q[1]=1;
	afn1.Q[2]=2;
	afn1.Q[3]=3;
	afn1.s=afn1.Q[0];
	afn1.F=calloc(afn1.taille[2],sizeof(int));
	afn1.F[0]=0;
	afn1.F[1]=2;
	afn1.F[2]=3;
	
	
	afn1.tab_transi=calloc(afn1.taille[3],sizeof(transition));
	afn1.tab_transi[0].etat_in=0;
	afn1.tab_transi[0].cons=a;
	afn1.tab_transi[0].etat_fin=1;
	
	afn1.tab_transi[1].etat_in=1;
	afn1.tab_transi[1].cons=b;
	afn1.tab_transi[1].etat_fin=2;
	
	afn1.tab_transi[2].etat_in=0;
	afn1.tab_transi[2].cons='c';
	afn1.tab_transi[2].etat_fin=3;
	
	
	
	afn2.taille[0]=6;
	afn2.taille[1]=2;
	afn2.taille[2]=3;
	afn2.taille[3]=3;
	afn2.Q=calloc(2,sizeof(int));
	afn2.Q[0]=2;
	afn2.Q[1]=5;
	afn2.Q[2]=8;
	afn2.Q[3]=9;
	afn2.s=9;
	afn2.F=calloc(1,sizeof(int));
	afn2.F[0]=2;
	afn2.F[1]=5;
	afn2.F[2]=9;
	afn2.tab_transi=calloc(afn2.taille[3],sizeof(transition));
	
	afn2.tab_transi[0].etat_in=2;
	afn2.tab_transi[0].cons=b;
	afn2.tab_transi[0].etat_fin=5;
	afn2.tab_transi[1].etat_in=2;
	afn2.tab_transi[1].cons='A';
	afn2.tab_transi[1].etat_fin=8;
	afn2.tab_transi[2].etat_in=5;
	afn2.tab_transi[2].cons=a;
	afn2.tab_transi[2].etat_fin=9;
	
	
	//AFN R=reunion(afn1,afn2);
	//aff_auto(R);
	//aff_auto(afn1);
	//aff_auto(afn2);
	//AFN k=Kleene(afn1);
	//aff_auto(k);
	AFN d=determinisation(afn1);
	aff_auto(d);
	return 0;
}
