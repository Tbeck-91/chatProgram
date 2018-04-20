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
	
	printf("[CLIENT] : Connecting to server with IP %s \n", inet_ntoa(server_addr.sin_addr));
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
	
	// Finally, launch our threads! 
	pthread_create (&tid, NULL, handleIncoming, (void*)&srvrSock);
	pthread_create (&tid, NULL, handleOutgoing, (void*)&srvrSock);	
	pthread_join(tid, (void *)(&retVal));	

	return 0;
}





