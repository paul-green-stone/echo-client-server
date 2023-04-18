cc = gcc

helper.o: helper.h helper.c
	$(cc) -c -g -o helper.o helper.c