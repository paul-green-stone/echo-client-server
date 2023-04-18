cc = gcc
objs = temp/main.o ../helper.o
dirs = temp
CFLAGS =

main: $(objs) 
	$(cc) $(CFLAGS) -o client.out $(objs)

temp/main.o: main.c
	$(cc) -c -g -o temp/main.o main.c

.PHONY: clean

clean:
	rm -rf temp ./*.o client.out 

$(shell mkdir -p $(dirs))