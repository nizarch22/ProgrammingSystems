exception = !(*.c|*.h|"Makefile"|"test.txt")
hw2: dispatcher.o 
	gcc dispatcher.o -o hw2
dispatcher.o: dispatcher.c
	gcc -c dispatcher.c	
clean: 
	rm *.o hw2 count*.txt thread*.txt stats.txt
run: test.txt
	./hw2 test.txt 5 5 1
