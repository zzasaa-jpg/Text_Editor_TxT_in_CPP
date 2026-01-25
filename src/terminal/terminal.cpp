#include "./terminal.hpp"
#include "../core/editor.hpp"

#include <windows.h>
#include <iostream>
using namespace std;

Terminal terminal;
TerminalContext g_Terminal_Context;

Terminal::Terminal() {}

void Terminal::Terminal_init(){
	g_Terminal_Context.hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	g_Terminal_Context.hIn = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD input;
	DWORD read;

	SetConsoleTitle("TxT - Undefined");

	if (!GetConsoleScreenBufferInfo(
				g_Terminal_Context.hStdOut,
				&g_Terminal_Context.csbi
	)) return;
}

void Terminal::move_cursor(HANDLE hStdOut, int row, int col){
	COORD position { (SHORT)col, (SHORT)row };
	SetConsoleCursorPosition(hStdOut, position);
}

void Terminal::Clear_Screen(
		HANDLE hStdOut,
		CONSOLE_SCREEN_BUFFER_INFO* csbi,
		int rows, int cols
){
	EditorState state{};
	DWORD written, cells = 
		g_Terminal_Context.csbi.dwSize.X *
		g_Terminal_Context.csbi.dwSize.Y;
	COORD homeCoords = {0, 0};
	FillConsoleOutputCharacter(
			g_Terminal_Context.hStdOut, ' ',
			cells, homeCoords, &written
	);
	move_cursor(g_Terminal_Context.hStdOut, 0, 0);
}
