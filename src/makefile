CC=g++
CCFLAGS=-Wall -std=c++0x
OBJECTS = Agenda.o AgendaUI.o AgendaService.o Storage.o Meeting.o Date.o User.o

Agenda.out : $(OBJECTS)
	$(CC) $(CCFLAGS) -o Agenda.out $(OBJECTS)
Agenda.o : AgendaUI.cpp AgendaService.cpp Storage.cpp User.cpp Meeting.cpp \
Date.cpp
	$(CC) $(CCFLAGS) -c Agenda.cpp
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
