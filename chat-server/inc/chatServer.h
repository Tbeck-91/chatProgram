/*
* FILE : chatServer.h
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 22 / 04 / 2018
* DESCRIPTION : All of the includes, definitions
*		and prototypes used in the chat server program.
*/


#define BACKLOG 5
#define MAX_CLIENTS 10

int createSocket(int port); 
void* clientHandler (void* shmID);
int initShMem(int shmVal,  SERVER_DATA** srvrInfo);
