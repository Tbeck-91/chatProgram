#include "../inc/chatProgram.h"
#include "../inc/chatClient.h"
#include <curses.h>

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

int main (int argc, char** argv) {

	CLIENT clientInfo;
	WINDOW* inputW; // input window
	WINDOW* historyW; // msg history window

	char buf[MSG_LEN + 1], chr;
	bool done = 0;
	int chrCount = 0;
	bool bckspc = true;	

	memset(buf, 0, 81);

	/* Get client Info */
	setupClient(&clientInfo);	
	initCurses();

	setUpWindows(&inputW, &historyW);

	
	mvprintw(LINES-1, 0, "ONLINE:");
	attron(COLOR_PAIR(clientInfo.colour));
	mvprintw(LINES - 1, 9, " %s", clientInfo.username);
	attroff(COLOR_PAIR(clientInfo.colour));	
		

	update(inputW, historyW);	
	while (!done) {
		
		bckspc = 0;
		
		if (isprint(chr = getch()) && chrCount < MSG_LEN) buf[chrCount++] = chr;
		if (chr == '`') break;

		if (chr == 10) {
			buf[chrCount] = '\0';
			printHistory(&clientInfo, buf, historyW, 1);
			clearInputW(inputW, buf);
			chrCount = 0;		
		}
		else if (chr== KEY_BACKSPACE || chr == 127 || chr == 8) {
			
			handleBckspc(inputW, &chrCount);			
			bckspc = 1;
	
		}		
		
		updateInputWin(inputW, chrCount,  buf, bckspc);
		// redraw screen
		update(inputW, historyW);		
	}	
		
	// free windows and exit curses mode
	delwin(inputW);
	delwin(historyW);
	endwin();
	

	return 0;
}

/*
	FUNCTION: setupClient

	DESCRIPTION: Gathers information about the client
		     used by the server (ip) and the UI
		     (username, name colour).

	PARAMETERS: CLIENT* clientinfo: pointer to client struct

	RETURNS: VOID


*/
void setupClient(CLIENT* clientInfo) {

	char nameBuf[USRNAME_LEN + 1];
	char hostNameBuf[128];  
	char* chr;
	int colour;
	int retVal;
	struct hostent* ipAddr;
	
	
	// stealthily store clients IP as a struct in_addr  ;-)
	gethostname(hostNameBuf, 128);
	ipAddr = gethostbyname(hostNameBuf);
	memcpy(&(clientInfo->ipAddr), ipAddr->h_addr, sizeof(struct in_addr));
	
	printf (
			"\nWelcome to the Beck - Corriveau Chat Room! \n"
			"Before you can enter the chat room, I need a couple "
			"pieces of information.\n\n"
			"First, enter your username [max %d characters] \n>> ",
			USRNAME_LEN
		);
		
	fgets(nameBuf,USRNAME_LEN + 1, stdin);
	if ( (chr = strchr(nameBuf, '\n')) != NULL ) *chr = '\0';
	strcpy(clientInfo->username, nameBuf);

	printf("\nThanks %s! One more thing!\n", clientInfo->username);
	printf("Enter the number corresponding to one of the following colours: \n");
	printf("1. Red 2. Green 3. Yellow 4. Blue\n>> ");
	
	
		
	while(!(colour >= MIN_COLOUR && colour <= MAX_COLOUR)) {

		stdin = freopen(NULL,"r",stdin); // clear input bufffer

		colour = atoi(fgets(nameBuf, USRNAME_LEN, stdin));
		
		if (colour < MIN_COLOUR || colour > MAX_COLOUR) {
		
			printf("That's not an option.\n>> ");
				
		}		
	}
	
	clientInfo->colour = colour;	
}

WINDOW* initWindow( int h, int w, int y, int x) {

	WINDOW* win = newwin(h, w, y, x);
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	return win;

}

void setUpWindows(WINDOW** inputW, WINDOW** historyW) {


	*historyW = initWindow(LINES - INPUT_BOX_SZ - 2 , COLS, INPUT_BOX_SZ + 1, 0);
	*inputW = initWindow(INPUT_BOX_SZ, COLS, 0, 0);
	scrollok(*historyW, true); // enable scrolling

	mvwprintw(*historyW, 0, COLS/2 - 4, "MESSAGES");
	mvwprintw(*inputW, INPUT_BOX_SZ -1, (COLS/2) - 3, "[%02d/%d]", 0, MSG_LEN);
	mvwprintw(*inputW, 2, 1, "> ");	
}

