#	FILE		: makefile
#	
#	PROJECT		:
#
#	PROGRAMMER	: Taylor Beck
#	
#	FIRST VERSION	:  /  / 18
#
#       DESCRIPTION	: Makefile Template

EXE = bin/chat-server
STD_HEADERS =  ../common/inc/chatProgram.h ./inc/chatServer.h
CC = cc
LD = cc
CFLAGS = -c -o
LDFLAGS = -lpthread -o
SRC = ./src/chatServer.c ./src/serverUtilities.c
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC)) 
RM = rm

$(EXE): $(OBJ) $(STD_HEADERS)
	$(LD) $(OBJ) $(LDFLAGS)$@

./obj/%.o:  src/%.c $(STD_HEADERS)
	$(CC) $< $(CFLAGS)$@

clean:
	$(RM) obj/*.o
	$(RM) $(EXE)	
