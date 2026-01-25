#include "./editor.hpp"
#include "../terminal/terminal.hpp"
#include "../buffer/buffer.hpp"
#include "../render/render_.hpp"
#include "../scroll/scroll.hpp"
#include "../input/input_Key_.hpp"

void Editor_boot(){
	EditorState state{};

	state.row =
		g_Terminal_Context.csbi.srWindow.Bottom - 
		g_Terminal_Context.csbi.srWindow.Top + 1;
	state.col =
		g_Terminal_Context.csbi.srWindow.Right - 
		g_Terminal_Context.csbi.srWindow.Left + 1;

	state.originalColor = g_Terminal_Context.csbi.wAttributes;
	state.cursor_line = 0, state.cursor_col = 0,
	state.scroll_offset = 0, state.h_scroll = 0,
	state.preferred_col = 0, state.redraw = false;

	terminal.Clear_Screen(
		g_Terminal_Context.hStdOut,
		&g_Terminal_Context.csbi,
		state.row, state.col
	);

	render_layout.render_(
		g_Terminal_Context.hStdOut,
		g_Buffer.get_buffer(), state.row, state.col,
		state.cursor_line, state.cursor_col,
		state.originalColor
	);

	terminal.move_cursor(g_Terminal_Context.hStdOut, 0, 2);

	while(true){
		ReadConsoleInput(
			g_Terminal_Context.hIn,
			&g_Terminal_Context.input,
			1, &g_Terminal_Context.read
		);
		
		// ----------------------- Key input -----------------------
		input.range_of_input_functions(
			state.cursor_line, state.cursor_col,
			state.preferred_col 
		);
		//----------------------------------------------------------

		// -------------------- Vertical_Scroll --------------------
		scroll.Vertical_Scroll(
			state.cursor_line,
			state.scroll_offset,
			state.row
		);
		// ---------------------------------------------------------

		// -------------------- Horizontal_Scroll ------------------
		scroll.Horizontal_Scroll(
			state.cursor_col,
			state.h_scroll,
			state.col		
		);
		// ---------------------------------------------------------

		if(!state.redraw){
			render_layout.ReDraw(
				state.row, state.col,
				state.cursor_line, state.cursor_col,
				state.scroll_offset, state.h_scroll,
				state.originalColor
			);
		}
	}
}

void Editor::Editor_run(){
	Buffer buffer;
	buffer.create_buffer();
	terminal.Terminal_init();
	Editor_boot();
}
