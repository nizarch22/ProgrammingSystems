hw5 : hw5.o
	gcc -o hw5 hw5.o
hw5.o : hw5.c
	gcc -c hw5.c -o hw5.o
clean:
	rm *.o hw5
run:
	./hw5 floppy.img dir
slide:
	./hw5 test2.img dir
copy:
	./hw5 floppy.img cp fs.c TESTCPY.c
