#include "./buffer.hpp"

#include <iostream>

Buffer::Buffer(){
	buffer.push_back(" ");       
}

void Buffer::create_buffer(){
	std::cout << "Buffer Created" << std::endl;
}
