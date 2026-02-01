#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

struct EditorState{
	int row;
	int col;
	int cursor_line;
	int cursor_col;
	WORD originalColor;
	int scroll_offset;
	int h_scroll;
	int preferred_col;
	bool redraw;
	bool editor_core_running;
};

extern EditorState state;

class Editor{
	public:
		Editor();
		void Editor_run();
		void Reset_view_after_load();
};

extern Editor editor;

#endif // EDITOR_H
