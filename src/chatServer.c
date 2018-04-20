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

	if ((srvrInfo->srvrSock = createSocket(PORT)) == -1) {
		printf("Server could not create socket. exiting. \n");
		return -1;
	}

	client_len = sizeof (client_addr);
	bind (srvrInfo->srvrSock, (struct sockaddr *)&server_addr, sizeof (server_addr));
	listen (srvrInfo->srvrSock, 5);
	
	srvrInfo->numClients = 0;
	// server will loop until the last client sends it a signal
	while (true) {

		
		if (srvrInfo->numClients < MAX_CLIENTS) {
				
			srvrInfo->clSock = accept (srvrInfo->srvrSock,(struct sockaddr *)&client_addr, &client_len); 
			srvrInfo->clientSocks[srvrInfo->numClients++] = srvrInfo->clSock;			
		
			// launch thread to handle our new client
			pthread_create(&(tid[(srvrInfo->numClients-1)]), NULL, clientHandler, (void *)&shmID);
		}
	}	
	return 0;
}







