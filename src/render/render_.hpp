#ifndef RENDER_H
#define RENDER_H

#include <windows.h>
#include <vector>
#include <string>

class Render_
{
	public:
		Render_();
		void render_
		(
			HANDLE hStdOut,
			std::vector<std::string>& buffer,
			int row, int col,
			int cursor_line,
			int cursor_col,
			int scroll_offset,
			int h_scroll,
			WORD originalColor
		);
		void ReDraw
		(
			int& row, int& col,
			int& cursor_line, int& cursor_col,
			int& scroll_offset, int& h_scroll,
			WORD originalColor
		);
		void Render_Dirty_Lines(
			std::vector<std::string>& buffer,
			int row, int col, int scroll_offset,
			int h_scroll, WORD originalColor
		);
		void Render_Status_Only(
			int row, int col,
			int cursor_line,
			int cursor_col,
			int scroll_offset,
			int h_scroll,
			std::vector<std::string>& buffer,
			WORD originalColor
		);

		void Render_Controller_Row();
};

extern Render_ render_layout;

// -------------------- Dirty flag --------------------
class Dirty_flag
{
	public:
		Dirty_flag();

		// Initialize dirty flags based on buffer size
		void initialize(size_t buffer_size);

		// Mark specifiv line dirty
		void markDirty(size_t line);

		// Mark range dirty (used for scroll / resize)
		void markRangeDirty(size_t start, size_t end);

		// check if line is dirty
		bool isDirty(size_t line) const;

		// Mark line clean after rendering
		void markClean(size_t line);

		// Resize dirty vector if buffer grows
		void resize(size_t new_size);

	private:
		std::vector<bool> dirty_lines;
};

extern Dirty_flag dirty_flag;
// ----------------------------------------------------
#endif //RENDER_H
