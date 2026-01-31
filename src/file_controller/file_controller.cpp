#include "./file_controller.hpp"
#include "../terminal/terminal.hpp"
#include "../core/editor.hpp"

#include <iostream>

File_Controller_state contrl_state;
File_Controller file_controller_;
File_Controller::File_Controller() {};


void File_Controller::Clear_Buffer()
{
	contrl_state.controller_buffer.clear();
}


void File_Controller::Execute_Command()
{
	Parse_Command();
	Clear_Buffer();
	//contrl_state.controller_ = false;
	//state.redraw = true;
}

void range_of_errors()
{
	
	std::string err = "Invalid Cmd";

	contrl_state.controller_buffer = err;

	file_controller_.Render_Controller();
	contrl_state.Error = true;
}



void File_Controller::Parse_Command()
{
	auto& cmd = contrl_state.controller_buffer;

	if(cmd == "q"){
		contrl_state.quit = true;
		Command_Quit();
	} else if (cmd == "w"){
		contrl_state.modified = false;
	}
	else {
		contrl_state.Error = true;
		range_of_errors();
	}
}

void File_Controller::Command_Quit()
{
	state.editor_core_running = false;
}


void File_Controller::Render_Controller()
{
    int controller_row = state.row - 1;

    // Move to controller row
    terminal.move_cursor(g_Terminal_Context.hStdOut, controller_row, 0);

    // Clear line
    std::string empty(state.col, ' ');
    std::cout << empty;

    // Move back and print ":" + buffer
    terminal.move_cursor(g_Terminal_Context.hStdOut, controller_row, 0);
    std::cout << ":" << contrl_state.controller_buffer;

    // Move cursor to end of buffer
    int cursor_col = 1 + contrl_state.controller_buffer.size();
    terminal.move_cursor(g_Terminal_Context.hStdOut, controller_row, cursor_col);

    std::cout.flush();

    state.redraw = false;
}

