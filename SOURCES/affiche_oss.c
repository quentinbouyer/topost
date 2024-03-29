#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <glib.h>

#include "topost.h"

//GLOB VAR

extern nbaffich;
extern io_ost *iost;
extern int numost;
extern int numclient;
extern char ostn[16];
extern time_t t;


//END OF GLOB VAR

//fonctions pour le qsort

long compareior (const void *v1, const void *v2)
{
  const io_ost *u1 = v1;
  const io_ost *u2 = v2;
  return (u2->ior - u1->ior);
}

long compareiow (const void *v1, const void *v2)
{
  const io_ost *u1 = v1;
  const io_ost *u2 = v2;
  return (u2->iow - u1->iow);
}

long compareiops (const void *v1, const void *v2)
{
  const io_ost *u1 = v1;
  const io_ost *u2 = v2;
  return (u2->iops_calcul - u1->iops_calcul);
}

long comparequota (const void *v1, const void *v2)
{
  const io_ost *u1 = v1;
  const io_ost *u2 = v2;
  return (u2->io_quota - u1->io_quota);
}

long comparepunch (const void *v1, const void *v2)
{
  const io_ost *u1 = v1;
  const io_ost *u2 = v2;
  return (u2->count_punch - u1->count_punch);
}


void affiche_oss ()
{

 FILE *load;
 float load1=0;
 float load5=0;
 float load15=0;
 char loadchaine[80]="";
 int i=0;
 int res=0;
 int nbenrg=0;

 nbenrg=numclient*numost;

 struct in_addr ip_addr;
 struct hostent *host;

 long total_read=0;
 long total_write=0;
 long total_riops=0;
 long total_wiops=0;
 int nbcli=0;

 time(&t);

 // lecture du load average

 if(NULL==(load=fopen("/proc/loadavg","r")))
 {
  	fprintf(stderr,"Impossible de trouver loadavg\n");
  	exit(EXIT_FAILURE);
 }
 fscanf(load,"%f %f %f %s",&load1,&load5,&load15,&loadchaine);
 fclose(load);

 qsort(iost,nbenrg,sizeof(io_ost),compareior);
 
 if ( iost[0].ior > 0 )
 {
 	printf("------------------------------------------------------------------------------------\n");
 	printf("Read\n");
 	printf("OST \t\t\tCLIENT \t\t\t\tMB_read \t\t\tIOPS\n");
 	printf("------------------------------------------------------------------------------------\n");

 	for(i=0; i < nbaffich; i++)
 	{
  		if(iost[i].ior > 0)
  		{
   			inet_aton(iost[i].nomhost, &ip_addr);

   			host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);

   			if (host != NULL) 
    				strcpy(iost[i].nomhost,host->h_name);

   			printf("%s\t%16s\t\t\t%ld\t\t\t\t%ld\n",iost[i].ost_name, iost[i].nomhost, iost[i].ior, iost[i].iops_calcul);

   			total_read = total_read + iost[i].ior;
   			total_riops = total_riops + iost[i].iops_calcul;
   			nbcli++;
  		}
 	}
 }

 i=0;
 
 qsort(iost,nbenrg,sizeof(io_ost),compareiow);

 if ( iost[0].iow > 0 )
 {
 	printf("\n\nWrite\n");
 	printf("OST \t\t\tCLIENT \t\t\t\tMB_write \t\t\tIOPS\n");
 	printf("------------------------------------------------------------------------------------\n");
 	for(i=0; i < nbaffich ;i++)
 	{
  		if(iost[i].iow > 0)
  		{
   			inet_aton(iost[i].nomhost, &ip_addr);

			host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);

			if (host != NULL)
    				strcpy(iost[i].nomhost,host->h_name);

			printf("%s\t%16s\t\t\t%ld\t\t\t\t%ld\n", iost[i].ost_name, iost[i].nomhost, iost[i].iow, iost[i].iops_calcul);


   			total_write=total_write+iost[i].iow;
   			total_wiops=total_wiops + iost[i].iops_calcul;
   			nbcli++;
  		}
 	}
 }

 i=0;

 qsort(iost,nbenrg,sizeof(io_ost),compareiops);

 if (iost[0].iops_calcul > 0)
 {
 	printf("\n\nIOPS\n");
 	printf("OST \t\t\tCLIENT \t\t\t\tMB_read\t\tMB_write \tIOPS\n");
 	printf("------------------------------------------------------------------------------------\n");
 	for(i=0; i < nbaffich ;i++)
 	{
  		if(iost[i].iops_calcul > 0)
  		{
   			inet_aton(iost[i].nomhost, &ip_addr);

   			host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);

   			if (host != NULL)
    				strcpy(iost[i].nomhost,host->h_name);

        		printf("%s\t%16s\t\t\t%ld\t\t%ld\t\t%ld\n", iost[i].ost_name, iost[i].nomhost,  iost[i].ior, iost[i].iow, iost[i].iops_calcul);

			nbcli++;
  		}
 	} 
 }

 i=0;

 qsort(iost,nbenrg,sizeof(io_ost),comparequota);

 if (iost[0].io_quota > 0)
 {
 	printf("\n\nQUOTACTL\n");
 	printf("OST \t\t\tCLIENT \t\t\t\tquotactl\n");
 	printf("------------------------------------------------------------------------------------\n");
 	for(i=0; i < nbaffich ;i++)
 	{
  		if(iost[i].io_quota > 0)
  		{
        		inet_aton(iost[i].nomhost, &ip_addr);

        		host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);

        		if (host != NULL)
                		strcpy(iost[i].nomhost,host->h_name);

        		printf("%s\t%16s\t\t\t%ld\n", iost[i].ost_name, iost[i].nomhost,  iost[i].io_quota);

        		nbcli++;
  		}
 	}
 }

 i=0;

 qsort(iost,nbenrg,sizeof(io_ost),comparequota);

 if (iost[0].count_punch > 0)
 {
        printf("\n\nPUNCH\n");
        printf("OST \t\t\tCLIENT \t\t\t\tpunch\n");
        printf("------------------------------------------------------------------------------------\n");
        for(i=0; i < nbaffich ;i++)
        {
                if(iost[i].count_punch > 0)
                {
                        inet_aton(iost[i].nomhost, &ip_addr);

                        host = gethostbyaddr(&ip_addr, sizeof(ip_addr), AF_INET);

                        if (host != NULL)
                                strcpy(iost[i].nomhost,host->h_name);

                        printf("%s\t%16s\t\t\t%ld\n", iost[i].ost_name, iost[i].nomhost,  iost[i].count_punch);

                        nbcli++;
                }
        }
 }

 printf("\nLoad : %.2f %.2f %.2f\n",load1,load5,load15);
 printf ("\nPendant l'intervale de 10s :\n");
 printf("------------------------------------------------------------------------------------\n");
 printf ("TOTAL MB READ  =\t\t%ld\n",total_read);
 printf ("TOTAL MB WRITE =\t\t%ld\n",total_write);
 printf ("TOTAL IOPS READ = \t\t%ld\n",total_riops);
 printf ("TOTAL IOPS WRITE =\t\t%ld\n\n",total_wiops);
}
