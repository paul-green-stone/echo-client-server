cc = gcc
objs = temp/main.o temp/client.o ../helper.o
dirs = temp
CFLAGS =

main: $(objs) 
	$(cc) $(CFLAGS) -o client.out $(objs)

temp/main.o: main.c
	$(cc) -c -g -o temp/main.o main.c

temp/client.o: client.h client.c
	$(cc) -c -g -o temp/client.o client.c

.PHONY: clean

clean:
	rm -rf temp ./*.o client.out 

$(shell mkdir -p $(dirs))