shell.o : shell.c
	gcc -c -o shell.o shell.c -w -I ./shell.h
shell.bin : shell.o
	gcc -o shell shell.o -lreadline
all : shell.bin
	@echo "compile done"
clean : 
	rm *.o
	