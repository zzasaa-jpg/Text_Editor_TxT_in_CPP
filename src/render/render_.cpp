#include "./render_.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"
#include "../buffer/buffer.hpp"
#include "../file_controller/file_controller.hpp"

#include <windows.h>
#include <vector>
#include <string>
#include <iostream>

Render_ render_layout;
Render_::Render_() {}

// dirty init----------------
Dirty_flag dirty_flag;
Dirty_flag::Dirty_flag() {};
//---------------------------

// Editor status bar --------------------------------------------------------------------------------
void status_bar(
		int row, int col,
		int cursor_line,
		int cursor_col,
		int scroll_offset, int h_scroll,
		std::vector<std::string>& buffer,
		WORD originalColor
){
	terminal.move_cursor(g_Terminal_Context.hStdOut, row - 2, 0);

	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	std::string status = 
		"LINE: " + std::to_string(cursor_line + 1) +
		" | COL: " + std::to_string(cursor_col) +
		" | V-OFF: " + std::to_string(scroll_offset) +
		" | H-OFF: " + std::to_string(h_scroll) +
		" | TOTAL: " + std::to_string(buffer.size()) +
		" | " + (contrl_state.file_path.empty() ? "[No Name]" : contrl_state.file_path) +
		(contrl_state.modified ? "*" : "");

	int padding = col - status.length();

	for (int i = 0; i < padding; i++) std::cout << " ";
	std::cout << status;
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);
}
// ----------------------------------------------------------------------------------------------------

// Buffer load to editor (Terminal screen) ------------------------------------------------------------
void buffer_content_load(
		int r, std::vector<std::string>& buffer,
		int& scroll_offset, int& h_scroll, int max_width,
		int text_col
){
		int buf_index = scroll_offset + r;
		if(buf_index >= (int)buffer.size())
			return;
		terminal.move_cursor(
			g_Terminal_Context.hStdOut,
			r, text_col
		);
		const std::string& line = buffer[buf_index];

		int visible_end = std::min((int)line.size(), h_scroll+ max_width);

		int printed = 0;

		for (int c = h_scroll; c < visible_end; c++)
		{
			std::cout << line[c];
			printed++;
		}

		for (int i = printed; i < max_width; i++)
		{
			std::cout << ' ';
		}
}
// ----------------------------------------------------------------------------------------------------

// Editor redraw function. Editor inside every new event or any new changes this function execute -----
void Render_::ReDraw(
		int& row, int& col,
		int& cursor_line, int& cursor_col,
		int& scroll_offset, int& h_scroll,
		WORD originalColor
){
	/*terminal.Clear_Screen(
		g_Terminal_Context.hStdOut,
		&g_Terminal_Context.csbi,
		row, col
	);*/
	
	render_layout.render_(
		g_Terminal_Context.hStdOut,
		g_Buffer.get_buffer(),
		row, col, cursor_line,
		cursor_col, scroll_offset, h_scroll,
		originalColor
	);
	int screen_row = cursor_line - scroll_offset;
	int screen_col = cursor_col - h_scroll + 2;
	if(!contrl_state.controller_){
		terminal.move_cursor(
			g_Terminal_Context.hStdOut, 
			screen_row, screen_col
		);
	} else{
		terminal.move_cursor(
			g_Terminal_Context.hStdOut, 
			state.row - 1, 1 + contrl_state.controller_buffer.size()
		);	
	}
	state.redraw = false;
}
// ----------------------------------------------------------------------------------------------------

