#include "./input_Key_.hpp"
#include "../buffer/buffer.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"

#include <windows.h>
#include <algorithm>

Input input;
Input::Input(){};

EditorState state{};

// Enter Key
void Input::Enter_Key(int& cursor_line, int& cursor_col, int& preferred_col){
	g_Buffer.get_buffer().insert(
		g_Buffer.get_buffer().begin() +
		cursor_line + 1, ""
	);
	cursor_line++;
	cursor_col = preferred_col = 0;
	state.redraw = true;
}

// BackSpace Key
void Input::BackSpace_Key(int& cursor_line, int& cursor_col, int& preferred_col){
	// case1: delete the characters inside line
	if(cursor_col > 0){
		g_Buffer.get_buffer()[cursor_line].erase(cursor_col - 1, 1);
		cursor_col--;
		preferred_col = cursor_col;
		state.redraw = true;
	}
	// case2: merge with previous line
	else if (cursor_line > 0){
		int prev_len = g_Buffer.get_buffer()[cursor_line  - 1].size();
		g_Buffer.get_buffer()[cursor_line - 1] += g_Buffer.get_buffer()[cursor_line];
		g_Buffer.get_buffer().erase(
			g_Buffer.get_buffer().begin() +
			cursor_line		
		);
		cursor_line--;
		cursor_col = prev_len;
		preferred_col = cursor_col;
		state.redraw = true;
	}
}

//Escape Key
void Input::Escape(){
	//break;
}

//Arrow_Up
void Input::Arrow_Up(int& cursor_line, int& cursor_col, int& preferred_col){
	if(cursor_line > 0){
		cursor_line--;
		int line_len = g_Buffer.get_buffer()[cursor_line].size();
		cursor_col = std::min(preferred_col, line_len);
		state.redraw = true;
	}
}

//Arrow_Down
void Input::Arrow_Down(int& cursor_line, int& cursor_col, int& preferred_col){
	if(cursor_line + 1 < g_Buffer.get_buffer().size()){
		cursor_line++;
		int line_len = g_Buffer.get_buffer()[cursor_line].size();
		cursor_col = std::min(preferred_col, line_len);
		state.redraw = true;
	}
}

//Arrow Left
void Input::Arrow_Left(int& cursor_line, int& cursor_col, int& preferred_col){
	if (cursor_col > 0) cursor_col--;
	else if(cursor_line > 0){
		cursor_line--;
		cursor_col = g_Buffer.get_buffer()[cursor_line].size();
	}
	preferred_col = cursor_col;
	state.redraw = true;
}

//Arrow Right
void Input::Arrow_Right(int& cursor_line, int& cursor_col, int& preferred_col){
	if (cursor_col < g_Buffer.get_buffer()[cursor_line].size()) cursor_col++;
	else if(cursor_line + 1 < g_Buffer.get_buffer().size()){
		cursor_line++;
		cursor_col = 0;
	}
	preferred_col = cursor_col;
	state.redraw = true;
}

void Input::range_of_input_functions(int& cursor_line, int& cursor_col, int& preferred_col){
	if(g_Terminal_Context.input.EventType == KEY_EVENT &&
		g_Terminal_Context.input.Event.KeyEvent.bKeyDown){
		
		WORD key = g_Terminal_Context.input.Event.KeyEvent.wVirtualKeyCode;

		switch(key){
			case VK_RETURN:
				Enter_Key(cursor_line, cursor_col, preferred_col);
				break;
			case VK_BACK:
				BackSpace_Key(cursor_line, cursor_col, preferred_col);
				break;
			case VK_LEFT:
				Arrow_Left(cursor_line, cursor_col, preferred_col);
				break;
			case VK_RIGHT:
				Arrow_Right(cursor_line, cursor_col, preferred_col);
				break;
			case VK_UP:
				Arrow_Up(cursor_line, cursor_col, preferred_col);
				break;
			case VK_DOWN:
				Arrow_Down(cursor_line, cursor_col, preferred_col);
				break;
			case VK_ESCAPE: Escape(); break;
		}
	}
}
