#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include <fstream>
#include "lexer.hpp"
#include "ast.hpp"


namespace pyc {
	class Parser {
	private:
		Lexer lexer_;
		Token &curr_token_;
		AST ast_;
		
		void advance_(void);
	public:
		Parser(std::istream &source);
		
		const AST &build_ast(void);
	};
}

#endif


