#include "./render_.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"
#include "../buffer/buffer.hpp"

#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

Render_ render_layout;
Render_::Render_() {}

void status_bar(
		int row, int col,
		int cursor_line,
		int cursor_col,
		std::vector<std::string>& buffer,
		WORD originalColor
){
	terminal.move_cursor(g_Terminal_Context.hStdOut, row - 1, 0);
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	std::string status = 
		"LINE: " + std::to_string(cursor_line + 1) +
		" | COL: " + std::to_string(cursor_col) +
	       	" | TOTAL: " + std::to_string(buffer.size());	

	int padding = col - status.length();

	for (int i = 0; i < padding; i++) std::cout << " ";
	std::cout << status;
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);
}

void buffer_content_load(int r, std::vector<std::string>& buffer, int row){
	int screen_rows = row;
	for (int screen_r = 0; screen_r < screen_rows; screen_r++){
		int buf_r = r + screen_r;

		terminal.move_cursor(g_Terminal_Context.hStdOut, screen_r, 2);

		if(buf_r < (int)buffer.size()){
			const std::string& line = buffer[buf_r];
			std::cout << line;
		 }
	}
}

void Render_::ReDraw(
		int& row, int& col,
		int& cursor_line, int& cursor_col,
		int& scroll_offset, int& h_scroll,
		WORD originalColor
){
	terminal.Clear_Screen(
		g_Terminal_Context.hStdOut,
		&g_Terminal_Context.csbi,
		row, col
	);
	
	render_layout.render_(
		g_Terminal_Context.hStdOut,
		g_Buffer.get_buffer(),
		row, col, cursor_line,
		cursor_col, originalColor
	);
	int screen_row = cursor_line - scroll_offset;
	int screen_col = cursor_col - h_scroll + 2;
	terminal.move_cursor(
		g_Terminal_Context.hStdOut, 
		screen_row, screen_col
	);
} 

void Render_::render_(
		HANDLE hStdOut,
		std::vector<std::string>& buffer,
		int row, int col,
		int cursor_line,
		int cursor_col,
		WORD originalColor
){
	int editor_rows = row - 1, r_ = 0;
	for (int r = r_; r < editor_rows; r++){
		terminal.move_cursor(g_Terminal_Context.hStdOut, r, 0);
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		std::cout<< "~";
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);
	}
	buffer_content_load(r_, g_Buffer.get_buffer(), row);
	status_bar(row, col, cursor_line, cursor_col, g_Buffer.get_buffer(), originalColor);
}
