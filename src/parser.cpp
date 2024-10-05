#include "parser.hpp"

// Statements:
// stmt --> if:block | while:block




// If - Statement:
// if:block --> 'if' + cond + ':' + '\n' + block + (elif:block | else:block | '')
// elif:block --> 'elif' + cond + ':' + '\n' + block + (elif:block | else:block | '')
// else:block --> 'else' + ':' + '\n' + block

// While - Statemnt:
// while:block

// code blocks
// block --> 


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



// Create also the lexer with the source file
pyc::Parser::Parser(std::istream &source) : lexer_(source)
{
	
}













