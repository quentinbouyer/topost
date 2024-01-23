#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glib.h>
#include <string.h>

#include "topost.h"

// GLOB VAR

extern int numost;

// END OF GLOB VAR

void compte_ost ()
{
 DIR *rep;
 struct dirent *fichierlu;

 struct stat buf;

 char path_complet[80]="";


 if(NULL==(rep=opendir("/proc/fs/lustre/obdfilter")))
 {
  	fprintf (stderr, "Impossible de trouver /proc/fs/lustre/obdfilter\n");
  	exit ( EXIT_FAILURE );
 }
 while ((fichierlu = readdir (rep)))
 {
  	if(fichierlu->d_name && strcmp(fichierlu->d_name,".") && strcmp(fichierlu->d_name,".."))
  	{
   		sprintf(path_complet,"/proc/fs/lustre/obdfilter/%s",fichierlu->d_name);
   		if (stat (path_complet, &buf))
   		{
			fprintf (stderr, "Probleme avec stat()\n");
			exit ( EXIT_FAILURE );
   		}
   		if (S_ISDIR (buf.st_mode)) 
			numost++; 

   		memset(&path_complet[0],0,sizeof(path_complet));
  	}
 }
 closedir (rep);
}
