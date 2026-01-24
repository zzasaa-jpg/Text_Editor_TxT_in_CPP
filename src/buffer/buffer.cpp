#include "./buffer.hpp"

#include <iostream>

Buffer g_Buffer;

Buffer::Buffer(){
	buffer.push_back(" ");       
}

void Buffer::create_buffer(){
	//std::cout << "Buffer Created" << std::endl;
}

std::vector<std::string>& Buffer::get_buffer(){
	return buffer;
}
