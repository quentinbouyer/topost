#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAILLE_BLOC 1024

char **transforme_texte (const char *str, const char *separator)
{
 char **a = NULL;
 int a_size = 0;
 int a_len = 0;

 int i;
 int separator_len = strlen (separator);
 const char *p0;
 char *p1;

 // Initialisations

 if (NULL == (a = malloc (TAILLE_BLOC * sizeof *a)))
	return (NULL);
 a_size = TAILLE_BLOC; 
 a_len = 0;
 p0 = str;

 // recup des sous chaines
 while (p0 && (p1 = strstr (p0, separator)))
 {
 	if (a_len >= (a_size+2))
	{
		a_size += TAILLE_BLOC;
		if (NULL == (a=realloc (a, a_size * sizeof * a)))
			return (NULL);
	}
	if ( NULL == (a[a_len] = calloc (p1 - p0 +1, sizeof **a)))
	{
		for (i=0; i < a_len; i++)
			free (a[i]);
		free (a);
		return (NULL);
	}
	memcpy (a[a_len], p0, p1 - p0);
	p0 = p1 + separator_len;
	a_len++;
 }

 if (NULL == (a[a_len] = strdup (p0)))
 {
	for (i=0; i < a_len; i++)
		free (a[i]);
	free (a);
	return (NULL);
 }
 a_len++;
 a[a_len] = NULL;

 return (a);
}
