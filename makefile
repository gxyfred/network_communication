all:	client.o	server_or.c	server_and.c	edge.c
	cc	-o	client	client.o
	cc	server_or.c	-o	server_or.out
	cc	server_and.c	-o	server_and.out
	cc	edge.c	-o	edge.out
client.o:client.c
	cc	-c	client.c
server_or:
	./server_or.out
server_and:
	./server_and.out
edge:
	./edge.out
.PHONY:	clean
clean:
	rm	client	client.o	server_or.out	server_and.out	edge.out
