#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <string>

class Buffer{
	public:
		Buffer();
		void create_buffer();
	private:
		std::vector<std::string> buffer;
};

#endif // BUFFER_H
