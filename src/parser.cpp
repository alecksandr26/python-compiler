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
#include "error.hpp"

using namespace pyc;


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
	if (!match_(Token(TokenType::IDENTIFIER, TagType::ID)))
		throw LogParserError(ERROR_EXPECT_FUNC_NAME, lexer_.get_line());

	node->token = curr_token_;
	
	// Parse the params
	advance_();
	if (!match_(Token::lparen))
		throw LogParserError(ERROR_EXPECT_LPAREM, lexer_.get_line());
	
	advance_();
	while (match_(Token(TokenType::IDENTIFIER, TagType::ID))) {
		node->params.push_back(curr_token_);
		
		advance_();
		if (match_(Token::comma)) {
			advance_();
			if (!match_(Token(TokenType::IDENTIFIER, TagType::ID)))
				throw LogParserError(ERROR_EXPECT_PARAM, lexer_.get_line());
		}
	}

	if (!match_(Token::rparen))
		throw LogParserError(ERROR_EXPECT_RPAREM, lexer_.get_line());


	advance_();
	if (!match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());
	

	// Append the new code block
	node->block = new TNodeBlock();
	ast_.append_new_stmnt(node);
	ast_.set_new_block(node->block);
	
	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();

	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());
	
	advance_();

	// We start parsing the block of code of the if statement
	block_();

	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}

void pyc::Parser::block_op_stmnt_(void)
{
	if (!(match_(Token(TokenType::KEYWORD, TagType::PASS))
	      or match_(Token(TokenType::KEYWORD, TagType::RETURN))
	      or match_(Token(TokenType::KEYWORD, TagType::BREAK))
	      or match_(Token(TokenType::KEYWORD, TagType::CONTINUE))))
		throw LogParserError(ERROR_EXPECT_OP_BLOCK, lexer_.get_line());
	
	TNodeBlockOp *node = new TNodeBlockOp();
	node->token = curr_token_;
	ast_.append_new_stmnt(node);
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
			throw LogParserError(ERROR_UNKNOW_NUMBER_TERM, lexer_.get_line());
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
				// advance_(); Not sure f this think will break the program
			}  else
				term = func_call_(id_token); // Is a function call
		}

		break;

	case TokenType::DELIMITER: // must be an l paren '('
		if (!match_(Token::lparen))
			throw LogParserError(ERROR_EXPECT_LPAREM, lexer_.get_line());
		advance_();
		// Then parse another expression here
		term = cond_op_expr_();
		if (!match_(Token::rparen))
			throw LogParserError(ERROR_EXPECT_RPAREM, lexer_.get_line());
		advance_();
		
		break;

	case TokenType::KEYWORD:
		if (not (curr_token_->get_tag() == TagType::TRUE or curr_token_->get_tag() == TagType::FALSE)) {
			throw LogParserError(ERROR_UNKNOW_BOOL_TERM, lexer_.get_line());
		} else {
			term = new TNodeTerm();
			(static_cast<TNodeTerm *>(term))->token = curr_token_;
			advance_();
		}
		break;
	default:
		throw LogParserError(ERROR_UNKNOW_TERM, lexer_.get_line());
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
		    or curr_token_->get_tag() == TagType::MOD)) {
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
	if (!match_(Token::init))
		throw LogParserError(ERROR_EXPECT_INITALIZATION, lexer_.get_line());
	
	TNodeInit *node_init = new TNodeInit();
	TNodeTerm *node_term = new TNodeTerm();
	node_term->token = id_token;
	node_init->token = curr_token_;
	node_init->id = node_term;
	
	advance_();
	
	// Parse an expression -> an expression could contain functions calls operations etc ... etc ..
	node_init->expr = cond_op_expr_();
	assert(node_init->expr != NULL && "Should not return an null expression tree");
	ast_.append_new_stmnt(static_cast<TNode *>(node_init));
}


