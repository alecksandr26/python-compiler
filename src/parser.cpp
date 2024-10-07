

// Statements:
// stmt --> (ident:block(n) + '') + (if:block | while:block | for:block | init | cond)

// If - Statement:
// if:block --> 'if' + cond + ':' + '\n' + block + (elif:block | else:block | '')
// elif:block --> 'elif' + cond + ':' + '\n' + block + (elif:block | else:block | '')
// else:block --> 'else' + ':' + '\n' + block

// While - Statemnt:
// while:block --> while + cond + ":" + '\n' + block

// code blocks
// block --> stmt + ident:block(n - 1) | stmt + '\n' + block


// Conditionals:
// cond --> cond:expr | cond:expr + ('and' | 'or') + cond

// NOTE: For me doesn't make sense have a mix of conditionals expressions with normal arithmetic expressions
// But.. thats how python works
// >>> vat4 = not 4 + 2
// >>> vat4
// False
// >>> vat4 = not 3.14 - 21.3
// >>> vat4
// False
// >>> vat4 = (not 3.14 - 21.3) == False
// >>> vat4
// True
// >>> if 3.15 - 21.3:
// ...     print("This is so fucked up")
// ... 
// This is so fucked up
// >>> vat4 = 3.15 == 53.12 == 10.2
// >>> vat4
// False
// >>> vat4 = 3.15 == 3.15 == 3.15
// >>> vat4
// True
// >>> 

// cond:expr --> expr | expr + ('==' | '!=' | '<' | '<=' | '>' | '>=') + cond:expr | '(' + cond + ')'
//                    | 'not' + cond:expr

// Initialization:
// init --> id:init + '=' + expr 

// Expressions:
// expr --> factor | factor + ('+' | '-') + expr
// factor --> term | term + ('*' | '/' | '%' | '**') + factor
// term --> INT | FLOAT | STRLIT | TRUE | FALSE | NONE | id | '(' + expr + ')'
// INT --> NUMBER | '-' + NUMBER
// FLOAT --> NUMBER + '.' + NUMBER | '-' + NUMBER + '.' + NUMBER

// NOTICE: 'Q', Refers to quotes
// STRLIT --> 'Q' + STR + 'Q'

// Identifier:
// id --> STR + (args | '') | STR + '.' + id
// id:init --> STR | STR + '.' + id:init

// Arguments:
// args --> '(' + arg_params + ')'
// arg_params --> expr | expr + ',' + arg_params

#include <vector>
#include "parser.hpp"
#include "token.hpp"

using namespace pyc;



// Create also the lexer with the source file
pyc::Parser::Parser(std::istream &source) : lexer_(source), curr_token_(Token::unknown), ast_()
{
	
}

void pyc::Parser::advance_(void)
{
	if (lexer_.is_token_available()) {
		curr_token_ = lexer_.next_token();
	}
	
}

const AST &pyc::Parser::build_ast(void)
{
	
	
	return ast_;
}

