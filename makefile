compile: 
	gcc -Wall queue.c hw.c -o hw

run: compile
	./hw 3
