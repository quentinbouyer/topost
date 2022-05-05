CC=gcc
CFLAGS=-g -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -lglib-2.0
topost_EXEC=topost
topost: compte_ost.o affiche_oss.o li_stat.o initialize_oss.o traite_arg.o read_conf_file.o topost.o
	$(CC) $(CFLAGS) -o $(topost_EXEC) compte_ost.o affiche_oss.o li_stat.o initialize_oss.o traite_arg.o read_conf_file.o topost.o
