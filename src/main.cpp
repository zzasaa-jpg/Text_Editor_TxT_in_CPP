#include "./core/editor.hpp"
#include "./file_controller/file_controller.hpp"

int main(int argc, char* argv[])
{
	if(argc > 1)
		contrl_state.file_path = argv[1];
	else
		contrl_state.file_path = "";

	Editor editor;
	editor.Editor_run();
	return 0;
}
