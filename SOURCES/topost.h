typedef struct
{
 char ost_name[32];
 char path[256];
 char addip[32];
 char nomhost[16];
 unsigned long read;
 unsigned long write;
 unsigned long iops_lu;
 unsigned long ior;
 unsigned long iow;
 unsigned long iops_calcul;
} io_ost;

typedef struct
{
 char nomcli[16];
 char ipcli[32];
} clitemp;
