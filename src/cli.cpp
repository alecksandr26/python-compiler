#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <regex>


struct {
	std::string output_file, input_file;
} compiler_params = {
	// By default de compiler will generate a.out as the default output file
	.output_file = "a.out",
	.input_file = "",
};


int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);

		if (arg == "-o") {
			assert(i + 1 < argc && "CLI: Error compiler needs to have an output file");
			compiler_params.output_file = std::string(argv[i + 1]);
			i++;	// Move to the next
		} else {
			compiler_params.input_file = std::string(argv[i]);
			std::regex pattern(R"(\.py$)");

			assert(std::regex_search(compiler_params.input_file, pattern)
			       && "CLI: Error compiler needs to have a valid input file");
		}
	}

	assert(compiler_params.input_file != ""
	       && "CLI: Error compiler needs to have an input file");
	
	
	return 0;
}







