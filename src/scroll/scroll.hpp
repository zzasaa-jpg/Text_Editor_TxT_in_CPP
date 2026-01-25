#ifndef SCROLL_H
#define SCROLL_H

class Scroll{
	public:
		Scroll();
		void Vertical_Scroll(int cursor_line, int& scroll_offset, int row);
		void Horizontal_Scroll(int cursor_col, int& h_scroll, int col);
};

extern Scroll scroll;

#endif //SCROLL_H
