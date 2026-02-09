/* 
 * This old_line & old_col capture for 'need_redraw' function.
 * need_redraw functions checking is cursor is moved or buffer content changed.
 * old_line & old_col values according 'state.redraw' execute. This code will
 * reduce screen flickering.
*/

#include "./input_Key_.hpp"
#include "../buffer/buffer.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"
#include "../file_controller/file_controller.hpp"

#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

Input input;
Input::Input(){};

inline bool need_redraw(
    int old_line, int old_col,
    int new_line, int new_col
){
    return old_line != new_line || old_col != new_col;
}

// BackSpace Key
void Input::BackSpace_Key(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	// case1: delete the characters inside line
	if(cursor_col > 0)
	{
		buf[cursor_line].erase(cursor_col - 1, 1);
		cursor_col--;
		preferred_col = cursor_col;
		contrl_state.modified = true;
		state.redraw = true;
	}
	// case2: merge with previous line
	else if (cursor_line > 0)
	{
		int prev_len = buf[cursor_line  - 1].size();
		buf[cursor_line - 1] += buf[cursor_line];
		buf.erase(
			buf.begin() +
			cursor_line
		);
		cursor_line--;
		cursor_col = prev_len;
		preferred_col = cursor_col;
		contrl_state.modified = true;
		state.redraw = true;
	}
}

//Escape Key
void Input::Escape()
{
	contrl_state.controller_ = !contrl_state.controller_;
	file_controller_.Clear_Buffer();
	if(contrl_state.controller_){
		contrl_state.controller_col = 1;
		file_controller_.Render_Controller();
	}
	state.redraw = !contrl_state.controller_;
	//std::system("git");
}

// Resize Window
void Input::ReSize_Window(
		HANDLE hStdOut, CONSOLE_SCREEN_BUFFER_INFO* csbi,
		int& row, int& col, int& cursor_line, int& cursor_col	
){
	auto& buf = g_Buffer.get_buffer();
	GetConsoleScreenBufferInfo(hStdOut, csbi);
	row = csbi->srWindow.Bottom - csbi->srWindow.Top + 1;
	col = csbi->srWindow.Right - csbi->srWindow.Left + 1;
	cursor_col = std::min(cursor_col, (int)buf[cursor_line].size());

	// This check for redraw only happens when the console size actually changed.
	static int last_row = -1, last_col = -1;
	if(row != last_row || col != last_col)
	{
		state.redraw = true;
		last_row = row;
		last_col = col;
		
	} else state.redraw = false;
}

//Arrow_Up
void Input::Arrow_Up(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	int old_line = cursor_line,
	    old_col = cursor_col;

	if(cursor_line > 0)
	{
		cursor_line--;
		int line_len = buf[cursor_line].size();
		cursor_col = std::min(preferred_col, line_len);
		state.redraw = need_redraw(
			old_line, old_col,
			cursor_line, cursor_col
		);
			
	} else state.redraw = false;
}

//Arrow_Down
void Input::Arrow_Down(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	int old_line = cursor_line,
	    old_col = cursor_col;

	if(cursor_line + 1 < buf.size())
	{
		cursor_line++;
		int line_len = buf[cursor_line].size();
		cursor_col = std::min(preferred_col, line_len);
		state.redraw = need_redraw(
			old_line, old_col,
			cursor_line, cursor_col
		);
	} else state.redraw = false; 
}

//Arrow Left
void Input::Arrow_Left(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	int old_line = cursor_line,
	    old_col = cursor_col;

	if (cursor_col > 0)
	{
		cursor_col--;
	}
	else if(cursor_line > 0)
	{
		cursor_line--;
		cursor_col = buf[cursor_line].size();
	} else
	{
		state.redraw = false;
		return;
	}
	preferred_col = cursor_col;
	state.redraw = need_redraw(
		old_line, old_col,
		cursor_line, cursor_col	
	);
}

//Arrow Right
void Input::Arrow_Right(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	int old_line = cursor_line,
	    old_col = cursor_col;

	if (cursor_col < buf[cursor_line].size()) cursor_col++;
	else if(cursor_line + 1 < buf.size()){
		cursor_line++;
		cursor_col = 0;
	} else
	{
		state.redraw = false;
		return;
	}

	preferred_col = cursor_col;
	state.redraw = need_redraw(
		old_line, old_col,
		cursor_line, cursor_col		
	);
	
}

// Home key
void Input::Home(int& cursor_line, int& cursor_col, int& preferred_col)
{
	if (cursor_col == 0) // Reduce the unwanted redraw and cpu performance stable.
		return;
	else
	{
		cursor_col = preferred_col = 0;
		state.redraw = true;
	}
}

// End key
void Input::End(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	if (cursor_col == buf[cursor_line].size()) // Reduce the unwanted redraw and cpu performance stable
		return;
	if (cursor_line >= 0 && cursor_line < buf.size())
		cursor_col = buf[cursor_line].size();
	else
		cursor_col = 0;
	preferred_col = cursor_col;
	state.redraw = true;
}

// Insert new line
void Input::Insert_New_Line(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	std::string right = buf[cursor_line].substr(cursor_col);
	buf[cursor_line].erase(cursor_col);

	buf.insert(buf.begin() + cursor_line + 1, right);

	cursor_line++;
	cursor_col = preferred_col = 0;
	contrl_state.modified = true;
	state.redraw = true;
}

