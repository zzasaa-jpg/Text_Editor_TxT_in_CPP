#ifndef INPUT_KEY_H
#define INPUT_KEY_H

#include <windows.h>
#include <vector>
#include <string>

class Input{
	public:
		Input();
		void Enter_Key(int& cursor_line, int& cursor_col, int& preferred_col);
		void BackSpace_Key(int& cursor_line, int& cursor_col, int& preferred_col);
		void Escape();
		void ReSize_Window(
			HANDLE hStdOut, CONSOLE_SCREEN_BUFFER_INFO* csbi,
			int& row, int& col, int& cursor_line, int& cursor_col,
			std::vector<std::string>& buffer
		);
		void Arrow_Up(int& cursor_line, int& cursor_col, int& preferred_col);
		void Arrow_Down(int& cursor_line, int& cursor_col, int& preferred_col);
		void Arrow_Left(int& cursor_line, int& cursor_col, int& preferred_col);
		void Arrow_Right(int& cursor_line, int& cursor_col, int& preferred_col);
		void range_of_input_functions(int& cursor_line, int& cursor_col, int& preferred_col);
};

extern Input input;

#endif //INPUT_KEY_H
