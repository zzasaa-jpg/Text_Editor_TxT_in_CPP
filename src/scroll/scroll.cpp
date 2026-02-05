#include "./scroll.hpp"

Scroll scroll;

const int text_col = 2; // i did n't use the 'define' for Type Safety and Debugging.

Scroll::Scroll() {}

// Vertical Scroll ------------------------------------------------------------
void Scroll::Vertical_Scroll(int cursor_line, int& scroll_offset, int row){
	int visible_lines = row - 1;

	// if cursor gose above screen -> moves screen UP.
	if (cursor_line < scroll_offset)
		scroll_offset = cursor_line;

	// if curosor goes below screen -> move screen DOWN.
	if (cursor_line >= scroll_offset + visible_lines)
		scroll_offset = cursor_line - visible_lines + 1;

	// don't go above first line.
	if (scroll_offset < 0) scroll_offset = 0;
}
// ----------------------------------------------------------------------------

// Horizontal Scroll ----------------------------------------------------------
void Scroll::Horizontal_Scroll(int cursor_col, int& h_scroll, int col){
	int visible_cols = col - text_col;

	// cursor moved LEFT outside screen -> move screen LEFT.
	if (cursor_col < h_scroll)
		h_scroll = cursor_col;

	// cursor moved RIGHT outside screen -> move screen RIGHT.
	if (cursor_col >= h_scroll + visible_cols)
		h_scroll = cursor_col - visible_cols + 1;

	// don't scroll before column 0.
	if (h_scroll < 0) h_scroll = 0;
}
// ----------------------------------------------------------------------------
