#ifndef PARSER_INCLUDED
#define PARSER_INCLUDED

#include <fstream>
#include "lexer.hpp"
#include "ident.hpp"
#include "ast.hpp"


namespace pyc {
	class Parser {
	private:
		Lexer lexer_;
		Token &curr_token_;
		AST ast_;
		Ident curr_ident_;
		
		void advance_(void);

		// To match the next token, 
		bool match_(Token token);

		// To parse any type of expression
		TNode *term_(void);     // It will return a parsed term, coulde be function call
		TNode *factor_(void); // It will return a parsed factor
		TNode *expr_(void); // it will return a tree with the parsed expression

		// To parse a simple function call with params, and then return the node
		TNode *func_call_(Token *id_token);
		
		// To parse a line, goind deep with tis expressions, factor until reach the term
		void stmnt_(void);

		// To parse the different types of initialization '='
		void init_(Token *id_token);

		// It will parse a new code block
		void block_(void);
		
		// To parser block operations: like return, continue, break, pass
		void block_op_stmnt_(void);
		
		//  It will parse a if, elif, else statements, with its block, and cond expression
		void if_stmnt_(void);

		//  It will parse while statment, with its block and and condition expression
		void while_stmnt_(void);
		
		// To parse this types of function
		void func_stmnt_(void);

		// To parse the arguments and the fucntion call

		void func_call_stmnt_(Token *id_token);

		
	public:
		Parser(std::istream &source);

		// It will parse the entire main block function, here the statemnts don't a have any
		// level of identation
		void parse(void);
		
		const AST &get_ast(void) const;
	};
}

#endif


