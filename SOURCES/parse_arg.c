#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include <glib.h>

// GLOB VAR

extern int nbaffich;

// END OF GLOB VAR


void print_version () {
	printf ("topost version 2.1\n"); 
	printf ("quentin.bouyer@gmail.com\n");
} 
 
void print_help ( const char *str ) {
	printf ("Usage : topost\n" 
		"sans argument\t: affiche les 10 clients les plus consommateurs\n"
		"-n X\t\t: limite l'affichage pour X client au max (10 par defaut)\n"
		"-h\t\t: pour afficher ce message d'aide\n"
		"-v\t\t: pour afficher la version\n");
	if (str)
		printf ("%s\n", str);
}
 
void parse_args (int argc, char **argv)
{ 
  	int i; 
	int val;
	DIR *rep=NULL;
	gchar **resultat;

	struct tm ti={0};
	time_t datec;

	/* Boucle d’analyse de la ligne de commande. */ 
  	for (i = 1; i < argc; i++) {

      		if (!strcmp (argv[i], "-n")) {
          		i++; 
          		if (i < argc) {  
				val = strtol (argv[i], NULL, 10);
				if ((0 == val) && (EINVAL == errno))
				{
					fprintf (stderr, "Impossible d'analyser le parametre pour -n\n");
					fprintf (stderr, "On prendra la valuer par defaut, soit 10\n");
					nbaffich = 10;
				}
				else
					nbaffich = val;
			} 
          		else 
			{
              			print_help ("Argument manquant pour -n"); 
              			exit (EXIT_FAILURE); 
            		} 
        	} 

		/* Arguments "-v" et "-h". */ 
      		else if (!strcmp (argv[i], "-v")) { 
			print_version ();
          		exit (EXIT_SUCCESS); 
        	} 

      		else if (!strcmp (argv[i], "-h")) { 
			print_help ("");	
          		exit (EXIT_SUCCESS); 
        	} 

		/* Argument non reconnu. */ 
      		else { 
			printf ("%s: Argument non reconnu\n",argv[i]);
			printf ("-h pour l'aide\n");
          		exit (EXIT_FAILURE); 
        	}
	}
}
