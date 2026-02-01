#include "./buffer.hpp"

#include <iostream>

Buffer g_Buffer;

Buffer::Buffer(){};

std::vector<std::string>& Buffer::get_buffer(){
	return buffer;
}
