#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "topost.h"

#define INTERVAL 10

extern int numclient;
extern int numost;

void li_stat_oss ()
{

 int i=0;
 int j=0;
 int nbenrg=0;
 unsigned long temp_read=0;
 unsigned long temp_write=0;
 unsigned long temp_iops=0;
 unsigned long ultemp=0;
 FILE *f2;
 extern io_ost *iost;
 char buffer[80]="";

 // pour supprimer \n avec fgets
 char *p="";


 // pour la glib
 gchar **result;

 nbenrg=numost*numclient;
 //printf("nbenrg=%d\n",nbenrg);

 for(i=0;i<nbenrg;i++)
 {
  //printf("i=%d path = %s \n",i,iost[i].path);
  // topost1.1 protection contre les entress vides
  if(iost[i].path)
  {
   if(NULL==(f2=fopen(iost[i].path,"r")))
   {
    //printf("Erreur dans l'ouverture de %s\n",iost[i].path);
    //exit(1);
    iost[i].ior=0;
    iost[i].iow=0;
    iost[i].iops_calcul=0;
   }
   else
   {
    fgets(buffer,80,f2);
    while(!strstr(buffer,"get_info") && !feof(f2))
    {
     // on supprime le \n a la fin de la chaine
     p=strchr(buffer,'\n');
     if ( p != NULL)
      *p='\0';

     if(strstr(buffer,"read_bytes"))
     {
      result=g_strsplit(buffer," ",0);  
      ultemp=strtoul(result[16],NULL,10);
      temp_iops=temp_iops+ultemp;
      ultemp=0;
      temp_read=strtoul(result[21],NULL,10);
      //iost[i].ior=((((temp_read-iost[i].read)/INTERVAL)/1024)/1024);
      iost[i].ior=((((temp_read-iost[i].read))/1024)/1024);
      iost[i].read=temp_read; 
      g_strfreev(result);
     }
     else if(strstr(buffer,"write_bytes"))
     {
      result=g_strsplit(buffer," ",0);
      ultemp=strtoul(result[15],NULL,10);
      temp_iops=temp_iops+ultemp;
      ultemp=0;
      temp_write=strtoul(result[20],NULL,10);
      iost[i].iow=((((temp_write-iost[i].write))/1024)/1024);
      iost[i].write=temp_write;
      g_strfreev(result);
     }
     fgets(buffer,80,f2);
    }
    fclose(f2);

    iost[i].iops_calcul=(temp_iops-iost[i].iops_lu);
    //printf("ost=%s iops_calcul=%lu\n",iost[i].ost_name,iost[i].iops_calcul);
    iost[i].iops_lu=temp_iops;
    temp_iops=0;
    //printf("ost=%s path=%s addip=%s host=%s ior=%ld iow=%ld\n",iost[i].ost_name,iost[i].path,iost[i].addip,iost[i].nomhost,iost[i].ior,iost[i].iow);
   }
  }
 }
}
