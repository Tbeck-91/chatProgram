/*
* FILE : chatServer.c
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 22 / 04 / 2018
* DESCRIPTION :
*
* The chat server gains knowledge of the
* clients IP address, through the accept call,
* which it then sends back to client. Each message
* from the client to the server is a CLIENT struct
* which contains all of the information necessary for
* the other clients to know whom they are speaking to. 
*
* Thus, all the server needs to do is read this struct from the client
* and then send it to all the other clients connected to the server
* who will then parse it on their end and display the contents (ip, username, message) in their message window.
*
* when the server receives the bye message, it decrements the number of connected clients and kills the thread.
* The last thread to exit is responsible for cleaning up and shutting down the server. It will close the client
* and server sockets, free the shared memory, and then send a SIG_TERM to the thread and the process, killing them both.
*
*
* To keep track of everything, the server keeps a SERVER_DATA struct in shared memory (see chatProgram.h), which 
* has fields for the current number of clients, an int array of client socket file descriptor handles, the socket
* file descriptor for the server, and the ip of the last client accepted. This shared memory is critical so the 
* threads can broadcast information to the other threads clients. This is also how the threads learn of the clients
* ip address. 
*
*/

#include "../../common/inc/chatProgram.h"
#include "../inc/chatServer.h"

int main(void) {

	int clSock, retval, client_len;
	struct sockaddr_in client_addr, server_addr;
	pthread_t tid[MAX_CLIENTS];
	int shmID; // shared memory id will be passed to clients
	SERVER_DATA* srvrInfo; // will point at shared memory location

	// attempt to create shared memory 
	if ( (shmID = initShMem(SHM_VAL, &srvrInfo)) == -1) {

		printf("Could not create shared memory! Server Exiting! \n");
		return -1;
	}

	if ((srvrInfo->srvrSock = createSocket(PORT)) == -1) {
		printf("Server could not create socket. exiting. \n");
		return -1;
	}

	// bind socket to port and begin listening
	client_len = sizeof (client_addr);
	bind (srvrInfo->srvrSock, (struct sockaddr *)&server_addr, sizeof (server_addr));
	listen (srvrInfo->srvrSock, 5);
	
	srvrInfo->numClients = 0;
	// server will loop until the last client sends it a signal
	while (true) {
		
		if (srvrInfo->numClients < MAX_CLIENTS) {
			
			// accept new client	
			srvrInfo->clSock = accept (srvrInfo->srvrSock,(struct sockaddr *)&client_addr, &client_len); 
			srvrInfo->clientSocks[srvrInfo->numClients++] = srvrInfo->clSock;
			srvrInfo->clientIP = client_addr.sin_addr;
								
			// launch thread to handle our new client
			pthread_create(&(tid[(srvrInfo->numClients-1)]), NULL, clientHandler, (void *)&shmID);
		}
	}	
	return 0;
}

