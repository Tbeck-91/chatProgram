/*
* FILE : chatServer.h
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 08 / 04 / 2018
* DESCRIPTION :
*
*
*
*/



#define BACKLOG 5
#define MAX_CLIENTS 10

//int createSocket(int port) 
//int getClient(int sockFD, struct sockaddr_in* clientAddr) 
void* clientHandler (void* clSock);
