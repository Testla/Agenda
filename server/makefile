CC=g++
CCFLAGS=-Wall -std=c++0x -g
OBJECTS = server.o  AgendaUI.o AgendaService.o Storage.o Meeting.o Date.o User.o

AgendaServer.out : $(OBJECTS)
	$(CC) $(CCFLAGS) -o AgendaServer.out $(OBJECTS) -lpthread
server.o : AgendaUI.o AgendaService.o Storage.o Meeting.o Date.o User.o
	$(CC) $(CCFLAGS) -c server.cpp -lpthread
AgendaUI.o : AgendaService.cpp Storage.cpp User.cpp Meeting.cpp Date.cpp
	$(CC) $(CCFLAGS) -c AgendaUI.cpp
AgendaService.o : Storage.cpp User.cpp Meeting.cpp Date.cpp
	$(CC) $(CCFLAGS) -c AgendaService.cpp
Storage.o : User.cpp Meeting.cpp Date.cpp
	$(CC) $(CCFLAGS) -c Storage.cpp
Meeting.o : Date.cpp
	$(CC) $(CCFLAGS) -c Meeting.cpp
Date.o :
	$(CC) $(CCFLAGS) -c Date.cpp
User.o :
	$(CC) $(CCFLAGS) -c User.cpp

.PHONY clean :
	-rm $(OBJECTS)
