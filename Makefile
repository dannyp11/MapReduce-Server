# Config build structure ##########################################
BINABC = serverABC_exe
BINAWS = serveraws_exe
BINCLIENT = client

SERVER_CLASS_OBJS = server_class.o common.o
SERVERABC_OBJS = $(SERVER_CLASS_OBJS) serverABC.o backend_server.o
SERVERAWS_OBJS = $(SERVER_CLASS_OBJS) aws.o aws_server.o
CLIENT_OBJS = client.o common.o
# Config build structure end ######################################

# Compiler flags -------------------------------------------------
CC =g++
DEBUG = -g
CFLAGS = -c -Wall -Werror $(DEBUG)
IFLAGS = -I. 
LDFLAGS = -pthread
ARCHFLAGS = 
# Compiler flags ends ---------------------------------------------
.PHONY: all

all: Scripts $(BINABC) $(BINAWS) $(BINCLIENT)

Scripts:

clean:
	rm -rf *.o *.so 
	rm -rf $(OBJDIR)
	rm -f $(BINABC) $(BINAWS)
	rm -f $(BINCLIENT)

serverA: $(BINABC)
	./$(BINABC) -n A

serverB: $(BINABC)
	./$(BINABC) -n B

serverC: $(BINABC)
	./$(BINABC) -n C

aws: $(BINAWS)
	./$(BINAWS)

# Build code #######################################
%.o:%.cpp
	$(CC) $(CFLAGS) $(IFLAGS) $(ARCHFLAGS) $< -o $@

$(BINABC): $(SERVERABC_OBJS)
	$(CC) $(LDFLAGS) $(SERVERABC_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINAWS): $(SERVERAWS_OBJS)
	$(CC) $(LDFLAGS) $(SERVERAWS_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINCLIENT): $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $(CLIENT_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