TNode *pyc::Parser::func_call_(const Token *id_token)
{
	if (!match_(Token::lparen))
		throw LogParserError(ERROR_EXPECT_LPAREM, lexer_.get_line());


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
			if (match_(Token::rparen))
				throw LogParserError(ERROR_EXPECT_PARAM, lexer_.get_line());
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
	ast_.append_new_stmnt(static_cast<TNode *>(node_call));
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
	node->or_else = NULL;

	// Parse the conditional expression
	advance_();
	node->cond = cond_op_expr_();
	
	// Sincie it is a new code block
	if (!match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());
	
	
	// Create the new code block
	node->block = new TNodeBlock();
	ast_.append_new_stmnt(node);
	ast_.set_new_block(node->block);
	
	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();

	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());
	
	advance_();
	
	// We start parsing the block of code of the if statement
	block_();

	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}

void pyc::Parser::elif_stmnt_(void)
{
	// To process an elif we need to have previously an elif
	TNodeBlock *block = ast_.top_block();

	// Then by having the actual code block the last stmnt must be en if stmnt	
	if (block->stmnts.back()->type != TNodeType::IFBLOCK)
		throw LogParserError(ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK, lexer_.get_line());
	
	TNodeIf *prev_if = static_cast<TNodeIf *>(block->stmnts.back());
	TNodeIf *new_if = NULL;

	while (prev_if->or_else != NULL) {
		if (prev_if->type != TNodeType::IFBLOCK)
			throw LogParserError(ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK, lexer_.get_line());
		prev_if = static_cast<TNodeIf *>(prev_if->or_else);
		if (prev_if->type != TNodeType::IFBLOCK)
			throw LogParserError(ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK, lexer_.get_line());
	}

	// Create a new if node
	prev_if->or_else = new_if = new TNodeIf();
	new_if->or_else = NULL;
	new_if->token = curr_token_;
	
	// Parse the conditional expression
	advance_();
	new_if->cond = cond_op_expr_();

	// Sincie it is a new code block
	if (not match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());
	
	// Create the new code block
	new_if->block = new TNodeBlock();
	ast_.set_new_block(new_if->block);

	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();
	
	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());
	
	advance_();
	
	// We start parsing the block of code of the if statement
	block_();


	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}

void pyc::Parser::else_stmnt_(void)
{
	// To process an elif we need to have previously an elif
	TNodeBlock *block = ast_.top_block();

	// Then by having the actual code block the last stmnt must be en if stmnt
	if (block->stmnts.back()->type != TNodeType::IFBLOCK)
		throw LogParserError(ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK, lexer_.get_line());

	TNodeIf *prev_if = static_cast<TNodeIf *>(block->stmnts.back());
	TNodeBlock *else_block = NULL;

	while (prev_if->or_else != NULL) {
		if (prev_if->type != TNodeType::IFBLOCK) {
			
		}
		prev_if = static_cast<TNodeIf *>(prev_if->or_else);
		if (prev_if->type != TNodeType::IFBLOCK)
			throw LogParserError(ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK, lexer_.get_line());
	}

	// Create a new code block because it is an else statement
	prev_if->or_else = else_block = new TNodeBlock();
	ast_.set_new_block(else_block);

	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();

	if (not match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());
	
	advance_();

	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());

	advance_();
	
	// We start parsing the block of code of the if statement
	block_();
	
	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}


void pyc::Parser::while_stmnt_(void)
{
	TNodeWhile *node = new TNodeWhile();
	node->token = curr_token_;

	// Parse the conditional
	advance_();
	node->cond = cond_op_expr_();

	// Sincie it is a new code block
	if (!match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());
	
	node->block = new TNodeBlock();
	ast_.append_new_stmnt(node);
	ast_.set_new_block(node->block);

	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();

	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());

	advance_();

	// We start parsing the block of code of the for statement
	block_();

	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}

