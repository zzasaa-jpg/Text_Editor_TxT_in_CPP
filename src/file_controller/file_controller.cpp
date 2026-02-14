#include "./file_controller.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"
#include "../file_engine/file_engine.hpp"

#include <iostream>
#include <windows.h>

File_Controller_state contrl_state;
File_Controller file_controller_;
bool contrl_controller_dirty = false;
File_Controller::File_Controller() {};

void File_Controller::Clear_Buffer()
{
	contrl_state.controller_buffer.clear();
	contrl_state.controller_col = 1;
	contrl_controller_dirty = true;
}

void File_Controller::Execute_Command()
{
	Parse_Command();
	Clear_Buffer();
	//contrl_state.controller_ = false;
	//state.redraw = true;
}

void File_Controller::range_of_mes(std::string mes, WORD color_set)
{
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, color_set);
	contrl_state.controller_buffer = mes;
	contrl_state.controller_col = 1 + contrl_state.controller_buffer.size();
	file_controller_.Render_Controller();
	SetConsoleTextAttribute(g_Terminal_Context.hStdOut, state.originalColor);
	contrl_state.Error = true;
	contrl_controller_dirty = true;
}

void File_Controller::Parse_Command()
{
	auto& cmd = contrl_state.controller_buffer;

	if(cmd == "q"){
		if(contrl_state.modified)
		{
			contrl_state.Error = true;
			range_of_mes("Unsaved changes! Use :q!", FOREGROUND_RED);
			return;
		}
		Command_Quit();
	}

	else if(cmd == "q!")
	{
		contrl_state.modified = false;
		contrl_state.quit = true;
		Command_Quit();
	}

	else if (cmd == "w")
	{
		if(contrl_state.file_path.empty())
		{
			contrl_state.Error = true;
			range_of_mes("No file name", FOREGROUND_RED);
			return;
		}

		if(file_engine_.Save(contrl_state.file_path))
		{
			contrl_state.controller_ = false;
			state.redraw = true;
			contrl_state.modified = false;
		}
	}

	else if (cmd.size() > 2 && cmd[0] == 'w' && cmd[1]==' ')
	{
		contrl_state.file_path = cmd.substr(2);
		file_engine_.Save(contrl_state.file_path);
		contrl_state.controller_ = false;
		state.redraw = true;
		contrl_state.modified = false;
	}

	else if(cmd.size() > 2 && cmd[0] == 'o' && cmd[1]==' ')
	{
		contrl_state.file_path = cmd.substr(2);
		if (file_engine_.Load(contrl_state.file_path))
		{
			editor.Reset_view_after_load();
			contrl_state.controller_ = false;
			state.redraw = true;
		}
		file_controller_.Clear_Buffer();
		contrl_state.modified = false;
	}

	else
	{
		contrl_state.Error = true;
		range_of_mes("Invalid Cmd", FOREGROUND_RED);
	}
}

void File_Controller::Command_Quit()
{
	state.editor_core_running = false;
}

void File_Controller::Render_Controller()
{
	contrl_state.controller_row = state.row - 1;
	terminal.move_cursor(
		g_Terminal_Context.hStdOut,
		contrl_state.controller_row, 0
	);

	std::string empty(state.col, ' ');
	std::cout << empty;

	terminal.move_cursor(
		g_Terminal_Context.hStdOut,
		contrl_state.controller_row, 0
	);

	std::cout << ":" << contrl_state.controller_buffer;

	terminal.move_cursor(
		g_Terminal_Context.hStdOut,
		contrl_state.controller_row,
		contrl_state.controller_col
	);

	std::cout.flush();
	contrl_controller_dirty = false;
}
