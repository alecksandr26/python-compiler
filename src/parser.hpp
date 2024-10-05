#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include <fstream>
#include "lexer.hpp"

namespace pyc {
	class Parser {
	private:
		Lexer lexer_;
		
	public:
		Parser(std::istream &source);
	};
}

#endif


