#include "./editor.hpp"
#include "../terminal/terminal.hpp"
#include "../buffer/buffer.hpp"
#include "../render/render_.hpp"
#include "../scroll/scroll.hpp"
#include "../input/input_Key_.hpp"
#include "../file_controller/file_controller.hpp"
#include "../file_engine/file_engine.hpp"

EditorState state{};
Editor editor;
Editor::Editor() {};
void Editor_boot(){
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

	// Clear buffer before loading
	//g_Buffer.get_buffer().clear();

	// Load the file if provided
	if (!contrl_state.file_path.empty())
	{
		if(file_engine_.Load(contrl_state.file_path))
		{
			editor.Reset_view_after_load();
		}
		file_controller_.Clear_Buffer();
		contrl_state.modified = false;
	}
	terminal.Clear_Screen(
		g_Terminal_Context.hStdOut,
		&g_Terminal_Context.csbi,
		state.row, state.col
	);

	render_layout.render_(
		g_Terminal_Context.hStdOut,
		g_Buffer.get_buffer(), state.row, state.col,
		state.cursor_line, state.cursor_col,
		state.scroll_offset, state.h_scroll,
		state.originalColor
	);

	terminal.move_cursor(g_Terminal_Context.hStdOut, 0, 2);

	// Editor core loop --------------------------------------------
	state.editor_core_running = true;
	while(state.editor_core_running){
		ReadConsoleInput(
			g_Terminal_Context.hIn,
			&g_Terminal_Context.input,
			1, &g_Terminal_Context.read
		);
		
		// --------------------- Resize Window ---------------------
		input.ReSize_Window(
			g_Terminal_Context.hStdOut,
			&g_Terminal_Context.csbi,
			state.row, state.col,
			state.cursor_line, state.cursor_col
		);
		// ---------------------------------------------------------

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

		// ----------------- Editor redraw handling ----------------
		if(state.redraw)
		{
			if(!contrl_state.controller_)
			{
				// Only redraw editor if NOT in controller mode
				render_layout.ReDraw(
					state.row, state.col,
					state.cursor_line, state.cursor_col,
					state.scroll_offset, state.h_scroll,
					state.originalColor
					);
			} else {
				// If controller mode is active, redraw only the controller row
				file_controller_.Render_Controller();
			}
		}
		// ---------------------------------------------------------
	}
	// Editor core loop end ----------------------------------------
}

void Editor::Editor_run()
{
	Buffer buffer;
	terminal.Terminal_init();
	Editor_boot();
}

void Editor::Reset_view_after_load()
{
	state.cursor_line = 0, state.cursor_col = 0,
	state.scroll_offset = 0, state.h_scroll = 0,
	state.preferred_col = 0, state.redraw = true;
}
