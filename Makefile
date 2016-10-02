# Config build structure ##########################################
BIN_DIR = bin/

BIN1 = $(BIN_DIR)server
BIN2 = $(BIN_DIR)client
# Config build structure end ######################################

# Compiler flags -------------------------------------------------
CC =g++

DEBUG = -g
CFLAGS = -c -Wall -Werror $(DEBUG)
IFLAGS = -I. 
LDFLAGS = -pthread
ARCHFLAGS = 
# Compiler flags ends ---------------------------------------------

all: Scripts BUILD_SERVER BUILD_CLIENT

Scripts:
	mkdir -p $(BIN_DIR)

clean:
	rm -rf *.o *.so 
	rm $(BIN1) $(BIN2)
	rm -r $(BIN_DIR)

# Build code #######################################
BUILD_SERVER: server.o common.o
	$(CC) $(LDFLAGS) -o $(BIN1) server.o common.o

BUILD_CLIENT: client.o common.o 
	$(CC) $(LDFLAGS) -o $(BIN2) client.o common.o

server.o: server.cpp
	$(CC) $(CFLAGS) $(IFLAGS) server.cpp

client.o: client.cpp
	$(CC) $(CFLAGS) $(IFLAGS) client.cpp

common.o: common.cpp
	$(CC) $(CFLAGS) $(IFLAGS) common.cpp
