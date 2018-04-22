#	FILE		: makefile
#	
#	PROJECT		: Systems Programming - Assignment 4
#
#	PROGRAMMER	: Taylor Beck
#	
#	FIRST VERSION	: 22 / 04 / 18
#
#       DESCRIPTION	: Top level makefile for chat program

STD_INC = common/inc/chatProgram.h
EXECUTABLES = chat-server/bin/chat-server chat-client/bin/chat-client

all: SERVER CLIENT
	cp $(EXECUTABLES) common/bin

SERVER: chat-server/src/*.c chat-server/inc/*.h $(STD_INC)
	make -C chat-server

CLIENT: chat-client/src/*.c chat-client/inc/*.h $(STD_INC)
	make -C chat-client

clean:
	rm common/bin/chat-client
	rm common/bin/chat-server
	rm $(EXECUTABLES)
	rm chat-server/obj/*.o
	rm chat-client/obj/*.o

	