// This function is ENTRY POINT of render module ------------------------------------------------------
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
	int editor_rows = row - 2,
	r_ = 0, text_col = 2, max_width = col - text_col;

	// initialize the editor rows
	for (int r = r_; r < editor_rows; r++){
		terminal.move_cursor(g_Terminal_Context.hStdOut, r, 0);
		SetConsoleTextAttribute(
				g_Terminal_Context.hStdOut,
				FOREGROUND_GREEN | FOREGROUND_INTENSITY
		);
		std::cout<< "~";
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);

		// buffer content load to screen
		buffer_content_load(
			r, buffer,
			scroll_offset, h_scroll,
			max_width, text_col
		);
	}

	// status bar execute
	status_bar(
		row, col, cursor_line,
		cursor_col, scroll_offset,
		h_scroll, buffer,
		originalColor
	);
	
	// This is editor controller code
	if(contrl_state.controller_)
	{
		terminal.move_cursor(
			g_Terminal_Context.hStdOut,
			row - 1, 0
		);
		std::cout << ":" << contrl_state.controller_buffer;

		terminal.move_cursor(
			g_Terminal_Context.hStdOut,
			row - 1, 1 + contrl_state.controller_buffer.size()
		);
		std::cout.flush();	
	}
}
// ----------------------------------------------------------------------------------------------------


void Render_::Render_Dirty_Lines(
	std::vector<std::string>& buffer,
	int row, int col, int scroll_offset,
	int h_scroll, WORD originalColor
)
{
	int editor_rows = row - 2,
	r_ = 0, text_col = 2, max_width = col - text_col;
	for (int r = r_; r < editor_rows; r++){
		int buf_index = scroll_offset + r;
		if (buf_index >= buffer.size())
			continue;
		if (!dirty_flag.isDirty(buf_index))
			continue;

		terminal.move_cursor(g_Terminal_Context.hStdOut, r, 0);
		SetConsoleTextAttribute(
			g_Terminal_Context.hStdOut,
			FOREGROUND_GREEN | FOREGROUND_INTENSITY
		);
		std::cout<< "~";
		SetConsoleTextAttribute(g_Terminal_Context.hStdOut, originalColor);

		// buffer content load to screen
		buffer_content_load(
			r, buffer,
			scroll_offset, h_scroll,
			max_width, text_col
		);

		dirty_flag.markClean(buf_index);
	}
}



void Render_::Render_Status_Only(
		int row, int col,
		int cursor_line,
		int cursor_col,
		int scroll_offset,
		int h_scroll,
		std::vector<std::string>& buffer,
		WORD originalColor
)
{
	status_bar(
		row, col, cursor_line,
		cursor_col, scroll_offset,
		h_scroll, buffer,
		originalColor
	);
}
void Render_::Render_Controller_Row()
{
    if (!contrl_state.controller_ && !contrl_state.Error) return;

    int row = state.row - 1;

    // Clear the row
    terminal.move_cursor(g_Terminal_Context.hStdOut, row, 0);
    std::string empty(state.col, ' ');
    std::cout << empty;

    // Print controller prompt / error message
    terminal.move_cursor(g_Terminal_Context.hStdOut, row, 0);
    std::cout << ":" << contrl_state.controller_buffer;

    // Move cursor to the correct column
    terminal.move_cursor(
        g_Terminal_Context.hStdOut,
        row,
        contrl_state.controller_col
    );

    std::cout.flush();
}
// --------------------
void Dirty_flag::initialize(size_t buffer_size)
{
	dirty_lines.clear();
	dirty_lines.resize(buffer_size, true);
}

void Dirty_flag::markDirty(size_t line)
{
	if(line < dirty_lines.size())
		dirty_lines[line] = true;
}

void Dirty_flag::markRangeDirty(size_t start, size_t end)
{
	if (dirty_lines.empty())
		return;

	if (end >= dirty_lines.size())
		end = dirty_lines.size() - 1;

	for (size_t i = start; i <= end; i++)
	{
		dirty_lines[i] = true;
	}
}

bool Dirty_flag::isDirty(size_t line) const
{
	if(line < dirty_lines.size())
		return dirty_lines[line];

	return false;
}

void Dirty_flag::markClean(size_t line)
{
	if(line < dirty_lines.size())
	{
		dirty_lines[line] = false;
	}
}

void Dirty_flag::resize(size_t new_size)
{
	if(new_size > dirty_lines.size())
		dirty_lines.resize(new_size, true);
}

// --------------------
