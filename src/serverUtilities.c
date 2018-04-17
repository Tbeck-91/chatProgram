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



#include "chatServer.h"

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
		memset (&serverData, 0, sizeof (struct serverData));
		serverData.sin_family = AF_INET;
		serverData.sin_addr.s_addr = (INADDR_ANY);
		serverData.sin_port = htons(port);
	
		// bind socket to our port
		retval = bind (socketFD, (struct sockaddr*)&serverData, sizeof (struct serverData);
		if (retVal != ERR) retVal = socketFD;
	}

	return retVal;
}

/*
	FUNCTION: createSocket
	DESCRIPTION: Takes a file descriptor
		     of a socket listening for connections
		     on a port and returns the file descriptor
		     for a new socket for communications. the sockaddr_in
		     pointer will be filled in with the IP info of incoming 
		     client. This is where the magic happens baby!		
	PARAMETERS: int port: the port to bind the socket to.
	returns: int retVal: will be -1 on error, otherwise the socket FILE DESCRIPTOR

*/
int getClient(int sockFD, struct sockaddr_in* clientAddr) {

	int retVal; // new remote communication socket FD or error (-1)
	int clientSize = sizeof (struct sockaddr_in)

	// open a new communications socket.
	retVal = accept (sockFD, clientAddr, clientSize);

	return retVal;

}
