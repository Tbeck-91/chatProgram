/*
* FILE : chatProgram.h
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 08 / 04 / 2018
* DESCRIPTION : All of the common includes and definitions
*		shared between the applications of the chat program.
*
*
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>


#define PORT 5000
#define ERR -1
#define USRNAME_LEN 5
#define MSG_LEN 80
#define PARCEL_LEN 40
#define bool int
#define false 0
#define true 1
#define MAX_CLIENTS 10
#define SPACE 32

typedef struct {

	char username[USRNAME_LEN + 1];
	struct in_addr ipAddr;
	char colour;
	int cID; // client ID

} CLIENT;

typedef struct {

	char msg[MSG_LEN];
	bool finalmsg; 
	int cID;
	
} MSG;





