/*
* FILE : chatClient.c
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 22 / 04 / 2018
* DESCRIPTION : The main file of the chat client program.
*               The program gathers information from the client 
*		and the connects to server and launches the curses UI.
*	        Once the client is setup and connected to the server it launches
*		threads to handle incoming and outgoing communication.
*/


#include "../inc/chatClient.h"


int main (int argc, char** argv) {
		
	struct hostent*    host;
	struct sockaddr_in server_addr;
	int srvrSock;
	int retVal;
	pthread_t tid; // thread id, gets thrown away since not needed
	
	// no cl args
	if (argc != 2) {
		printf("Usage: chat-client <server-addr>\n");
		return -1;
	}
	
	// resolve hostname (or ipv4 addr) into canonical ip
	if ((host = gethostbyname(argv[1])) == NULL) {
		printf("Failed to to get host name! \n");
		return -1;		
	}	

	/* Pack struct with server data */	 	
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));
	server_addr.sin_port = htons (PORT);

	// establish a socket FD for remote communications
	if ((srvrSock = socket (AF_INET, SOCK_STREAM, 0)) < 0)  {
		printf ("[CLIENT] : Getting Client Socket - FAILED\n");
       		return -1;
     	}	

	// connect to server
	if (connect (srvrSock, (struct sockaddr *)&server_addr,sizeof (server_addr)) < 0) {
		printf ("[CLIENT] : Connect to Server - FAILED\n");
		close (srvrSock);
		return -1;
	}


	/* Get client Info */
	setupClient(&clientInfo);	

	// setup curses and windows	
	initCurses();
	setUpWindows(&inputW, &historyW);
	update(inputW, historyW);

	// the client needs to read it's ip address from the server
	// apparently the server needs to tell the client who they are! 
	recv (srvrSock, (void*)&(clientInfo.ipAddr), sizeof (struct in_addr), 0);
	
	// Finally, launch our threads! 
	pthread_create (&tid, NULL, handleOutgoing, (void*)&srvrSock);	
	pthread_create (&tid, NULL, handleIncoming, (void*)&srvrSock);
	
	// wait for thread to "rejoin". In reality, one of the threads will kill the process
	pthread_join(tid, (void *)(&retVal));	

	return 0;
}





