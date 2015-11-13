#include <ncurses.h>
#include <string.h>

#define ESC 27	// Keycode for 'ESC' 

// Globals
int cursorX = 0; 
int cursorY = 1;
bool editState = false;

// Declarations
void setPrompt(char *prompt, int y, int x);
void moveCursor(int dy,int dx); 

// Initializes the curses library
void initializeCurses()
{
	initscr();
	raw();
	nonl();
	keypad(stdscr, true);
	noecho();

	printw("Press E to Exit\n");
}

// Move cursor function.
void moveCursor(int dy, int dx)
{
	if (dx >= 0 && dy > 0)
	{
		move(dy, dx);
		cursorY = dy;
		cursorX = dx;
	}
	else
	{
		setPrompt("Cannot move out of bounds!", cursorY, cursorX);
	}
}

// Sets the prompt
void setPrompt(char *prompt, int y, int x)
{
	getyx(stdscr, y, x);
	move(LINES - 1 ,0);
	clrtoeol();
	mvprintw(LINES - 1,0, prompt);
	move(y,x);
	
}

void handleEditInput(int ch)
{
	switch(ch)
	{
		case ESC:
			editState = false;
			setPrompt("Entered command mode.", cursorY, cursorX);
			break;	
		case '\r':
			setPrompt("Enter pressed", cursorY, cursorX);
			mvprintw(cursorY,cursorX, "\n");
			cursorY += 1;
			cursorX = 0;
		default:
			printw("%c", ch);
			cursorX += 1;
			break;	
	}	
}


void handleCommandInput(int ch)
{

	switch(ch)
	{
		case ESC:
			editState = true;
			setPrompt("Entered edit mode.", cursorY, cursorX);
			break;
		case 'x':
			mvprintw(cursorY, cursorX, " ");	
		default:
			break;		
	}	
}


int main()
{
	initializeCurses();
	int ch; // keyboard input char.

	while((ch = getch()) != 'E')
	{
		switch(ch)
		{
			case KEY_UP:	
				setPrompt("Up arrow", cursorY, cursorX);
				moveCursor(cursorY - 1, cursorX);
				break;

			case KEY_DOWN:
				setPrompt("Down Arrow", cursorY, cursorX);
				moveCursor(cursorY + 1, cursorX);
				break;
			case KEY_LEFT:
				setPrompt("Left Arrow", cursorY, cursorX);
				moveCursor(cursorY, cursorX - 1);
				break;
			case KEY_RIGHT:
				setPrompt("Right Arrow", cursorY, cursorX);
				moveCursor(cursorY, cursorX + 1);
				break;
			default:
				if (editState)
				{
					handleEditInput(ch);
				}
				else
				{
					handleCommandInput(ch);
				}
		}

	}	

	endwin();
	return 0;
}
