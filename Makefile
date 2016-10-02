# Config build structure ##########################################
BINSERVER = server
BINA = serverA_exe
BINB = serverB_exe
BINC = serverC_exe
BINAWS = serveraws_exe
BINCLIENT = client

SERVER_CLASS_OBJS = server_class.o common.o
SERVER_OBJS = $(SERVER_CLASS_OBJS) server.o
SERVERA_OBJS = $(SERVER_CLASS_OBJS) serverA.o backend_server.o
SERVERB_OBJS = $(SERVER_CLASS_OBJS) serverB.o backend_server.o
SERVERC_OBJS = $(SERVER_CLASS_OBJS) serverC.o backend_server.o
SERVERCAWS_OBJS = $(SERVER_CLASS_OBJS) serveraws.o
CLIENT_OBJS = 
# Config build structure end ######################################

# Compiler flags -------------------------------------------------
CC =g++
DEBUG = -g
CFLAGS = -c -Wall -Werror $(DEBUG)
IFLAGS = -I. 
LDFLAGS = -pthread
ARCHFLAGS = 
# Compiler flags ends ---------------------------------------------
#.PHONY: serverA

all: Scripts $(BINA)

Scripts:

clean:
	rm -rf *.o *.so 
	rm -rf $(OBJDIR)
	rm -f $(BINSERVER) $(BINA) $(BINB) $(BINC) $(BINAWS)
	rm -f $(BINCLIENT)

serverA: $(BINA)
	./$(BINA)

serverB: $(BINB)
	./$(BINB)

serverC: $(BINC)
	./$(BINC)

serveraws: $(BINAWS)
	./$(BINAWS)

# Build code #######################################
%.o:%.cpp
	$(CC) $(CFLAGS) $(IFLAGS) $(ARCHFLAGS) $< -o $@

$(BINSERVER): $(SERVER_OBJS)
	$(CC) $(LDFLAGS) $(SERVER_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINA): $(SERVERA_OBJS)
	$(CC) $(LDFLAGS) $(SERVERA_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINB): $(SERVERB_OBJS)
	$(CC) $(LDFLAGS) $(SERVERB_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINC): $(SERVERC_OBJS)
	$(CC) $(LDFLAGS) $(SERVERC_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINAWS): $(SERVERAWS_OBJS)
	$(CC) $(LDFLAGS) $(SERVERAWS_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@

$(BINCLIENT): $(CLIENT_OBJS)
	$(CC) $(LDFLAGS) $(CLIENT_OBJS) $(IFLAGS) $(ARCHFLAGS) -o $@
