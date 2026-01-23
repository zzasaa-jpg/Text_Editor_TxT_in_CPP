#include "./editor.hpp"
#include "../terminal/terminal.hpp"
#include "../buffer/buffer.hpp"

Terminal terminal;

void Editor_boot(){
	EditorState state;

	state.row =
		g_Terminal_Context.csbi.srWindow.Bottom - 
		g_Terminal_Context.csbi.srWindow.Top + 1;
	state.col =
		g_Terminal_Context.csbi.srWindow.Right - 
		g_Terminal_Context.csbi.srWindow.Left + 1;

	state.originalColor = g_Terminal_Context.csbi.wAttributes;
	state.cursor_line = 0, state.cursor_col = 0;

	terminal.Clear_Screen(
			g_Terminal_Context.hStdOut,
			&g_Terminal_Context.csbi,
			state.row, state.col	
	);
	while(true){
		ReadConsoleInput(
				g_Terminal_Context.hIn,
				&g_Terminal_Context.input,
				1, &g_Terminal_Context.read
		);
	}
}

void Editor::Editor_run(){
	Buffer buffer;
	buffer.create_buffer();
	terminal.Terminal_init();
	Editor_boot();
}