void pyc::Parser::for_stmnt_(void)
{
	TNodeFor *node = new TNodeFor();
	node->token = curr_token_;
	

	// TODO: Investigate the other options
	// Parse the variable iterator, it must be a variable
	advance_();

	
	if (curr_token_->get_tag() != TagType::ID)
		throw LogParserError(ERROR_EXPECT_ITERATOR_VAR, lexer_.get_line());

	node->element = new TNodeTerm();
	(static_cast<TNodeTerm *>(node->element))->token = curr_token_;
	
	advance_();

	if (curr_token_->get_tag() != TagType::IN)
		throw LogParserError(ERROR_EXPECT_IN, lexer_.get_line());
	
	advance_();

	const Token *id_token = curr_token_;
	
	advance_();
	
	if (!match_(Token::lparen)) { // if it is not a function call then it should be a var
		node->iterate = new TNodeTerm();
		(static_cast<TNodeTerm *>(node->element))->token = id_token;
	}  else
		node->iterate = func_call_(id_token); // Is a function call
	
	if (not match_(Token::two_points))
		throw LogParserError(ERROR_EXPECT_COLON_FOR_CODE_BLOCK, lexer_.get_line());

	node->block = new TNodeBlock();
	ast_.append_new_stmnt(node);
	ast_.set_new_block(node->block);

	// Create a new ident level
	curr_ident_.set_ident_level(curr_ident_.get_ident_level() + 1);
	advance_();
	
	if (not match_(Token::end_of_line))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());

	advance_();

	// We start parsing the block of code of the if statement
	block_();

	if (not match_(Token::end_of_line) and not match_(curr_ident_))
		throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());
}

void pyc::Parser::stmnt_(void)
{
	// Can't be an unknow token at this stage
	assert(curr_token_->get_type() != TokenType::UNKNOWN);
	
	// std::cout << "Next token statement to parse: " << *curr_token_ << std::endl;

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
		case TagType::ELIF:
			elif_stmnt_();
			break;
		case TagType::ELSE:
			else_stmnt_();
			break;
		case TagType::WHILE:
			while_stmnt_();
			break;
		case TagType::FOR:
			for_stmnt_();
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
			} else
				throw LogParserError(ERROR_EXPECT_INIT_OR_CALL, lexer_.get_line());
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

void pyc::Parser::block_(void)
{
	if (!match_(curr_ident_)
	    and static_cast<const Ident *>(curr_token_)->get_ident_level() != curr_ident_.get_ident_level())
		throw LogParserError(ERROR_IDENT_NOT_MATCH, lexer_.get_line());
	
	while (lexer_.is_token_available()) {
		if (curr_token_->get_tag() == TagType::IDENT) {
			const Ident *actual_ident = static_cast<const Ident *>(curr_token_);
			if (actual_ident->get_ident_level() < curr_ident_.get_ident_level()) {
				// Move to previous code block by the given identationa
				ast_.pop_block();
				curr_ident_.set_ident_level(actual_ident->get_ident_level());
				return;
			}
			advance_();
		} else {
			if (not match_(Token::end_of_line))
				throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());
			
			ast_.pop_block();
			curr_ident_.set_ident_level(curr_ident_.get_ident_level() - 1);
			return;
		}
		
		if (match_(Token::end_of_line)) {
			advance_();
			continue;
		}

		stmnt_();


		// This is it, because in the case that the parsed stmnt was a block type, its parsing
		// will be eneded in a identation shit
		if (not match_(Token::end_of_line) and not match_(curr_ident_))
			throw LogParserError(ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK, lexer_.get_line());

		
		if (match_(Token::end_of_line) and lexer_.is_token_available())
			advance_();
	}
}

void pyc::Parser::parse(void)
{
	if (!lexer_.is_token_available())
		assert(0 && "There isn't tokens to parse");
	
	while (lexer_.is_token_available()) {
		advance_();
		
		if (match_(Token::end_of_line))
			continue;

		stmnt_();
		
		if (not match_(Token::end_of_line))
			throw LogParserError(ERROR_EXPECT_END_OF_LINE, lexer_.get_line());
	}
}

const AST &pyc::Parser::get_ast(void) const
{
	return ast_;
}

