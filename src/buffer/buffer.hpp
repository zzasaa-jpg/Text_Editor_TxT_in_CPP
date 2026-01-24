#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>

class Buffer{
	public:
		Buffer();
		void create_buffer();
		std::vector<std::string>& get_buffer();
	private:
		std::vector<std::string> buffer;
};

extern Buffer g_Buffer;

#endif // BUFFER_H
