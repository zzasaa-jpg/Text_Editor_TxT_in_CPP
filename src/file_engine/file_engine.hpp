#ifndef FILE_ENGINE_HPP
#define FILE_ENGINE_HPP

#include <string>

class File_Engine{
	public:
		File_Engine();
		bool Save(const std::string& path);
		bool Load(const std::string& path);
};

extern File_Engine file_engine_;

#endif // FILE_ENGINE_HPP
