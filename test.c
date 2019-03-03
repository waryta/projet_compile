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
	for(i=0;i<4;i++)
	{
		printf("A.taille[%d] = %d :\n",i,A.taille[i]);
	}
	
	for(i=0;i<A.taille[0];i++)
	{
		printf("l'état %d est %d :\n",i,A.Q[i]);
	}
	//printf( "A.P[0] = %d " ,A.P[0]);
	for(j=0;j<A.taille[1];j++)
	{
		printf("le caractère %d de l'alphabet est %c :\n",j,A.P[j]);
	}
	printf("l'état initial est %d :\n",A.s);
    for(k=0;k<A.taille[2];k++)
	{
		printf("l'etat accepteur numero %d est %d :\n",k,A.F[k]);
	}
	for(t=0;t<A.taille[3];t++)
	{
		printf("la transistion %d est (%d ,%c, %d):\n",t,A.tab_transi[t].etat_in,A.tab_transi[t].cons,A.tab_transi[t].etat_fin);
	}
}


AFN empty_langage()
{
	AFN A;
	
	
	A.taille[0]=1;
	A.taille[1]=0;
	A.taille[2]=0;
	A.taille[3]=0;
	A.P = NULL;
	A.Q = malloc(1*sizeof(int));
	A.s=A.Q[0];
	A.F = NULL;
	A.tab_transi = NULL;
	
	return A ;
	
	
}


