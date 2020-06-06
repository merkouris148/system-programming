#libraries
CONTAINERS 	= ../lib_containers
UTILITIES 	= ../lib_utilities
SYSTEM		= ../lib_system

#main macros
MAIN_OBJ	= bitcoin.o

#scources
OBJS 		= bitcoin_app_helper.o  bitcoin_app.o  transactions.o	
LIBS		= $(CONTAINERS)/string.o  $(UTILITIES)/time.o  $(SYSTEM)/file_io.o

#compliler macros
OUT  	= bitcoin
CC		= g++
FLAGS   = -std=c++11 -c -Wno-write-strings -Wno-reorder


#targets
$(OUT): $(MAIN_OBJ) $(OBJS) $(LIBS)
	$(CC) -g $(MAIN_OBJ) $(OBJS) $(LIBS) -o $@ -lm


#mains 
bitcoin.o: bitcoin.cpp
	$(CC) $(FLAGS) bitcoin.cpp

bitcoin_app.o: bitcoin_app.cpp
	$(CC) $(FLAGS) bitcoin_app.cpp

#modules
transactions.o: transactions.cpp
	$(CC) $(FLAGS) transactions.cpp

#Utilities
$(CONTAINERS)/string.o: $(CONTAINERS)/string.cpp
	$(CC) $(FLAGS) $(CONTAINERS)/string.cpp -o $(CONTAINERS)/string.o

$(UTILITIES)/time.o: $(UTILITIES)/time.cpp
	$(CC) $(FLAGS) $(UTILITIES)/time.cpp -o $(UTILITIES)/time.o
	
$(SYSTEM)/file_io.o: $(SYSTEM)/file_io.cpp
	$(CC) $(FLAGS) $(SYSTEM)/file_io.cpp -o $(SYSTEM)/file_io.o

bicoin_app_helper.o: bitcoin_app_helper.cpp
	$(CC) $(FLAGS) bitcoin_app_helper.cpp

# housekeeping
clean:
	rm -i ./*.o $(CONTAINERS)/*.o $(UTILITIES)/*.o $(SYSTEM)/*.o $(OUT)
