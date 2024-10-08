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

		// To match the next token
		bool match_(const Token &token);

		void term_(void);
		
		void factor_(void);
		
		void expr_(void);

		// To parse a line, goind deep with tis expressions, factor until reach the term
		void stmnt_(void);

		// It will parse a block of function
		void block_(void);

		//  It will parse a if, elif, else statements, with its block, and cond expression
		void if_stmnt_(void);

		//  It will parse while statment, with its block and and condition expression
		void while_stmnt_(void);
		
		// To parse this types of function
		void func_stmnt_(void);
		
		// It will parse the entire main block function, here the statemnts don't a have any
		// level of identation
		void program_(void);
		
	public:
		Parser(std::istream &source);
		
		const AST &build_ast(void);
	};
}

#endif


