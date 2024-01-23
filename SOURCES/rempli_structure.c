#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <glib.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "topost.h"
#include "tranforme_texte.h"

// GLOB VAR

extern clitemp *nomip;
extern int numclient;

// END OF GLOB VAR

void rempli_structure()
{

 // pointer for char split whit oss and remove \n
 //char *pos=NULL;
 //char *p=NULL;
 int i=0;
 int k=0;

 gchar **resultat;
 char **chaine;

 char *sep="@";

 DIR *dir=0;
 DIR *replustre=0;

 struct dirent *ent;
 struct dirent *dirlustre;

 char temp[256];
 int flag=0;

 if ( (replustre = opendir("/proc/fs/lustre/obdfilter")) != NULL)
 {
	while ((dirlustre = readdir (replustre)) != NULL)
        {
        	if (dirlustre->d_name && strcmp(dirlustre->d_name,".") && strcmp(dirlustre->d_name,"..") && flag == 0)
                {
			sprintf(temp,"/proc/fs/lustre/obdfilter/%s/exports",dirlustre->d_name);

			if ((dir = opendir(temp)) != NULL)
                        {
                        	while ((ent = readdir(dir)) != NULL )
                                {
					if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
              				{
                      				//printf("nid = %s i=%d \n", ent->d_name, i);
                      				strcpy(nomip[i].ipcli,ent->d_name);

                      				resultat=g_strsplit(ent->d_name,"@",2);
                      				if(resultat[0] != NULL)
                      				{
                              				strcpy(nomip[i].nomcli,resultat[0]);
                              				//printf("nomcli=%s ipcli=%s\n",nomip[i].nomcli,nomip[i].ipcli);
							for(k=0; k < 2; k++)
								free(resultat[k]);
                      				}
						//g_strfreev (resultat);
              					i++;
              				}
      				}
				flag = 1;
      				closedir(dir);
			}
			else
			{
				fprintf ( stderr, "Impossible d'ouvrir %s\n", temp);
				exit (EXIT_FAILURE);
			}
			memset(temp,'\0',sizeof(temp));
		}
	}
	closedir(replustre);
 }
 
}
