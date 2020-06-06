#lubraries
CONTAINERS 	= ../lib_containers
UTILITIES 	= ../lib_utilities
SYSTEM		= ../lib_system

#main macros
SERVER		= dropbox_server
SERVER_O	= dropbox_server.o
CLIENT		= dropbox_client
CLIENT_O	= dropbox_client.o

ENVIROMENT = ./environment

#scources
OBJS = dropbox_protocol.o
LIBS = $(SYSTEM)/thread.o  $(SYSTEM)/simple_poll.o  $(SYSTEM)/socket.o	$(SYSTEM)/path.o $(SYSTEM)/directory.o $(SYSTEM)/file_io.o  $(CONTAINERS)/string.o	$(SYSTEM)/system.o

#compliler macros
CC		= g++
FLAGS   = -std=c++11 -c -Wno-write-strings -Wno-reorder


#targets
all: $(SERVER) $(CLIENT)

dropbox_client: $(CLIENT_O) $(LIBS) $(OBJS)
	$(CC) -g $(CLIENT_O) $(LIBS) $(OBJS) -o $(ENVIROMENT)/$@ -lpthread

dropbox_server: $(SERVER_O) $(LIBS) $(OBJS)
	$(CC) -g $(SERVER_O) $(LIBS) $(OBJS) -o $(ENVIROMENT)/$@ -lpthread

#mains
dropbox_server.o: dropbox_server.cpp
	$(CC) $(FLAGS) dropbox_server.cpp

dropbox_client.o: dropbox_client.cpp
	$(CC) $(FLAGS) dropbox_client.cpp


#threads
$(SYSTEM)/thread.o: $(SYSTEM)/thread.cpp
	$(CC) $(FLAGS) $(SYSTEM)/thread.cpp -o $(SYSTEM)/thread.o

#network
dropbox_protocol.o: dropbox_protocol.cpp
	$(CC) $(FLAGS) dropbox_protocol.cpp

$(SYSTEM)/socket.o: $(SYSTEM)/socket.cpp
	$(CC) $(FLAGS) $(SYSTEM)/socket.cpp -o $(SYSTEM)/socket.o

#Utilities
$(SYSTEM)/simple_poll.o: $(SYSTEM)/simple_poll.cpp
	$(CC) $(FLAGS) $(SYSTEM)/simple_poll.cpp -o $(SYSTEM)/simple_poll.o

$(SYSTEM)/system.o: $(SYSTEM)/system.cpp
	$(CC) $(FLAGS) $(SYSTEM)/system.cpp -o $(SYSTEM)/system.o

$(SYSTEM)/path.o: $(SYSTEM)/path.cpp
	$(CC) $(FLAGS) $(SYSTEM)/path.cpp -o $(SYSTEM)/path.o

$(SYSTEM)/directory.o: $(SYSTEM)/directory.cpp
	$(CC) $(FLAGS) $(SYSTEM)/directory.cpp -o $(SYSTEM)/directory.o

string.o: string.cpp
	$(CC) $(FLAGS) string.cpp
	
$(SYSTEM)/file_io.o: $(SYSTEM)/file_io.cpp
	$(CC) $(FLAGS) $(SYSTEM)/file_io.cpp -o $(SYSTEM)/file_io.o

#housekeeping
clean:
	rm -i ./*.o $(CONTAINERS)/*.o $(UTILITIES)/*.o $(SYSTEM)/*.o $(ENVIROMENT)/$(SERVER) $(ENVIROMENT)/$(CLIENT)
