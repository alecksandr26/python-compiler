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
#include <cassert>
#include <iostream>
#include "parser.hpp"
#include "ast.hpp"
#include "token.hpp"

using namespace pyc;

// Create also the lexer with the source file
pyc::Parser::Parser(std::istream &source) : lexer_(source), curr_token_(Token::unknown)
{
	
}

void pyc::Parser::advance_(void)
{
	if (lexer_.is_token_available()) {
		curr_token_ = lexer_.next_token();
		return;
	}
	
	assert(0 && "There isn't more tokens");
}

bool pyc::Parser::match_(Token token)
{
	// Asserts the actual token
	return (token.get_type() == curr_token_.get_type()) and (token.get_tag() == curr_token_.get_tag());
}

void pyc::Parser::func_stmnt_(void)
{
	// Then match the naming
	// Create new function node
	TNodeFunc *node = new TNodeFunc();

	// Parse the name
	advance_();
	if (!match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
		std::cerr << "Error: expect to have a function name for a function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}

	node->token = &curr_token_;
	
	// Parse the params
	advance_();
	if (!match_(Token(TokenType::DELIMITER, TagType::LPAREN))) {
		std::cerr << "Error: expect to have an left parentheses '(' for a function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}
	
	advance_();
	while (match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
		node->params.push_back(&curr_token_);

		advance_();
		if (match_(Token(TokenType::DELIMITER, TagType::COMMA))) {
			advance_();
			if (!match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
				std::cerr << "Error: expect to have another param for"
					" a function definition in line "
					  << lexer_.get_line() << std::endl;
				assert(0);
			}
		}
	}

	if (!match_(Token(TokenType::DELIMITER, TagType::RPAREN))) {
		std::cerr << "Error: expect to have an right parentheses ')' for a function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}


	advance_();
	if (!match_(Token(TokenType::DELIMITER, TagType::TWO_POINTS))) {
		std::cerr << "Error: expect to have an initialization of a code block ':' for a "
			"function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}
	

	// Append the new code block
	node->block = new TNodeBlock();
	ast_.append_new_stmt(node);
	ast_.set_new_block(node->block);
	
}

void pyc::Parser::block_op_stmnt_(void)
{
	if (!(match_(Token(TokenType::KEYWORD, TagType::PASS))
	      or match_(Token(TokenType::KEYWORD, TagType::RETURN))
	      or match_(Token(TokenType::KEYWORD, TagType::BREAK))
	      or match_(Token(TokenType::KEYWORD, TagType::CONTINUE)))) {
		std::cerr << "Error: expect to have at least one operational block statement like 'pass, "
			"return, break, continue' for a code block in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}
	
	TNodeBlockOp *node = new TNodeBlockOp();
	node->token = &curr_token_;
	ast_.append_new_stmt(node);
}

TNode *pyc::Parser::term_(void)
{
	TNode *term = NULL;
	switch (curr_token_.get_type()) {
	case TokenType::NUMBER:
		switch (curr_token_.get_tag()) {
		case TagType::INTEGER:
			break;
		case TagType::REAL:
			break;
		default:
			std::cerr << "Error: Uknow number in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
			break;
		}
		break;
	case TokenType::STRING:
		// TODO: Parse the functionality of formating strings here
		break;

	case TokenType::IDENTIFIER: // Could be a function call
		{
			Token *id_token = &curr_token_;
			advance_();
			if (!match_(Token::lparen)) { // if it is not a function call then it should be a var
				// TODO: you left it here
				
			}  else
				term = func_call_(id_token); // Is a function call
		}

		break;

	case TokenType::DELIMITER: // must be an l paren '('
		if (!match_(Token::lparen)) {
			std::cerr << "Error: Uknow token term in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
		}
		advance_();
		// Then parse another expression here
		term = expr_();
		break;
	default:
		std::cerr << "Error: Uknow token term in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
		break;
	};

	return term;
}

TNode *pyc::Parser::factor_(void)
{
	TNode *node_fac = term_();
	
	assert(node_fac != NULL && "Can't be null baby");

	return node_fac;
}

TNode *pyc::Parser::expr_(void)
{
	TNode *node_expr = factor_();
	
	assert(node_expr != NULL && "Can't have a node expr null");

	return node_expr;
}

void pyc::Parser::init_(Token *id_token)
{
	assert(id_token != NULL && "Can't receive null pointers");
	if (!match_(Token::init)) {
		std::cerr << "Error: expect to have an initialization operation '=', "
			"in line " << lexer_.get_line() << std::endl;
		assert(0);
	}
	
	TNodeInit *node_init = new TNodeInit();
	TNodeTerm *node_term = new TNodeTerm();
	node_term->token = id_token;
	node_init->token = &curr_token_;
	node_init->id = node_term;
	
	advance_();
	
	// Parse an expression -> an expression could contain functions calls operations etc ... etc ..
	node_init->expr = expr_();
	assert(node_init->expr != NULL && "Should not return an null expression tree");
	ast_.append_new_stmt(static_cast<TNode *>(node_init));
}


TNode * pyc::Parser::func_call_(Token *id_token)
{
	// TODO: you left it here
}

void pyc::Parser::func_call_stmnt_(Token *id_token)
{
	// TODO: you left it here
}

void pyc::Parser::stmnt_(void)
{
	// Can't be an unknow token at this stage
	assert(curr_token_.get_type() != TokenType::UNKNOWN);

	std::cout << "Next token statement to parse: " << curr_token_ << std::endl;

	switch (curr_token_.get_type()) {
	case TokenType::KEYWORD:
		// Keywords of an structure
		switch (curr_token_.get_tag()) {
		case TagType::DEF:
			// A function definition
			func_stmnt_();
			break;
			
		case TagType::PASS:
		case TagType::RETURN:
		case TagType::BREAK:
		case TagType::CONTINUE:
			block_op_stmnt_();
			break;
		}
		
		break;
	case TokenType::IDENTIFIER:
		// a functon call ?, or initialization of a variable
		{		// New block
			Token *id_token = &curr_token_; // catchs the current token

			// Advance to notice the next token
			advance_();
			if (match_(Token::init))
				init_(id_token);
			else if (match_(Token::lparen))
				func_call_(id_token);
			else {
				std::cerr << "Error: expect to have function call or initialization of variable "
					"in line "  << lexer_.get_line() << std::endl;
				assert(0);
			}
		}
		
		break;
	case TokenType::DELIMITER:
		// A blank line or could be any other thing

		// End of the line
		if (curr_token_.get_tag() == TagType::EOL)
			return;
		
		
		break;
	}

}


void pyc::Parser::block_(void)
{
	if (curr_ident_.get_ident_level() > 0 ) {
		if (!match_(curr_ident_)) {
			std::cerr << "Error: identation don't match with the current  "
				"block of code definition in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
			return;
		}
		advance_();
	}

	for ( ; lexer_.is_token_available(); advance_()) {
		if (curr_token_.get_tag() == TagType::IDENT) {
			Ident *actual_ident = static_cast<Ident *>(&curr_token_);
			if (actual_ident->get_ident_level() < curr_ident_.get_ident_level()) {
				// Move to previous code block by the given identation
				long n = curr_ident_.get_ident_level();
				while (n > actual_ident->get_ident_level())
					ast_.pop_block();
				
				curr_ident_.set_ident_level(actual_ident->get_ident_level());
				// Finish with parsing the block
				return;
			}
			advance_();
		}

		if (match_(Token::end_of_line))
			continue;
		
		stmnt_();

		advance_();
		if (!match_(Token::end_of_line)) {
			std::cerr << "Error: expect to have an end of line of a statement "
				"in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
			return;
		}
	}
}



void pyc::Parser::parse(void)
{
	if (!lexer_.is_token_available())
		assert(0 && "There isn't tokens to parse");

	// Read the first tokenu
	advance_();
	
	// Parse the main block
	
	block_();
}

const AST &pyc::Parser::get_ast(void) const
{
	return ast_;
}

