#include <iostream>
#include <fstream>
#include <cassert>


#include "tag.hpp"
#include "word.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "lexer.hpp"

using namespace pyc;

int main(void)
{
	std::ifstream source("./test.py");



	if (!source.is_open()) {
		std::cerr << "The is not open" << std::endl;
		assert(0);
	}
	
	Lexer lexer(source);

	while (lexer.is_token_available()) {
		const Token &token = lexer.next_token();

		switch (token.get_type()) {
		case TokenType::IDENTIFIER:
		case TokenType::KEYWORD: {
			const Word &word = static_cast<const Word&>(token);
			std::cout << word << std::endl;
		}
			break;

		case TokenType::NUMBER: {
			if (token.get_tag() == TagType::INTEGER) {
				const Integer &inter = static_cast<const Integer &>(token);
				std::cout << inter << std::endl;
			} else {
				const Real &real = static_cast<const Real &>(token);
				std::cout << real << std::endl;
			}
			break;
		} default:
			std::cout << token << std::endl;
			break;
			
		}
		

	}

	source.close();
	
	return 0;
}



