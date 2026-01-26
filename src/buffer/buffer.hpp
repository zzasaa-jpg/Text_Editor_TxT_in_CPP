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
		std::vector<std::string> buffer = {
			"#include <iostream>",
			"using namespace std;",
			" ", "int main(){",
			"      cout << 'Hello world;' << endl;",
			"      return 0;", "}",
			"Hellow this editor written cpp and development tools uses daily, in offices, the other develop mind is boom!Hellow this editor written cpp and development tools uses daily, in offices, the other develop mind is boom!"
		};
};

extern Buffer g_Buffer;

#endif // BUFFER_H
