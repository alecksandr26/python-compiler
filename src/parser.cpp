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

// TODO: Build proper expections

// Create also the lexer with the source file
pyc::Parser::Parser(std::istream &source) : lexer_(source), curr_token_(NULL)
{
	
}

void pyc::Parser::advance_(void)
{
	if (lexer_.is_token_available()) {
		curr_token_ = &lexer_.next_token();
		return;
	}
	
	assert(0 && "There isn't more tokens");
}

bool pyc::Parser::match_(Token token)
{
	// Asserts the actual token
	return (token.get_type() == curr_token_->get_type()) and (token.get_tag() == curr_token_->get_tag());
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

	node->token = curr_token_;
	
	// Parse the params
	advance_();
	if (!match_(Token::lparen)) {
		std::cerr << "Error: expect to have an left parentheses '(' for a function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}
	
	advance_();
	while (match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
		node->params.push_back(curr_token_);
		
		advance_();
		if (match_(Token::comma)) {
			advance_();
			if (!match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
				std::cerr << "Error: expect to have another param for"
					" a function definition in line "
					  << lexer_.get_line() << std::endl;
				assert(0);
			}
		}
	}

	if (!match_(Token::rparen)) {
		std::cerr << "Error: expect to have an right parentheses ')' for a function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}


	advance_();
	if (!match_(Token::two_points)) {
		std::cerr << "Error: expect to have an initialization of a code block ':' for a "
			"function definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}
	

	// Append the new code block
	node->block = new TNodeBlock();
	ast_.append_new_stmt(node);
	ast_.set_new_block(node->block);
	
	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();
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
	node->token = curr_token_;
	ast_.append_new_stmt(node);
	node->expr = NULL;
	
	advance_();
	if (node->token->get_tag() == TagType::RETURN)
		node->expr = cond_op_expr_();

}

TNode *pyc::Parser::term_(void)
{
	TNode *term = NULL;
	switch (curr_token_->get_type()) {
	case TokenType::NUMBER:
		switch (curr_token_->get_tag()) {
		case TagType::INTEGER:
		case TagType::REAL:
			{
				term = new TNodeTerm();
				// Weird fucked up cast expression
				(static_cast<TNodeTerm *>(term))->token = curr_token_;
			}
			break;
		default:
			std::cerr << "Error: Uknow number in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
			break;
		}

		advance_();
		break;
	case TokenType::STRING:
		{
			// TODO: Parse the functionality of formating strings in python here
			term = new TNodeTerm();
			// Weird fucked up cast expression
			(static_cast<TNodeTerm *>(term))->token = curr_token_;
			advance_();
		}		
		break;
	case TokenType::IDENTIFIER: // Could be a function call
		{
			const Token *id_token = curr_token_;
			advance_();
			if (!match_(Token::lparen)) { // if it is not a function call then it should be a var
				term = new TNodeTerm();
				(static_cast<TNodeTerm *>(term))->token = id_token;
				advance_();
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
		term = cond_op_expr_();
		if (!match_(Token::rparen)) {
			std::cerr << "Error: Expecting a close parem ')' closing the expression in line "
				  << lexer_.get_line() << std::endl;
			assert(0);
		}
		advance_();
		
		break;
	default:
		std::cerr << "Error: Uknow token term in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
		break;
	};

	return term;
}

TNode *pyc::Parser::exp_(void)
{
	TNode *node_exp = term_();

	assert(node_exp != NULL && "Can't be null baby");

	// we have a while loop because of what happend if '2 ** 3 ** 4'
	while (curr_token_->get_type() == TokenType::OPERATOR
	       and curr_token_->get_tag() == TagType::POW) {
		// Each time we find out an operation we need to find out another term

		// Captures the current node 
		TNode *temp = node_exp;

		node_exp = new TNodeOp();
		static_cast<TNodeOp *>(node_exp)->token = curr_token_;
		static_cast<TNodeOp *>(node_exp)->left = temp;

		// And find out the other pointer it could be another expression we don't know it jijiji
		advance_();
		static_cast<TNodeOp *>(node_exp)->right = term_();
	}


	return node_exp;
}

TNode *pyc::Parser::factor_(void)
{
	TNode *node_fac = exp_();
	
	assert(node_fac != NULL && "Can't be null baby");

	// Solving operations hierarchy for '*', '/',  and '%', we need another layer for '**'
	// Again while because of the chain operations '2 * 1 / 2 % 3'
	while (curr_token_->get_type() == TokenType::OPERATOR
	       and (curr_token_->get_tag() == TagType::MUL
		    or curr_token_->get_tag() == TagType::DIV
		    or curr_token_->get_type() == TagType::MOD)) {
		TNode *temp = node_fac;

		node_fac = new TNodeOp();
		static_cast<TNodeOp *>(node_fac)->token = curr_token_;
		static_cast<TNodeOp *>(node_fac)->left = temp;

		// So yeas again recursion my friend
		advance_();
		static_cast<TNodeOp *>(node_fac)->right = exp_();
	}

	return node_fac;
}

TNode *pyc::Parser::expr_(void)
{
	TNode *node_expr = factor_();
	
	assert(node_expr != NULL && "Can't have a node expr null");

	// Basically the same frame of code
	while (curr_token_->get_type() == TokenType::OPERATOR
	       and (curr_token_->get_tag() == TagType::ADD
		    or curr_token_->get_tag() == TagType::SUB)) {
		TNode *temp = node_expr;

		node_expr = new TNodeOp();
		static_cast<TNodeOp *>(node_expr)->token = curr_token_;
		static_cast<TNodeOp *>(node_expr)->left = temp;

		// So yeas again recursion my friend, so fucked up, my brain suffers of damage
		advance_();
		static_cast<TNodeOp *>(node_expr)->right = factor_();
	}

	return node_expr;
}

void pyc::Parser::init_stmnt_(const Token *id_token)
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
	node_init->token = curr_token_;
	node_init->id = node_term;
	
	advance_();
	
	// Parse an expression -> an expression could contain functions calls operations etc ... etc ..
	node_init->expr = cond_op_expr_();
	assert(node_init->expr != NULL && "Should not return an null expression tree");
	ast_.append_new_stmt(static_cast<TNode *>(node_init));
}


TNode * pyc::Parser::func_call_(const Token *id_token)
{
	if (!match_(Token::lparen)) {
		std::cerr << "Error: expect to have an left parentheses '(' "
			"in line " << lexer_.get_line() << std::endl;		
		assert(0);
	}

	TNodeCall *node = new TNodeCall();
	node->token = id_token;
	
	// Parse the arguments
	advance_();
	while (!match_(Token::rparen)) {
		// Here we could parse an entire new expression
		TNode *arg_expr = cond_op_expr_();
		node->args.push_back(arg_expr);
		
		if (match_(Token::comma)) {
			advance_();
			if (match_(Token::rparen)) {
				std::cerr << "Error: expect to have another argument for"
					" a function call in line "
					  << lexer_.get_line() << std::endl;
				assert(0);
			}
		}
	}

	
	// Move to the next token probably an end of line or something like that
	advance_();
	return static_cast<TNode *>(node);
}

void pyc::Parser::func_call_stmnt_(const Token *id_token)
{
	assert(id_token != NULL && "Can't receive a null pointer");
	TNodeCall *node_call = static_cast<TNodeCall *>(func_call_(id_token));
	assert(node_call != NULL && "Can't receive a null pointer");
	ast_.append_new_stmt(static_cast<TNode *>(node_call));
}


TNode *pyc::Parser::cond_cmp_expr_(void)
{
	TNode *node_cmp_expr = expr_();

	// Parse the comparision expressions
	while (curr_token_->get_type() == TokenType::OPERATOR
	       and (curr_token_->get_tag() == TagType::EQ
		    or curr_token_->get_tag() == TagType::NEQ
		    or curr_token_->get_tag() == TagType::GT
		    or curr_token_->get_tag() == TagType::GE
		    or curr_token_->get_tag() == TagType::LT
		    or curr_token_->get_tag() == TagType::LE)) {
		TNode *temp = node_cmp_expr;

		node_cmp_expr = new TNodeOp();
		static_cast<TNodeOp *>(node_cmp_expr)->token = curr_token_;
		static_cast<TNodeOp *>(node_cmp_expr)->left = temp;
		
		advance_();
		static_cast<TNodeOp *>(node_cmp_expr)->right = expr_();
	}
	
	return node_cmp_expr;
}

TNode *pyc::Parser::cond_op_expr_(void)
{

	TNode *node_op_expr;
	if (curr_token_->get_tag() == TagType::NOT) {
		do {
			node_op_expr = new TNodeOp();
			// The not keyword is different and it needs to follow this order
			static_cast<TNodeOp *>(node_op_expr)->left = NULL;
			static_cast<TNodeOp *>(node_op_expr)->token = curr_token_;
			
			// Advance to the next expression
			advance_();
			static_cast<TNodeOp *>(node_op_expr)->right = cond_op_expr_();
		} while (curr_token_->get_tag() == TagType::NOT);
	} else {
		node_op_expr = cond_cmp_expr_();
		while (curr_token_->get_type() == TokenType::OPERATOR
		       and (curr_token_->get_tag() == TagType::AND
			    or curr_token_->get_tag() == TagType::OR)) {
			TNode *temp = node_op_expr;

			node_op_expr = new TNodeOp();
			
			static_cast<TNodeOp *>(node_op_expr)->token = curr_token_;
			static_cast<TNodeOp *>(node_op_expr)->left = temp;
		
			advance_();
			static_cast<TNodeOp *>(node_op_expr)->right = cond_cmp_expr_();
		}
	}

	return node_op_expr;
}

void pyc::Parser::if_stmnt_(void)
{
	TNodeIf *node = new TNodeIf();
	node->token = curr_token_; // Catch the if statement

	// Parse the conditional expression
	advance_();
	node->cond = cond_op_expr_();

	// Sincie it is a new code block
	if (!match_(Token::two_points)) {
		std::cerr << "Error: expect to have an initialization of a code block ':' for a "
			"if definition in line "
			  << lexer_.get_line() << std::endl;
		assert(0);
	}

	// Create the new code block
	node->block = new TNodeBlock();
	ast_.append_new_stmt(node);
	ast_.set_new_block(node->block);
	
	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);

	advance_();
}

void pyc::Parser::stmnt_(void)
{
	// Can't be an unknow token at this stage
	assert(curr_token_->get_type() != TokenType::UNKNOWN);

	std::cout << "Next token statement to parse: " << *curr_token_ << std::endl;

	switch (curr_token_->get_type()) {
	case TokenType::KEYWORD:
		// Keywords of an structure
		switch (curr_token_->get_tag()) {
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
		case TagType::IF:
			if_stmnt_();
			break;
		}
		
		break;
	case TokenType::IDENTIFIER:
		// a functon call ?, or initialization of a variable
		{		// New block
			const Token *id_token = curr_token_; // catchs the current token


			// Advance to notice the next token
			advance_();
			if (match_(Token::init)) {
				init_stmnt_(id_token);
			} else if (match_(Token::lparen)) {
				func_call_stmnt_(id_token);
			} else {
				std::cerr << "Error: expect to have function call or initialization of variable "
					"in line "  << lexer_.get_line() << std::endl;
				assert(0);
			}
		}
		break;
	case TokenType::DELIMITER:
		// A blank line or could be any other thing

		// End of the line
		if (curr_token_->get_tag() == TagType::EOL)
			return;
		break;
	}
}


void pyc::Parser::parse_(void)
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
		if (curr_token_->get_tag() == TagType::IDENT) {
			const Ident *actual_ident = static_cast<const Ident *>(curr_token_);
			if (actual_ident->get_ident_level() < curr_ident_.get_ident_level()) {
				// Move to previous code block by the given identation
				for (long n = curr_ident_.get_ident_level();
				     n > actual_ident->get_ident_level(); n--)
					ast_.pop_block();
				
				
				curr_ident_.set_ident_level(actual_ident->get_ident_level());
			}
			advance_();
		} else {
			for (long n = curr_ident_.get_ident_level(); n > 0; n--)
				ast_.pop_block();
			curr_ident_.set_ident_level(0);
		}
		
		if (match_(Token::end_of_line)) {
			if (!lexer_.is_token_available())
				break;
			continue;
		}
		
		stmnt_();
		
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
	// TODO: Refactor this shit
	if (!lexer_.is_token_available())
		assert(0 && "There isn't tokens to parse");
	
	// Read the first tokenu
	advance_();
	
	// Init the parser
	parse_();
}

const AST &pyc::Parser::get_ast(void) const
{
	return ast_;
}

