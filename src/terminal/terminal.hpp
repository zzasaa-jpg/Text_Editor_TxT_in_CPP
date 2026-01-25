#ifndef TERMINAL_H
#define TERMINAL_H

#include <windows.h>

struct TerminalContext {
	HANDLE hStdOut;
	HANDLE hIn;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	INPUT_RECORD input;
	DWORD read;
};

extern TerminalContext g_Terminal_Context;

class Terminal{
	public:
		Terminal();
		void Terminal_init();
		void move_cursor(HANDLE hStdOut, int row, int col);
		void Clear_Screen(
				HANDLE hStdOut,
				CONSOLE_SCREEN_BUFFER_INFO* csbi,
				int rows, int cols
		);
};

extern Terminal terminal;

#endif // TERMINAL_H