// Insert characters
void Input::Insert_Characters(char ch, int& cursor_line, int& cursor_col)
{
	auto& buf = g_Buffer.get_buffer();
	buf[cursor_line].insert(cursor_col, 1, ch);
	cursor_col++;
	contrl_state.modified = true;
	state.redraw = true;
}

// Tab
void Input::Insert_Tab(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();
	for (int i = 0; i < 4; i++)
	{
		buf[cursor_line].insert(cursor_col++, 1, ' ');
	}
	preferred_col = cursor_col;
	contrl_state.modified = true;
	state.redraw = true;
}

void Input::range_of_input_functions(int& cursor_line, int& cursor_col, int& preferred_col)
{
	auto& buf = g_Buffer.get_buffer();// Editor buffer
	auto& controller_buf = contrl_state.controller_buffer;// Controller buffer

	if(g_Terminal_Context.input.EventType == KEY_EVENT &&
		g_Terminal_Context.input.Event.KeyEvent.bKeyDown)
	{
		
		KEY_EVENT_RECORD& key = g_Terminal_Context.input.Event.KeyEvent;

		WORD vk = key.wVirtualKeyCode;
		char ch = key.uChar.AsciiChar;

		if(vk == VK_ESCAPE)
		{
			Escape();
			return;
		}

		if(contrl_state.controller_== false)
		{
			// Navigation and control
			switch(vk){
				case VK_LEFT:
					Arrow_Left(cursor_line, cursor_col, preferred_col);
					return;
				case VK_RIGHT:
					Arrow_Right(cursor_line, cursor_col, preferred_col);
					return;
				case VK_UP:
					Arrow_Up(cursor_line, cursor_col, preferred_col); 
					return;
				case VK_DOWN:
					Arrow_Down(cursor_line, cursor_col, preferred_col);
					return;
				case VK_HOME:
					Home(cursor_line, cursor_col, preferred_col);
					return;
				case VK_END:
					End(cursor_line, cursor_col, preferred_col);
					return;
			}
			// Special Keys
			if (vk == VK_RETURN) Insert_New_Line(cursor_line, cursor_col, preferred_col);
			else if(vk == VK_BACK) BackSpace_Key(cursor_line, cursor_col, preferred_col);
			else if(vk == VK_TAB) Insert_Tab(cursor_line, cursor_col, preferred_col);

			// Printable chars
			else if(ch >= 32 && ch <= 126) Insert_Characters(ch, cursor_line, cursor_col);
		} 
		else {
			contrl_state.controller_row = state.row - 1;

			// AUTO clear error
			if (contrl_state.Error)
			{
				file_controller_.Clear_Buffer();
				contrl_state.controller_col = 1;
				contrl_state.Error = false;
				file_controller_.Render_Controller();
				return;
			}

			// Handle backspace
			if(vk == VK_BACK)
			{
				int idx = contrl_state.controller_col - 2;
				if (idx >= 0)
				{
					contrl_state.controller_buffer.erase(idx, 1);
					contrl_state.controller_col--;
					file_controller_.Render_Controller();
				}
				return;
			}
			
			// Handle Printable chars
			if(ch >= 32 && ch <= 126)
			{
				int idx = contrl_state.controller_col - 1;
				if (idx >= 0 && idx <= contrl_state.controller_buffer.size())
				{
					contrl_state.controller_buffer.insert(
						contrl_state.controller_buffer.begin() + idx,
						ch
					);
					contrl_state.controller_col++;
					file_controller_.Render_Controller();
				}
			}

			// Execute command on Enter
			if(vk == VK_RETURN && contrl_state.controller_)
			{
				file_controller_.Execute_Command();
				return;
			}

			// VK_RIGHT
			if (vk == VK_RIGHT)
			{
				int max_col = 1 + contrl_state.controller_buffer.size();

				if (contrl_state.controller_col < max_col)
				{
					contrl_state.controller_col++;
					terminal.move_cursor(
						g_Terminal_Context.hStdOut,
						contrl_state.controller_row,
						contrl_state.controller_col
					);
				}
				return;
			}

			// VK_LEFT
			if (vk == VK_LEFT)
			{
				contrl_state.controller_row = state.row -1;
				if (contrl_state.controller_col > 1) {
					contrl_state.controller_col--;
					terminal.move_cursor(
						g_Terminal_Context.hStdOut,
						contrl_state.controller_row,
						contrl_state.controller_col
					);
				}
				return;
			}

			// Home key
			if(vk == VK_HOME)
			{
				// Reduce the unwanted Render Controller and cpu performance stable.
				if (contrl_state.controller_col == 1)
					return;
				else
				{
					contrl_state.controller_col = 1;
					file_controller_.Render_Controller();
				}
			}

			// End
			if(vk == VK_END)
			{
				// Reduce the unwanted Render Controller and cpu performance stable.
				if(contrl_state.controller_col == (1 + contrl_state.controller_buffer.size()))
					return;
				else
				{
					contrl_state.controller_col = 1 + contrl_state.controller_buffer.size();
					file_controller_.Render_Controller();
				}
			}
			return;	
		}
	}
}
