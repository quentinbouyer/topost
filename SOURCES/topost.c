#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <glib.h>

#include "topost.h"
#include "traite_arg.h"
#include "compte_ost.h"
#include "read_conf_file.h"
#include "li_stat.h"
#include "affiche_oss.h"

#define TAILLE_BLOC 1024

// GLOBAL VAR

// nb de client a afficher
int nbaffich=10;

// nb de client
int numclient=0;

// nb d'ost
int numost=0;

// pour stocker l'ost en argument
char ostn[16]="";

// pour la lecture d'un ost
io_ost *iost;

// provisoire
clitemp *nomip;

// pour traiter les arguments
//config_arg *argument_cmd;

time_t t;

// FIN GLOBAL VAR

DIR *rep = NULL;
struct dirent *fichierlu = NULL;

FILE *f1;
FILE *f2;

char ligne[80]="";
char buffer[80]="";
char *li = NULL;

int i=0;
int j=0;
long temp=0;

char servic[20];

// pour la fonction split_chaine
char **resultat_split;

//pour lire les fichier /proc/fs/lustre/devices
char c1[20]="";
char c2[20]="";
char c3[20]="";
char c4[20]="";

int main( int argc, char *argv[] )
{

	//lecture des arguments
	parse_args (argc, argv);

	printf ("%d clients max to display\n",nbaffich); 

	// on cherche le nombre d'ost
	compte_ost ();
	printf ("%d ost\n",numost);

 	//lit conf file, combien de client
 	read_conf_file(0);
	printf ("%d clients in /etc/topost.conf\n",numclient);

 	//donnees pour tous les clients de tous les ost
 	iost=malloc(numost * numclient * sizeof(*iost));

 	//pour stocker provisoirement addip et nom des clients
 	nomip=malloc(numclient * sizeof(*nomip));

 	//lit conf file, rempli la structure d'un client
 	read_conf_file(1);

 	printf("First read\n");

 	// initialize pour tous les clients de tous les ost
 	initialize_oss();

 	printf("Wait 10s before to read a second time\n");

 	sleep(10);

	printf("Second read \n");

 	li_stat_oss();

  	affiche_oss();
 	return 0;
}   
