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
		int scroll_offset, int h_scroll,
		std::vector<std::string>& buffer,
		WORD originalColor
){
	terminal.move_cursor(g_Terminal_Context.hStdOut, row - 1, 0);
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	std::string status = 
		"LINE: " + std::to_string(cursor_line + 1) +
		" | COL: " + std::to_string(cursor_col) +
		" | V-OFF: " + std::to_string(scroll_offset) +
		" | H-OFF: " + std::to_string(h_scroll) +
	       	" | TOTAL: " + std::to_string(buffer.size());	

	int padding = col - status.length();

	for (int i = 0; i < padding; i++) std::cout << " ";
	std::cout << status;
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);
}

void buffer_content_load(
		int r, std::vector<std::string>& buffer,
		int& scroll_offset, int& h_scroll, int max_width,
		int text_col
){
		int buf_index = scroll_offset + r;
		if(buf_index < (int)buffer.size()){
			terminal.move_cursor(
				g_Terminal_Context.hStdOut,
				r, text_col
			);
			const std::string& line = buffer[buf_index];
			for (int c = h_scroll; c < (int)line.size() &&
				c < h_scroll + max_width; c++){
				std::cout << line[c];
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
		cursor_col, scroll_offset, h_scroll,
		originalColor
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
		int scroll_offset,
		int h_scroll,
		WORD originalColor
){
	int editor_rows = row - 1,
	r_ = 0, text_col = 2, max_width = col - text_col;

	for (int r = r_; r < editor_rows; r++){
		terminal.move_cursor(g_Terminal_Context.hStdOut, r, 0);
		SetConsoleTextAttribute(
				g_Terminal_Context.hStdOut,
				FOREGROUND_GREEN | FOREGROUND_INTENSITY
		);
		std::cout<< "~";
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);

		buffer_content_load(
			r, buffer,
			scroll_offset, h_scroll,
			max_width, text_col
		);
	}
	status_bar(
		row, col, cursor_line,
		cursor_col, scroll_offset,
		h_scroll, buffer,
		originalColor
	);
}
