#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "topost.h"

// GLOB VAR

extern clitemp *nomip;
extern int numclient;

// END OF GLOB VAR

void read_conf_file (int flag)
{

 // field in conf file
 char *CHAMP1="mds";
 char *CHAMP2="client";


 // pointer for char split whit oss and remove \n
 char *pos=NULL;
 char *p=NULL;
 int i=0;
 gchar **resultat;

 //for conf file
 FILE *f;
 FILE *f2;
 char buffer[80]="";
 char chemin[80]="";
 char temp[80]="";

 //sep caracter for config file
 char *sep_egal="=";
 char *sep_virgule=",";

 //argument_cmd->ltools_conf = g_strdup_printf ("/etc/topost.conf");

 if ( NULL==(f=fopen("/etc/topost.conf","r")))
 {
  printf("Impossible d'ouvrir le fichier de config /etc/topost.conf\n");
  exit (EXIT_FAILURE);
 }
 
 if ( flag == 0 )
 {
  while( fgets(buffer,sizeof(buffer),f) != NULL )
  {
   //on cherche le nombre de client
   if (strstr(buffer,CHAMP2))
   {
    numclient++;
   }
  }
  if(numclient == 0 )
  {
   printf("Il faut au moins un client dans /etc/topost.conf\n"); 
   exit(EXIT_FAILURE);
  }
 }
 else if ( flag == 1 )
 {
  //on recupere toutes les adresse ip des clients
  while( fgets(buffer,sizeof(buffer),f) != NULL )
  {
   //printf("buffer=%s\n",buffer);
   if (strstr(buffer,CHAMP2))
   {
    //on supprime le \n a la fin de la chaine
    p=strchr(buffer,'\n');
    if ( p != NULL) *p='\0';

    //go to the field in the str
    pos=strtok(buffer,sep_egal);
    pos=strtok(NULL,sep_egal);
    memcpy(temp,pos,strlen(pos));

    resultat=g_strsplit(temp,",",0);

    if(resultat[0] == NULL)
    {
     printf("Erreur de syntaxe dans /etc/topost.conf pour\n");
     printf("%s\n",resultat[0]);
     exit(EXIT_FAILURE);
    }
    else
    {
     strcpy(nomip[i].nomcli,resultat[0]);
     //printf("nomcli=%s\n",nomip[i].nomcli);
    }
    if(resultat[1] == NULL)
    {
     printf("Erreur de syntaxe dans /etc/topost.conf pour\n");
     printf("%s\n",resultat[0]);
     exit(EXIT_FAILURE);
    }
    else
    {
     strcpy(nomip[i].ipcli,resultat[1]);
     //printf("ipcli=%s\n",nomip[i].ipcli);
    }

    g_strfreev(resultat);
    i++;
    // remise a zero des buffers
    memset(&buffer[0],0,sizeof(buffer));
    memset(&temp[0],0,sizeof(temp));
   }
  } 
 }
 //close the conf file
 fclose(f);
}
