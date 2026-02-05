#include "./file_engine.hpp"
#include "../buffer/buffer.hpp"
#include "../file_controller/file_controller.hpp"

#include <fstream>

File_Engine file_engine_;
File_Engine::File_Engine(){};

// Save file function ------------------------------------------------------------
bool File_Engine::Save(const std::string& path)
{
	std::ofstream out(path);

	if(!out.is_open())
	{
		contrl_state.Error = true;
		file_controller_.range_of_mes("Write failed", FOREGROUND_RED);
		return false;
	}

	auto& buf = g_Buffer.get_buffer();

	for (auto& line : buf)
	{
		out << line << '\n';
	}

	contrl_state.modified = false;
	return true;
}
// -------------------------------------------------------------------------------

// Load file function ------------------------------------------------------------
bool File_Engine::Load(const std:: string& path)
{
	std::ifstream file(path);

	if(!file.is_open()){
		contrl_state.Error = true;
		file_controller_.range_of_mes("Open failed", FOREGROUND_RED);
		return false;
	}

	auto& buf = g_Buffer.get_buffer();
	buf.clear();

	std::string line;
	while (std::getline(file, line))
	{
		buf.push_back(line);
	}

	// Empty file -> at least one line
	if(buf.empty())
		buf.push_back("");

	contrl_state.modified = false;

	return true;
}
// -------------------------------------------------------------------------------
