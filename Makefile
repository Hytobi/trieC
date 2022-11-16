all: exec run clean

exec: main.o
	gcc -o exec main.c

run: exec
	./exec

clean:
	rm -f *.o exec