hw2: dispatcher.o 
	gcc dispatcher.o -o hw2
dispatcher.o: dispatcher.c
	gcc -c dispatcher.c	
clean:
	rm -v !(*.c|"Makefile"|*.o|"test.txt")
run: test.txt
	./hw2 test.txt 5 5 1
