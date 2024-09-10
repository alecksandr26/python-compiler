#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

#include <fstream>
#include <map>

#include "token.hpp"

namespace pyc {
	class Lexer {
	private:
		std::ifstream &source_;
		std::map<std::string, Token> keywors_;
		char peek_;
		int line_;
		

		void readch(void);
		bool expectch(char ch);
		
	public:
		Lexer(std::ifstream &source);
		Token next_token(void);
	};  
}

#endif