void initCurses(void) {

	curs_set(0);

	/* Startup Curses */	
	initscr(); // enter curses mode
	start_color(); // turn on colours
	cbreak(); // disable line buffering
	noecho(); // echo off	

	//initialize colour pairs
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
}


void update(WINDOW* win1, WINDOW* win2) {

	// refresh stdscr and windows			
	refresh(); 	
	wrefresh(win1);	
	wrefresh(win2);

}

void printHistory(CLIENT* clientInfo, char* buf, WINDOW* historyW, bool client) {

	static int histCount = 1; // tracks what line we're on
	char lineBuf[MSG_LEN + 1];
	char tmpBuf[PARCEL_LEN + 1];
	int msgLen;
	int i;

	while ((msgLen = strlen(buf)) > PARCEL_LEN ) {

		// walk back a few characters and check for a space
		for (i = PARCEL_LEN ; i > 33 && buf[i] != SPACE; i--);

		// split message at space or at parcel limit
		if (buf[i] == SPACE) {
			strncpy(tmpBuf, buf, i);
			strcpy(buf, (buf + i));
			buf[i] = tmpBuf[i] = '\0';			
			printHistory(clientInfo, tmpBuf, historyW, 1);
			break;
		}
		else{

			strncpy(tmpBuf, buf, PARCEL_LEN);
			strcpy(buf, (buf + PARCEL_LEN ));
			buf[PARCEL_LEN] = tmpBuf[PARCEL_LEN] = '\0';
			printHistory(clientInfo, tmpBuf, historyW, 1);
		}		
	}

	formatOutputString(buf, lineBuf, clientInfo, true);
	
	if (histCount == (LINES - INPUT_BOX_SZ - 3)) {
		wborder(historyW, ' ', ' ', ' ',' ',' ',' ',' ',' '); // remove border
		wscrl(historyW, 1); // scroll up one line
		wborder(historyW, '|', '|', '-', '-', '+', '+', '+', '+');
		mvwprintw(historyW, 0, COLS/2 - 4, "MESSAGES");
		histCount -= 1;
	}	
	
	
	mvwprintw(historyW, histCount, 1, "%s", lineBuf);

	// bold the clients username and set it to their colour
	mvwchgat(historyW, histCount, 18, 5, A_BOLD, clientInfo->colour, NULL);
	wmove(historyW, 0, 0);
	histCount++;

}

void formatOutputString(char* msgBuf, char* lineBuf, CLIENT* clientInfo, bool client) {

	char timeBuf[9];
	time_t curTime = time(NULL);
	// populate time struct with current time 
	struct tm* tmPtr = localtime( &curTime );
        strftime (timeBuf, (size_t)9, "%H:%M:%S", tmPtr);

	char* ipAddr = inet_ntoa(clientInfo->ipAddr);

	sprintf(lineBuf, "%-15s [%-5s] %s %-40s (%s)", 
		ipAddr, clientInfo->username, client ? ">>" : "<<", msgBuf, timeBuf);
}

void clearInputW(WINDOW* inputW, char* buf) {

	werase(inputW);
	wborder(inputW, '|', '|', '-', '-', '+', '+', '+', '+');
	mvwprintw(inputW, 2, 1, ">");	
	memset(buf, 0, MSG_LEN);	
	buf[0] = SPACE;

}

void handleBckspc(WINDOW* win, int* chrCounter) {

	int cury, curx;

	getyx(win, cury, curx);
	mvwprintw(win, cury, curx-1, " ");
	wmove(win, cury, curx-1);
	*chrCounter = *chrCounter > 0 ? *chrCounter - 1 : 0;
}

void updateInputWin(WINDOW* inputW, int chrCount, char* buf, bool bckspc) {

	int cury, curx;

	getyx(inputW, cury, curx);

	// Turn on "warning" colours if user is close or at input boundary
	if (chrCount >= 70 && chrCount < 80) wattron(inputW, COLOR_PAIR(COLOR_YELLOW));
	else if (chrCount == 80) wattron(inputW, COLOR_PAIR(COLOR_RED));

	mvwprintw(inputW, INPUT_BOX_SZ - 1, (COLS/2) - 3, "[%02d/%d]", chrCount, MSG_LEN);
	wmove(inputW, cury, curx);

	if (chrCount >= 70 && chrCount < 80) wattroff(inputW, COLOR_PAIR(COLOR_YELLOW));
	else if (chrCount == 80) wattroff(inputW, COLOR_PAIR(COLOR_RED));

	// print last character entered onto input screen
	if (!bckspc && chrCount < 80) wprintw(inputW, "%c", buf[chrCount != 0 ? chrCount - 1 : 0]);

}




