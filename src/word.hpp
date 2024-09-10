#ifndef WORD_INCLUDED
#define WORD_INCLUDED

#include <string>
#include "token.hpp"

namespace pyc {
	
	class Word : public Token {
	private:
		std::string lexeme;
		
	public:
		Word(const std::string &lexeme, const Token &token);
	};
}

#endif


