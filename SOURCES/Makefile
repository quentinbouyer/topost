CC=gcc
CFLAGS=-O2 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -lglib-2.0
topost_EXEC=topost
topost: compte_ost.o affiche_oss.o li_stat.o initialize_oss.o parse_arg.o topost.o compte_nid.o rempli_structure.o tranforme_texte.o
	$(CC) $(CFLAGS) -o $(topost_EXEC) compte_ost.o affiche_oss.o li_stat.o initialize_oss.o parse_arg.o compte_nid.o rempli_structure.o tranforme_texte.o topost.o 
