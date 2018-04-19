/*
* FILE : chatServer.c
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 08 / 04 / 2018
* DESCRIPTION :
*
*
*
*/

#include "../inc/chatProgram.h"
#include "../inc/chatServer.h"


int initShMem(int shmVal, SERVER_DATA** srvrInfo);

int main(void) {

	int clSock, retval, client_len;
	struct sockaddr_in client_addr, server_addr;
	pthread_t tid[MAX_CLIENTS];
	int shmID;
	SERVER_DATA* srvrInfo;

	if ( (shmID = initShMem(SHM_VAL, &srvrInfo)) == -1) {

		printf("Could not create shared memory! Server Exiting! \n");
		return -1;
	}
	else {
		printf("Checking perms: ..\n");
		srvrInfo->numClients = 0;
		printf("Perms good! \n");
	}

	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	//if((inet_pton(AF_INET,"192.168.0.24", &(server_addr.sin_addr))) == -1) return -1;
	server_addr.sin_port = htons (PORT);	
	srvrInfo->srvrSock = socket (AF_INET, SOCK_STREAM, 0);

	client_len = sizeof (client_addr);
	bind (srvrInfo->srvrSock, (struct sockaddr *)&server_addr, sizeof (server_addr));
	printf("[SERVER] : Listening for connections...\n");
	fflush(stdout);
	listen (srvrInfo->srvrSock, 5);
	printf("srvrSock is: %d\n", srvrInfo->srvrSock);

	srvrInfo->numClients = 0;
	// server will loop until the last client sends it a signal
	while (true) {

		
		if (srvrInfo->numClients < MAX_CLIENTS) {				
			srvrInfo->clSock = accept (srvrInfo->srvrSock,(struct sockaddr *)&client_addr, &client_len); 
			srvrInfo->clientSocks[srvrInfo->numClients++] = srvrInfo->clSock;			
			printf("Added new client. numClients is %d\n", srvrInfo->numClients);			
			fflush(stdout);

			// launch thread to handle our new client
			pthread_create(&(tid[(srvrInfo->numClients-1)]), NULL, clientHandler, (void *)&shmID);
		}
	}	
	return 0;
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
	printf("[CLIENT THREAD] srvrSock is: %d\n", srvrInfo->srvrSock);
	printf("numClients is %d\n", srvrInfo->numClients);
	printf("SOCKFD: %d", sockFD);
	fflush(stdout);
	
	
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




