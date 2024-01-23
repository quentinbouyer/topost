#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <glib.h>

#include "topost.h"

// GLOB VAR

extern clitemp *nomip;
extern int numclient;

// END OF GLOB VAR

void compte_nid()
{
	DIR *dir_lustre;
    	DIR *dir;

    	struct dirent *ent_lustre;
    	struct dirent *ent;

	char temp[256];
	
	numclient=0;

    	if ((dir_lustre = opendir("/proc/fs/lustre/obdfilter")) != NULL)
        {
   		while ((ent_lustre = readdir (dir_lustre)))
 		{
  			if(ent_lustre->d_name && strcmp(ent_lustre->d_name,".") && strcmp(ent_lustre->d_name,"..") && numclient == 0)
  			{
				sprintf(temp,"/proc/fs/lustre/obdfilter/%s/exports",ent_lustre->d_name);
				//printf("dir trouve = %s\n", temp);

   				if ((dir = opendir(temp)) != NULL)
        			{
                			while ((ent = readdir(dir)) != NULL)
                			{
                        			if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                        				numclient++;
                			}
					closedir (dir);
        			}
				else
				{
					fprintf ( stderr, "Erreur dans l'ouverture de %s\n", temp);
					exit (EXIT_FAILURE);
				}
			}
		}
		closedir(dir_lustre);
	}
	else 
	{
        	fprintf (stderr, "Impossible d'ouvrir le répertoire /proc/fs/lustre/obdfilter");
        	exit ( EXIT_FAILURE );
    	}
}
