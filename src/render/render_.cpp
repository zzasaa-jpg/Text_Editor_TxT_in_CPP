#include "./render_.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"

#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

Render_ render_layout;
Render_::Render_() {}

void Render_::render_(
		HANDLE hStdOut,
		std::vector<std::string> buffer,
		int row, int col,
		int cursor_line,
		int cursor_col,
		WORD originalColor
){
	int editor_rows = row - 1;
	for (int r = 0; r < editor_rows; r++){
		terminal.move_cursor(g_Terminal_Context.hStdOut, r, 0);
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout<< "~";
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);
	}
}
