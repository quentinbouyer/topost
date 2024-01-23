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
 int k=0;
 int nbenrg=0;
 unsigned long temp_read=0;
 unsigned long temp_write=0;
 unsigned long temp_iops=0;
 unsigned long ultemp=0;
 FILE *f3;
 extern io_ost *iost;
 char buffer[256]="";

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
   		if(NULL==(f3=fopen(iost[i].path,"r")))
   		{
    			//printf("Erreur dans l'ouverture de %s\n",iost[i].path);
    			iost[i].ior=0;
    			iost[i].iow=0;
    			iost[i].iops_calcul=0;
   		}
   		else
   		{
    			while( fgets(buffer,256,f3) )
    			{
				
				//printf("buffer = %s\n", buffer);
     				// on supprime le \n a la fin de la chaine
     				p=strchr(buffer,'\n');
     				if ( p != NULL)
      					*p='\0';
				
     				if (strstr(buffer,"read_bytes"))
     				{
      					result=g_strsplit(buffer," ",23);  
      					ultemp=strtoul(result[16],NULL,10);
      					temp_iops=temp_iops+ultemp;
      					ultemp=0;
      					temp_read=strtoul(result[21],NULL,10);

      					iost[i].ior=((((temp_read-iost[i].read))/1024)/1024);
      					iost[i].read=temp_read; 
					
					for(k=0;k<23;k++)
                                        	free(result[k]);
      					//g_strfreev(result);
     				}
     				else if (strstr(buffer,"write_bytes"))
     				{
                                	//printf("buffer = %s\n",buffer);
					//printf("path = %s addip=%s\n",iost[i].path,iost[i].addip);


      					result=g_strsplit(buffer," ",23);
					//for(k=0;k<22;k++)
                                       	//	printf("k=%d str=%s\n",k,result[k]);

      					ultemp=strtoul(result[15],NULL,10);
      					temp_iops=temp_iops+ultemp;

					//printf("write ost=%s ultemp=%lu temp_iops=%lu\n",iost[i].ost_name,ultemp,temp_iops);

      					ultemp=0;
      					temp_write=strtoul(result[20],NULL,10);

      					iost[i].iow=((((temp_write-iost[i].write))/1024)/1024);
      					iost[i].write=temp_write;

					for(k=0;k<23;k++)
                                        	free(result[k]);
      					//g_strfreev(result);
     				}
				else if (strstr(buffer,"quotactl"))
                        	{
                                	result=g_strsplit(buffer," ",23);
                                	//for(k=0;k<22;k++)
                                        //	printf("k=%d str=%s\n",k,resultat[k]);

                                	ultemp=strtoul(result[18],NULL,10);
                                	//temp_iops_init=temp_iops_init+ultemp;
                                	//ultemp=0;
                                	iost[i].io_quota=(ultemp-iost[i].quotactl);
                                	for(k=0;k<22;k++)
                                        	free(result[k]);
					//printf("ost=%s host=%s quotactl = %ld\n",iost[i].ost_name,iost[i].nomhost,ultemp);
                        	}
				else if (strstr(buffer,"punch"))
				{
					result=g_strsplit(buffer," ",23);
					ultemp=strtoul(result[21],NULL,10);
					iost[i].count_punch=(ultemp-iost[i].punch);
					for(k=0;k<22;k++)
                                                free(result[k]);
					//printf("ost=%s host=%s punch = %ld\n",iost[i].ost_name,iost[i].nomhost,iost[i].count_punch);
				}
				memset(buffer,'0',sizeof(buffer));
				
    			}
    			fclose(f3);

    			iost[i].iops_calcul=(temp_iops-iost[i].iops_lu);
    			//printf("ost=%s iops_calcul=%lu\n",iost[i].ost_name,iost[i].iops_calcul);
    			iost[i].iops_lu=temp_iops;
    			temp_iops=0;
    			//printf("ost=%s path=%s addip=%s host=%s ior=%ld iow=%ld\n",iost[i].ost_name,iost[i].path,iost[i].addip,iost[i].nomhost,iost[i].ior,iost[i].iow);
   		}
  	}
 }
}