AFN one_char(char word)
{
	
	AFN A;
	
	//printf("\n Mot donne en parametre : %c", word);
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

AFN concat(AFN A,AFN B)
{   
	
	//printf("Hello \n");
	 
	AFN C ; // Automate reconnaissant la concatenation de A et B
	int i=0, j =0, indb =0 , deb_ini =0, ac_A = A.taille[2]; 
	
	// Changement des numéros d'états de l'automate B
	B.s += A.taille[0];
	for(i=0 ; i< B.taille[0];i++)
	{ 
		B.Q[i] +=A.taille[0];
		printf ("\n B.Q[%d] = %d",i,B.Q[i]);	
	}

    for(i=0 ; i< B.taille[2];i++)
	{ 
		B.F[i] +=A.taille[0];
		printf ("\n B.F[%d] = %d",i,B.F[i]);	
	}
	
	for(i=0; i< B.taille[3];i ++)
	{
		 
		 B.tab_transi[i].etat_in  += A.taille[0];
		 B.tab_transi[i].etat_fin += A.taille[0];
		 printf ("\n B.tab_transi[%d].etat_in = %d",i,B.tab_transi[i].etat_in);
		  printf ("\n B.tab_transi[%d].etat_fin = %d",i,B.tab_transi[i].etat_fin);
	
	}
	
	// les états de C sont tout ceux de A et tout ceux de B sauf l'état initial de B
	
	
	C.taille[0] = A.taille[0] + B.taille[0]-1;
	C.Q = malloc(C.taille[0]*sizeof(int));
	
	for(i =0;i<A.taille[0];i++)
	{
		C.Q[i] = A.Q[i];
		
	}
	
	/* ind est l'indice de départ pour les éléments de B qui nous interessent */
	 int ind =1; 
	 
	for(i=A.taille[0];i<A.taille[0]+B.taille[0]-1;i++)
	{
		C.Q[i] = B.Q[ind];
		ind++;
	}
	ind =0;
	// Le langage de C est l'union des langages de A et de B
	C.taille[1] = A.taille[1]+B.taille[1];
	//printf( "C.taille[1] = %d avec S.t = %d et T.t = %d",C.taille[1],A.taille[1],B.taille[1]);
	C.P = malloc(C.taille[1]*sizeof(char));
	

	if(A.taille[1] == 0 )
	{
	
	    //printf( " \n A.taille[1] = 0  et B.P[0] = %c", B.P[0]);
	    for(i=0;i<B.taille[1];i++)
	     {
		    C.P[i] = B.P[i];
		    //printf( " \n C.P[%d] = %c",i,C.P[i]);
		   
	     }
	  //printf( " \n ON A C.P[0] = %c \n ",C.P[0]);    	   	
	}
	else 
	{
		
		for(i =0;i<A.taille[1];i++)
	    {
		   C.P[i] = A.P[i];
	    }
		
		
	     for(i=A.taille[1];i< A.taille[1] + B.taille[1];i++)
	     {
		    C.P[i] = B.P[ind];
		    ind++;
	     }
	
    }
    C.s = A.s;
    int verif_accepteur=0;
	/* on vérifie si l'état initial de B est accepteur */
	for( i=0; i< B.taille[2];i++)
	{
		if( B.F[i] == B.s)
		{
			verif_accepteur+=1;
		}
	}
	//printf(" \n VERI_ACCEPTEUR = %d  avec B.F[0] = %d et B.s = %d \n", verif_accepteur,B.F[0],B.s); 
	
	
	/* Si c'est le cas  on garde tous les etats accepteurs de B sauf ce dernier
	 * ainsi que tous ceux de A*/ 
	
	if(verif_accepteur >0)
	{  
		printf(" INI B EST ACCEPTEUR"); 
		ind = 1;
		C.taille[2] = A.taille[2]+B.taille[2] -1;
		C.F = malloc(C.taille[2] * sizeof(int));
		
		for(i=0;i<A.taille[2];i++)
	    {
		   C.F[i] = A.F[i]; 
	    }
	    
	    if(A.taille[2] ==0) // rien n'a été rempli dans C.F
	    {
			
			 for(i=0;i<B.taille[2]-1;i++)
	         {
		       C.F[i] = B.P[ind];
		       ind ++;
		  
		     }
		
		}
	    else
	    {
	       
	      
	       
	       for(i=A.taille[2];i< A.taille[2]+B.taille[2]-1;i++)
	       {
		     C.F[i] = B.P[ind];
		     ind ++;
		  
		   }
	    }
		
	}
	else
	/*  Sinon on ne garde que ceux de B*/
	{
		
		ac_A = 0;
		printf(" INI B PAS  ACCEPTEUR"); 
		C.taille[2] = B.taille[2];
		C.F = malloc(C.taille[2] * sizeof(int));
		
	    for(i=0;i< B.taille[2];i++)
	    {
		   C.F[i] = B.F[i];
		   printf( "\n C.F[%d] = %d \n ",i,C.F[i] );
		   
		}
		
	}
	
   /*ind va nous permettre de connaître le nombre de 
	* transitions dans B ne commençant pas par l'état initial */
	ind =0; 
	//printf(" B.taille[3] = %d et ",B.taille[3]);
	for(i=0;i<B.taille[3];i++)
	{
		//printf(" B.tab_transi[i].etat_in = %d  avc B.s= %d",B.tab_transi[i].etat_in,B.s);
		if(B.tab_transi[i].etat_in != B.s)
		{ ind ++;}
	}
	printf ("\n Il y a %d transi ds B comm pas par %d ",ind,B.s);
	C.taille[3] = A.taille[3]+ ind +(ac_A*(B.taille[3]-ind));
	
	if( C.taille[3] ==0)
	{
	   C.taille[1] =0;
	
	}
	else{
	C.tab_transi = malloc(C.taille[3]* sizeof(int));
	printf(" C.taille[3] = %d   \n A.t[3] = %d \n A.t[2] = %d \n B.t[3]-ind = %d et ",C.taille[3],A.taille[3],A.taille[2],B.taille[3]-ind);
	
	/* On conserve toutes les transitions de A s'il y en a */
	for(i=0;i<A.taille[3];i++)
	{
		C.tab_transi[i] = A.tab_transi[i];
	}
    if(A.taille[3] ==0) //C.tab_transi est toujours vide
    {
		
	/* En commençant par la première case du tableau de transitions,on ajoute les transitions (a,c,p) 
     où a est un état accepteur de A et telque (i,c,p) soit une transition partant de l'état initial i de B ) */
		indb =0;
	    for(i=0;i<A.taille[2];i++)
	    {
	      for(j=0;j<B.taille[3]-ind;j++)
	      {
	   	     C.tab_transi[indb].etat_in =A.F[i];
		     C.tab_transi[indb].cons = B.tab_transi[j].cons;
		     C.tab_transi[indb].etat_fin = B.tab_transi[j].etat_fin;
		     indb ++;
		   
	      }
	    }
	
	
	/* on conserve aussi toutes les transitions de B ne partant pas de
	  * l'état initial i, deb_ini est le nombre de transitions de B
	  *  commençant par son état initial */
	   deb_ini = B.taille[3]-ind;
	  for(i = indb ; i < C.taille[3]; i++)
	  {
		  C.tab_transi[i] = B.tab_transi[deb_ini];
		  deb_ini ++;
	  }
	
	}
    else
    {
    /* A la suite des transitions de A précedemment ajoutées , on complète le tableau avec 
       les transitions (a,c,p) où a est un état accepteur de A et telque (i,c,p) soit une 
       transition partant de l'état initial i de B ) */
    indb = A.taille[3];
	//transition t;
	for(i=0;i<A.taille[2];i++)
	{
	   for(j=0;j<B.taille[3]-ind;j++)
	   {
		   C.tab_transi[indb].etat_in =A.F[i];
		   C.tab_transi[indb].cons = B.tab_transi[j].cons;
		   C.tab_transi[indb].etat_fin = B.tab_transi[j].etat_fin;
		   indb ++;
		   
	   }
	}
	
		
	 /* on conserve aussi toutes les transitions de B ne partant pas de
	  * l'état initial i ;*/
	 deb_ini = B.taille[3]-ind;
	  for(i = indb ; i < C.taille[3]; i++)
	  {
		  C.tab_transi[i] = B.tab_transi[deb_ini];
		  deb_ini ++;
	  }
	} 
	
	C.taille[1] = C.taille[3];
	C.P = malloc(C.taille[1]*sizeof(char));
	for(i=0;i<C.taille[1];i++)
	{
		C.P[i] = C.tab_transi[i].cons;
	}
	
}
	
	printf( "FINI");
	return C;
	
}

AFN mot_vide()
{
	printf("automate reconnaissant le language composé du seul mot vide \n");
	AFN A;
	A.taille[0]=1;
	A.taille[1]=1;
	A.taille[2]=1;
	A.taille[3]=0;
	A.Q=calloc(1,sizeof(int));//permet d'initialiser à 0 les cases du tableau
	A.Q[0]=0;
	A.P=calloc(1,sizeof(char));
	A.P[0] = '$';
	A.s=A.Q[0];
	A.F=calloc(1,sizeof(int));
	A.F[0]=A.Q[0];
	A.tab_transi=NULL;

	return A;
}

int f_transi(int etat,char c, AFD D)
{
	int etat_f = -1;
	int i =0;
	
	for(i=0;i<D.taille[3];i++)
	{
	  if((D.tab_transiD[i].etat_in == etat)&&(D.tab_transiD[i].cons == c))
	  {
		  etat_f = D.tab_transiD[i].etat_fin;
	  }	
	}
	
	return etat_f;
}

AFD create_AFD()
{
	//int i =0;
	AFD A;
	A.taille[0]=3;
	A.taille[1]=2;
	A.taille[2]=1;
	A.taille[3]=2;
	A.QD=calloc(3,sizeof(int));//permet d'initialiser à 0 les cases du tableau
	A.QD[0]=0;
	A.QD[1]=1;
	A.QD[2]=2;
	A.PD=calloc(2,sizeof(char));
	A.PD[0] = 'a';
	A.PD[1] = 'b';
	A.sD=A.QD[0];
	A.FD=calloc(1,sizeof(int));
	A.FD[0]=A.QD[1];
	A.tab_transiD = calloc(A.taille[3],sizeof(transition));
	A.tab_transiD[0].etat_in = A.QD[0];
	A.tab_transiD[0].cons = A.PD[0];
	A.tab_transiD[0].etat_fin = A.QD[1];
	A.tab_transiD[1].etat_in = A.QD[0];
	A.tab_transiD[1].cons = A.PD[1];
	A.tab_transiD[1].etat_fin=A.QD[2];
    int (*pointeur_f_transi)(int ,char, AFD );
    
    pointeur_f_transi = f_transi;
    
       
	return A;

}


int main(int argc,char** argv)
{   
	
	 
	 AFD D = create_AFD();
	 AFD G = create_AFD();
	 D.pointeur_f_transi = f_transi;
	 int essai = (*D.pointeur_f_transi)(0,'a',G);
    
    printf("\n  essai = %d", essai); 
	
	AFN MOV = mot_vide();
	AFN ENSV = empty_langage();
	AFN UNCHAR = one_char('c');
	//printf("Affichage de l'automate  ENSV reconnaisant le langage vide");
	//printf("****************************************************\n");
	aff_auto(MOV);
	
	//printf("Affichage de l'automate  UNCHAR reconnaisant un caractère");
	//printf("****************************************************\n");
	//aff_auto(UNCHAR);
	
	//concat(ENSV,UNCHAR);
	AFN U = concat(UNCHAR,MOV);
	
	printf(" \n Affichage de l'automate Qui concatène UNCHAR et MOV ");
	printf("****************************************************\n");
	aff_auto(U);
	
	
	return 0;
}
