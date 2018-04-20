/*
* FILE : serverUtilities.c
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 08 / 04 / 2018
* DESCRIPTION : Contains the problem domain of the chat server application.
*		That is to say, all of the necessary functions to 
*		create and maintain the chat server are found in this
*		file.
*/

#include "../inc/chatProgram.h"
#include "../inc/chatServer.h"

/*
	FUNCTION: createSocket
	DESCRIPTION: Creates communications socket and
		     returns the socket file descriptor
	PARAMETERS: int port: the port to bind the socket to.
	returns: int retVal: will be -1 on error, otherwise the socket FILE DESCRIPTOR

*/
int createSocket(int port) {

	struct sockaddr_in serverData;
	int socketFD; // socket file descriptors
	int retVal; // set to error if api calls fail else the socket fd #

	// open an internet stream socket under tcp/ip
	retVal = socketFD = socket (AF_INET, SOCK_STREAM, 0);

	if (retVal != ERR) {

		// pack our socket struct
		memset (&serverData, 0, sizeof (struct sockaddr_in));
		serverData.sin_family = AF_INET;
		serverData.sin_addr.s_addr = (INADDR_ANY);
		serverData.sin_port = htons(port);
	
		// bind socket to our port
		retVal = bind (socketFD, (struct sockaddr*)&serverData, sizeof (struct sockaddr_in));
		if (retVal != ERR) retVal = socketFD;
	}

	return retVal;
}

int initShMem(int shmVal,  SERVER_DATA** srvrInfo) {

	key_t shmKey = ftok(".", shmVal);
	int shmID = 0;

	// ftok call failed
	if (shmKey == -1)  return -1;	

	// Memory could already exist, so we'll check first
        if ( (shmID = shmget (shmKey, sizeof(SERVER_DATA), 0)) == -1 ) {
			
      		shmID = shmget (shmKey, sizeof(SERVER_DATA), IPC_CREAT | SHM_PERM);
               
		// Shared memory could not be created
		if (shmID == -1)  return -1;                	
	}

	// finally, attach our pointer
        *srvrInfo = (SERVER_DATA*) shmat (shmID, NULL, 0);
	if (*srvrInfo == NULL) {		
		return -1;
	}

	return shmID;
}


void* clientHandler (void* shmid) {

	CLIENT clientData;
	int shmID = *((int*)shmid);	
	SERVER_DATA* srvrInfo = (SERVER_DATA*) shmat (shmID, NULL, 0);

	if (srvrInfo == NULL) {
		printf("[SERVER] : Client thread couldn't attach to shared mem. \n");
		fflush(stdout);
		pthread_kill(pthread_self(), SIGTERM);
	}
	
	int sockFD = srvrInfo->clSock;		
	
	memset(&clientData, 0, sizeof (CLIENT));
	while (strcmp(clientData.msg, ">>bye<<")) {
	
		memset(&clientData, 0, sizeof (CLIENT));
		read (sockFD, (void*)&clientData, sizeof (CLIENT));

		if (clientData.msg[0] != 0) printf("Server recieved msg: \"%s\", from %s \n", clientData.msg, clientData.username);
		// Broadcast the message to all other clients 
		
		for (int i = 0; i < srvrInfo->numClients; i++) {
			if (srvrInfo->clientSocks[i] != sockFD) write (srvrInfo->clientSocks[i], &clientData, sizeof (CLIENT));
		}
		
			
	}

	// shuffle clients
	for (int i = srvrInfo->numClients + 1; i < MAX_CLIENTS; i++) {
		memcpy(srvrInfo->clientSocks + (i-1), srvrInfo->clientSocks + i, sizeof (int)); 
	}

	srvrInfo->numClients--; 
	close(sockFD);	

	// If this thread is the last to exit, close server socket and send terminate signal to proccess
	if (srvrInfo->numClients == 0) {
		printf("\n[SERVER] : All clients are gone - SHUTDOWN INITIATED. \n");
		fflush(stdout);
		close(srvrInfo->srvrSock);

		shmdt (srvrInfo); // detach from shared mem
		shmctl(shmID, IPC_RMID, 0); 
		pthread_kill(pthread_self(), SIGTERM);
	}	
}


