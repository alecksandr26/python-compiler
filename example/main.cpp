#include <iostream>
#include <fstream>
#include <cassert>

#include <fcntl.h>    // for open()
#include <unistd.h>   // for read()

#include "word.hpp"
#include "lexer.hpp"

using namespace pyc;

int main(void)
{
	// std::ifstream source("./test.py");


	// if (source.is_open()) {
	// 	std::cerr << "The is not open" << std::endl;
	// 	assert(0);
	// }

	
	// Lexer lexer(source);
	
	int fd = open("test.py", O_RDONLY); // Open the file in read-only mode
	

	while (lexer.is_token_available()) {
		const Token &token = lexer.next_token();

		switch (token.get_type()) {
		case TokenType::IDENTIFIER:
		case TokenType::KEYWORD: {
			const Word &word = static_cast<const Word&>(token);
			std::cout << word << std::endl;
		}
			break;
		default:
			std::cout << token << std::endl;
			break;
			
		}
		

	}

	source.close();
	
	return 0;
}



