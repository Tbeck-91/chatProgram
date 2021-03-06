/*
* FILE : chatClient.h
* PROJECT : PROG1970 - Systems Programming Assignment 4
* PROGRAMMER : Taylor Beck
* FIRST VERSION : 22 / 04 / 2018
* DESCRIPTION : All of the includes, definitions
*		and prototypes used in the chat client program.
*/

#include "../../common/inc/chatProgram.h"
#include <curses.h>

#define MIN_COLOUR 1
#define MAX_COLOUR 4
#define INPUT_BOX_SZ 5
#define QUIT_COMMAND ">>bye<<"

/* Global Variables */

WINDOW* inputW; // input window
WINDOW* historyW; // msg history window
CLIENT clientInfo;

/* UI / Curses functions */
WINDOW* initWindow(int h, int w, int y, int x);
void setupClient(CLIENT* clientInfo);
void initCurses(void);
void update(WINDOW* win1, WINDOW* win2);
void handleBckspc(WINDOW* win, int* chrCounter);
void updateInputWin(WINDOW* inputW, int chrCount, char* buf, bool bckspc);
void printHistory(CLIENT* clientInfo, char* buf, WINDOW* historyW, bool client); 
void clearInputW(WINDOW* inputW, char* buf);
void setUpWindows(WINDOW** inputW, WINDOW** historyW); 
void setUpWindows(WINDOW** inputW, WINDOW** historyW);
void formatOutputString(char* msgBuf, char* lineBuf,CLIENT* clientInfo, bool client);


/* Thread utilities */
void* handleOutgoing (void* srvrSock);
void* handleIncoming (void* srvrSock);
