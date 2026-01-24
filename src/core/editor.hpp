#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

struct EditorState{
	int row;
	int col;
	int cursor_line;
	int cursor_col;
	WORD originalColor;
};

class Editor{
	public:
		void Editor_run();
	private:
		EditorState state;		
};

#endif // EDITOR_H
