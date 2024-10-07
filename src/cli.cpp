// cli.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cassert>
#include <regex>
#include "lexer.hpp"
#include "parser.hpp"

struct {
    std::string output_file, input_file;
} compiler_params = {
    // By default the compiler will generate a.out as the default output file
    .output_file = "a.out",
    .input_file = "",
};

int main(int argc, char *argv[])
{
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);

        if (arg == "-o") {
            assert(i + 1 < argc && "CLI: Error compiler needs to have an output file");
            compiler_params.output_file = std::string(argv[i + 1]);
            i++;    // Move to the next
        } else {
            compiler_params.input_file = std::string(argv[i]);
            std::regex pattern(R"(\.py$)");

            assert(std::regex_search(compiler_params.input_file, pattern)
                   && "CLI: Error compiler needs to have a valid input file");
        }
    }

    assert(compiler_params.input_file != ""
           && "CLI: Error compiler needs to have an input file");

    // Open the input file
    std::ifstream inputFile(compiler_params.input_file);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file: " << compiler_params.input_file << std::endl;
        return 1;
    }

    // Read the entire contents of the input file into a string
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string inputString = buffer.str();

    // Create a stringstream from the input string
    std::istringstream source(inputString);

    // Instantiate the lexer with the input source
    pyc::Lexer lexer(source);

    // Optionally, instantiate the parser
    pyc::Parser parser(lexer);

    // Process the input using the lexer and parser
    try {
        // Lexer output
        std::cout << "Lexer Output:\n";
        while (lexer.is_token_available()) {
            auto token = lexer.next_token();
            std::cout << token->to_string() << std::endl;
        }

        // Reset the lexer with the same input for parsing
        std::istringstream sourceForParser(inputString);
        pyc::Lexer lexerForParser(sourceForParser);
        pyc::Parser parser(lexerForParser);

        // Parse the input
        parser.parse();
        std::cout << "Parsing successful." << std::endl;

    } catch (const std::exception &e) {
        std::cerr << "Error while processing: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
