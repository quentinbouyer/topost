#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <glib.h>
#include <string.h>

#include "topost.h"

// GLOB VAR

extern io_ost *iost;
extern clitemp *nomip;
extern int numost;
extern int numclient;
extern char ostn[16];

// END OF GLOB VAR

void initialize_oss ()
{

 int i=0;
 int j=0;
 int k=0;
 int co=0;
 DIR *rep = NULL;
 DIR *rep2 = NULL;
 struct dirent *fichierlu = NULL;
 struct dirent *fichierlu2 = NULL;

 // pour le split chaine
 gchar **resultat;

 FILE *f2;

 // pour les iops
 unsigned long temp_iops_init=0;
 unsigned long ultemp=0;

 // nb total d'enregistrement
 int nbenrg=0;

 //pour supprimer le caractere \n dans le fgets
 char *p="";

 char buffer[256]="";

 //premier parcours pour finir de remplir la structure avec tous les ost
 if ( numost > 0 )
 {
  	if ( (rep = opendir("/proc/fs/lustre/obdfilter")) == NULL )
  	{
   		fprintf (stderr, "Impossible de trouver /proc/fs/lustre/obdfilter\n");
   		exit (EXIT_FAILURE);
  	}
	else
	{
  		for(i=0;i<numost+2;i++)
  		{
   			fichierlu=readdir(rep);
   			if(fichierlu->d_name && strcmp(fichierlu->d_name,".") && strcmp(fichierlu->d_name,".."))
   			{
    				for(j=0;j<numclient;j++)
    				{
     					strcpy(iost[co].ost_name,fichierlu->d_name);
     					sprintf(iost[co].path,"/proc/fs/lustre/obdfilter/%s/exports/%s/stats",fichierlu->d_name,nomip[j].ipcli);
     					strcpy(iost[co].addip,nomip[j].ipcli);
     					strcpy(iost[co].nomhost,nomip[j].nomcli);
					iost[co].ior=0;
					iost[co].iow=0;
					iost[co].io_quota=0;
					iost[co].count_punch=0;
     					//printf("ost=%s path=%s addip=%s host=%s\n",iost[co].ost_name,iost[co].path,iost[co].addip,iost[co].nomhost);
     					co++;
    				}
   			}
  		} 
  		closedir(rep);
		co=0;
	}
 }

 /// NOUVELLE BOUCLE A FAIRE DE 0 A NUMCLIENT * NUMOST ET LIRE LES INFOS

 nbenrg=numclient*numost;

 for(i=0;i<nbenrg;i++)
 {
  	if ( (f2=fopen(iost[i].path,"r")) != NULL )
  	{
		//printf("fichier = %s \n",iost[i].path);
   		while(fgets(buffer,256,f2))
   		{       
    			// on supprime le \n a la fin de la chaine
    			p=strchr(buffer,'\n');
    			if ( p != NULL)
     				*p='\0';

    			if(strstr(buffer,"read_bytes"))
    			{       
     				//printf("path = %s addip=%s\n",iost[co].path,iost[co].addip);
     				resultat=g_strsplit(buffer," ",23);
				//for(k=0;k<22;k++)
				//	printf("k=%d str=%s\n",k,resultat[k]);

     				ultemp=strtoul(resultat[16],NULL,10);
     				temp_iops_init=ultemp+temp_iops_init;
     				//printf("read ost=%s ultemp=%lu temp_iops_init=%lu\n",iost[i].ost_name,ultemp,temp_iops_init);
     				ultemp=0;
     				iost[i].read=strtoul(resultat[21],NULL,10);
				for(k=0;k<22;k++)
     					free(resultat[k]);
				//g_strfreev(resultat);
    			}       
    			else if(strstr(buffer,"write_bytes"))
    			{       
     				//printf("buffer = %s\n",buffer);
				//printf("path = %s addip=%s\n",iost[co].path,iost[co].addip);

     				resultat=g_strsplit(buffer," ",23);
				//for(k=0;k<22;k++)
                                //	printf("k=%d str=%s\n",k,resultat[k]);

     				ultemp=strtoul(resultat[15],NULL,10);
     				temp_iops_init=temp_iops_init+ultemp;

     				//printf("write ost=%s ultemp=%lu temp_iops_init=%lu\n",iost[i].ost_name,ultemp,temp_iops_init);

     				ultemp=0;
     				iost[i].write=strtoul(resultat[20],NULL,10);
     				//printf("write ost=%s %lu\n",iost[i].ost_name,iost[i].write);

				for(k=0;k<22;k++)
                                        free(resultat[k]);
     				//g_strfreev(resultat);
    			}
			else if(strstr(buffer,"quotactl"))
			{
				//printf("path = %s addip=%s\n",iost[i].path,iost[i].addip);
                                //printf("buffer = %s\n",buffer);

				resultat=g_strsplit(buffer," ",23);
				//for(k=0;k<22;k++)
                                //        printf("k=%d str=%s\n",k,resultat[k]);

				iost[i].quotactl=strtoul(resultat[18],NULL,10);
				for(k=0;k<22;k++)
                                        free(resultat[k]);
				//printf("ost=%s host=%s quotactl = %ld\n",iost[i].ost_name,iost[i].nomhost,iost[i].quotactl);
			}
			else if(strstr(buffer,"punch"))
			{
				//printf("buffer = %s\n",buffer);
				//printf("path = %s addip=%s\n",iost[i].path,iost[i].addip);

				resultat=g_strsplit(buffer," ",23);
				//for(k=0;k<22;k++)
                                //        printf("k=%d str=%s\n",k,resultat[k]);

                                iost[i].punch=strtoul(resultat[21],NULL,10);
				for(k=0;k<22;k++)
                                        free(resultat[k]);
				//printf("path = %s addip=%s punch=%ld\n",iost[i].path,iost[i].addip,iost[i].punch);
			}

			memset(buffer,'0',sizeof(buffer));
   		}

   		//iops
   		//printf("iops_init=%lu\n",temp_iops_init);
                //g_strfreev(resultat);
   		iost[i].iops_lu=temp_iops_init;
   		temp_iops_init=0;
                fclose (f2);

   		//memset(buffer,'0',sizeof(buffer));
   		//printf("ost=%s path=%s addip=%s host=%s read=%lu write=%lu\n",iost[i].ost_name,iost[i].path,iost[i].addip,iost[i].nomhost,iost[i].read,iost[i].write);
  	}
 }
}
