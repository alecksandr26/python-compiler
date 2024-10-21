#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

#include <fstream>
#include <map>
#include <vector>

#include "token.hpp"
#include "word.hpp"
#include "ident.hpp"

namespace pyc {
	class Lexer {
	private:
		std::istream &source_;
		std::map<std::string, Word> keywords_;
		std::map<long, Ident> identation_;
		std::vector<const Token *> token_seq_;
		char peek_;
		int line_;
		
		void readch(void);
		bool expectch(char ch);
		bool reads_until_finds_something(void);
		
	public:
		Lexer(std::istream &source);
		Lexer(int fd);
		bool is_token_available(void);
		const Token &next_token(void);
		int get_line(void);
		
	};  
}

#endif





