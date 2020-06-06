#lubraries
CONTAINERS 	= ../lib_containers
UTILITIES 	= ../lib_utilities
SYSTEM		= ../lib_system

#main macros
SENTER			= sender
SENTER_O		= senter.o

RECEIVER		= receiver
RECEIVER_O		= receiver.o

MIRROR_CLIENT	= mirror_client
MIRROR_CLIENT_O	= mirror_client_main.o

#enviroment
ENVIROMENT	= ./environment

#scources
OBJS =	senter_receiver.o mirror_client.o
LIBS = $(SYSTEM)/system.o  $(SYSTEM)/spawn_child_process.o  $(SYSTEM)/path.o  $(SYSTEM)/pipes.o  $(SYSTEM)/directory.o	 $(SYSTEM)/file_io.o  $(UTILITIES)/time.o  $(CONTAINERS)/string.o

#compliler macros
CC		= g++
FLAGS   = -std=c++11 -c -Wno-write-strings -Wno-reorder


#targets
all: $(ENVIROMENT)/$(SENTER) $(ENVIROMENT)/$(RECEIVER) $(ENVIROMENT)/$(MIRROR_CLIENT)

$(ENVIROMENT)/senter: $(SENTER_O) $(LIBS) $(OBJS)
	$(CC) -g $(SENTER_O) $(OBJS) $(LIBS) -o $(ENVIROMENT)/$@

$(ENVIROMENT)/receiver: $(RECEIVER_O) $(LIBS) $(OBJS)
	$(CC) -g $(RECEIVER_O) $(OBJS) $(LIBS) -o $(ENVIROMENT)/$@

$(ENVIROMENT)/mirror_client: $(MIRROR_CLIENT_O) $(LIBS) $(OBJS)
	$(CC) -g $(MIRROR_CLIENT_O) $(OBJS) $(LIBS) -o $(ENVIROMENT)/$@

#mains
senter.o: senter.cpp
	$(CC) $(FLAGS) senter.cpp

receiver.o: receiver.cpp
	$(CC) $(FLAGS) receiver.cpp

mirror_client_main.o: mirror_client_main.cpp
	$(CC) $(FLAGS) mirror_client_main.cpp

#modules
senter_receiver.o: senter_receiver.cpp
	$(CC) $(FLAGS) senter_receiver.cpp

mirror_client.o: mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp

#Utilities
$(SYSTEM)/system.o: $(SYSTEM)/system.cpp
	$(CC) $(FLAGS) $(SYSTEM)/system.cpp -o $(SYSTEM)/system.o

$(SYSTEM)/spawn_child_process.o: $(SYSTEM)/spawn_child_process.cpp
	$(CC) $(FLAGS) $(SYSTEM)/spawn_child_process.cpp -o $(SYSTEM)/spawn_child_process.o

$(SYSTEM)/path.o: $(SYSTEM)/path.cpp
	$(CC) $(FLAGS) $(SYSTEM)/path.cpp -o $(SYSTEM)/path.o

$(SYSTEM)/directory.o: $(SYSTEM)/directory.cpp
	$(CC) $(FLAGS) $(SYSTEM)/directory.cpp -o $(SYSTEM)/directory.o

$(SYSTEM)/pipes.o: $(SYSTEM)/pipes.cpp
	$(CC) $(FLAGS) $(SYSTEM)/pipes.cpp -o $(SYSTEM)/pipes.o

$(CONTAINERS)/string.o: $(CONTAINERS)/string.cpp
	$(CC) $(FLAGS) $(CONTAINERS)/string.cpp -o $(CONTAINERS)/string.o

$(CONTAINERS)/time.o: $(UTILITIES)/time.cpp
	$(CC) $(FLAGS) $(UTILITIES)/time.cpp -o $(UTILITIES)/time.o
	
$(SYSTEM)/file_io.o: $(SYSTEM)/file_io.cpp
	$(CC) $(FLAGS) $(SYSTEM)/file_io.cpp -o $(SYSTEM)/file_io.o

#housekeeping
clean:
	rm -i ./*.o $(CONTAINERS)/*.o $(UTILITIES)/*.o $(SYSTEM)/*.o $(ENVIROMENT)/$(SENTER) $(ENVIROMENT)/$(RECEIVER) $(ENVIROMENT)/$(MIRROR_CLIENT)
