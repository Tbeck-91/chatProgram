#	FILE		: makefile
#	
#	PROJECT		:
#
#	PROGRAMMER	: Taylor Beck
#	
#	FIRST VERSION	:  /  / 18
#
#       DESCRIPTION	: Makefile Template

EXE = bin/chat-client
STD_HEADERS =  ../common/inc/chatProgram.h ./inc/chatClient.h
CC = cc
LD = cc
CFLAGS = -c  -o
LDFLAGS = -lpthread -o
SRC = src/chatClient.c src/clientUtilities.c
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC)) 
RM = rm

$(EXE): $(OBJ) $(STD_HEADERS)
	$(LD) $(OBJ) $(LDFLAGS)$@ -lncurses

./obj/%.o:  src/%.c $(STD_HEADERS)
	$(CC) $< $(CFLAGS)$@ 

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)	
