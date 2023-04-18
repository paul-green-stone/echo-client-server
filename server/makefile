cc = gcc
objs = temp/main.o temp/server.o ../helper.o
dirs = temp
CFLAGS =

main: $(objs) 
	$(cc) $(CFLAGS) -o server.out $(objs)

temp/main.o: main.c
	$(cc) -c -g -o temp/main.o main.c

temp/server.o: server.h server.c
	$(cc) -c -g -o temp/server.o server.c

.PHONY: clean

clean:
	rm -rf temp ./*.o server.out 

$(shell mkdir -p $(dirs))