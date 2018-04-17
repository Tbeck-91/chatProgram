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

//CLIENT clientLobby[MAX_CLIENTS];

int main(void) {

	int srvrSock, clSock, retval, numClients, client_len;
	struct sockaddr_in client_addr, server_addr;
	char buf[81] = { 0 };

	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (PORT);
	
	
	srvrSock = socket (AF_INET, SOCK_STREAM, 0)) < 0) 

	client_len = sizeof (client_addr);
	bind (server_socket, (struct sockaddr *)&server_addr, sizeof (server_addr);
	listen (server_socket, 5);
	clSock = accept (server_socket,(struct sockaddr *)&client_addr, &client_len); 

	while (strcmp(buf, ">>bye<<")) {

		memset(buf,0, 81);
		read (clSock, buffer, BUFSIZ);

		/* we're actually not going to execute the command - but we could if we wanted */
		printf (message, "[SERVER] : Received the command - %s\n", buf);
		// clear out and get the next command and process		

	}	
	
	close (server_socket);
	return 0;
}
