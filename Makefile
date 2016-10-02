CC =g++

DEBUG = -g
CFLAGS = -c -Wall -Werror $(DEBUG) -std=c++11
IFLAGS = -I. 
LDFLAGS = -pthread
ARCHFLAGS = 

all: Scripts BUILD_SERVER BUILD_CLIENT

Scripts:
	

clean:
	rm -rf *.o *.so 
	rm $(BIN1) $(BIN2)

# Config build structure ##########################################
BIN1 = server
BIN2 = client
# Config build structure end ######################################

# Build code #######################################
BUILD_SERVER: server.o common.o
	$(CC) $(LDFLAGS) -o $(BIN1) server.o common.o

BUILD_CLIENT: client.o common.o 
	$(CC) $(LDFLAGS) -o $(BIN2) client.o common.o

server.o: server.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c server.cpp

client.o: client.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c client.cpp

common.o: common.cpp
	$(CC) $(CFLAGS) $(IFLAGS) -c common.cpp
