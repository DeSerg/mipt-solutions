СС	= gcc
LINK	= g++
LIBS	= -lpthread -lrt
FLAGS	= -std=c99
DESTDIR	= 
TARGET	= Client \
	  Server

all: Client Server

Server: messages.o server.o
	$(LINK) messages.o server.o $(FLAGS) -o Server $(LIBS) 

Client: client.o messages.o
	$(LINK) messages.o client.o $(FLAGS) -o Client $(LIBS) 

messages.o: messages.c messages.h
	$(CC) -c $(FLAGS) -o messages.o messages.c

server.o: server.c server.h
	$(CC) -c $(FLAGS) -o server.o server.c

client.o: client.c 
	$(CC) -c $(FLAGS) -o client.o client.c
