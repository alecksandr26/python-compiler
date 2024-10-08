#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <regex>

#include "parser.hpp"
#include "lexer.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "ident.hpp"

struct {
	std::string output_file, source_file;
} compiler_params = {
	// By default de compiler will generate a.out as the default output file
	.output_file = "a.out",
	.source_file = "",
};



void test_lexer(std::ifstream &source)
{
	pyc::Lexer lexer(source);

	while (lexer.is_token_available()) {
		const pyc::Token &token = lexer.next_token();

		switch (token.get_type()) {
		case pyc::TokenType::IDENTIFIER:
		case pyc::TokenType::KEYWORD:
		case pyc::TokenType::STRING: {  // Case for strings
			const pyc::Word &word = static_cast<const pyc::Word&>(token);
			std::cout << word << std::endl;
			break;
		}
		case pyc::TokenType::NUMBER: {
			if (token.get_tag() == pyc::TagType::INTEGER) {
				const pyc::Integer &integer = static_cast<const pyc::Integer &>(token);
				std::cout << integer << std::endl;
			} else {
				const pyc::Real &real = static_cast<const pyc::Real &>(token);
				std::cout << real << std::endl;
			}
			break;
		}
		case pyc::TokenType::DELIMITER: {
			if (token.get_tag() == pyc::TagType::IDENT) {
				const pyc::Ident &ident = static_cast<const pyc::Ident &>(token);
				std::cout << ident << std::endl;
				break;
			}
		}
		default:
			std::cout << token << std::endl;
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++) {
		std::string arg(argv[i]);

		if (arg == "-o") {
			assert(i + 1 < argc && "CLI: Error compiler needs to have an output file");
			compiler_params.output_file = std::string(argv[i + 1]);
			i++;	// Move to the next
		} else {
			compiler_params.source_file = std::string(argv[i]);
			std::regex pattern(R"(\.py$)");

			assert(std::regex_search(compiler_params.source_file, pattern)
			       && "CLI: Error compiler needs to have a valid input file");
		}
	}

	assert(compiler_params.source_file != ""
	       && "CLI: Error compiler needs to have an input file");

	// For the moment process with the lexer
	std::ifstream source(compiler_params.source_file);

	// pyc::Parser parser(source);


	// const pyc::AST &ast = parser.build_ast();

	// std::cout << ast << std::endl;

	test_lexer(source);
	
	return 0;
}







