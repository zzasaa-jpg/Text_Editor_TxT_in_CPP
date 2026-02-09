#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <string>
#include <windows.h>

struct File_Controller_state {
	bool modified = false;
	std::string file_path;
	std::string file_store_path;
	bool quit = false;
	bool controller_ = false;
	bool discard_save = false;
	bool Error = false; // Incorrect or wrong cmd or values message to user
	std::string controller_buffer;
	int controller_row;
	int controller_col;
};

extern File_Controller_state contrl_state;

class File_Controller{
	public:
		File_Controller();
		// Rendering
		void Render_Controller();

		// controller_buffer clear
		void Clear_Buffer();

		void range_of_mes(std::string mes, WORD color_set);
		void Execute_Command(); 
		void Parse_Command();

		void Command_Quit();
};

extern File_Controller file_controller_;

#endif // FILE_CONTROLLER_H
