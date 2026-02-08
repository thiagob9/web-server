server: 
	gcc -c -o server.o server.c
	gcc -o server server.o
	rm server.o