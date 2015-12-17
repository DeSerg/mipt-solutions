CC=mpic++
CFLAGS=-lpthread -std=c++11
#LDFLAGS=
SOURCES=master.cpp worker.cpp support.cpp m_help.cpp m_quit.cpp m_run.cpp m_start.cpp m_status.cpp m_stop.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hello

all:
	$(CC) $(CFLAGS) $(SOURCES) -o life_the_game

#master:
#	$(CC) $(CFLAGS) master.cpp -o master.o 

#worker: master
#	$(CC) $(CFLAGS) worker.cpp -o worker.o

#support: master
#	$(CC) $(CFLAGS) support.cpp -o support.o

#methods: master
#	$(CC) $(CFLAGS) m_help.cpp m_quit.cpp m_run.cpp m_start.cpp m_status.cpp m_stop.cpp -o methods.o

