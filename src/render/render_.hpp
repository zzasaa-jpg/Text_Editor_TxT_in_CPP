#ifndef RENDER_H
#define RENDER_H

#include <windows.h>
#include <vector>
#include <string>

class Render_{
	public:
		Render_();
		void render_(
			HANDLE hStdOut,
			std::vector<std::string>& buffer,
			int row, int col,
			int cursor_line,
			int cursor_col,
			int scroll_offset,
			int h_scroll,
			WORD originalColor
		);
		void ReDraw(
			int& row, int& col,
			int& cursor_line, int& cursor_col,
			int& scroll_offset, int& h_scroll,
			WORD originalColor
		);
};

extern Render_ render_layout;

#endif //RENDER_H
