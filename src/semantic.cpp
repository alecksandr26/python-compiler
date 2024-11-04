#include <iostream>
#include <vector>
#include <algorithm> // For std::find

#include "semantic.hpp"

using namespace pyc;

pyc::Semantic::Semantic(const AST &ast) : ast_(ast)
{
	// Std library of python
	Word *func_print_ = new Word("print", Token(TokenType::IDENTIFIER, TagType::ID));
	Word *func_range_ = new Word("range", Token(TokenType::IDENTIFIER, TagType::ID));
	Word *name_var_ = new Word("__name__", Token(TokenType::IDENTIFIER, TagType::ID));
	
	sym_table_[func_print_->get_lexeme()] = {
		.token = func_print_,
		.block = ast_.get_program()
	};

	sym_table_[func_range_->get_lexeme()] = {
		.token = func_range_,
		.block = ast_.get_program()
	};

	sym_table_[name_var_->get_lexeme()] = {
		.token = name_var_,
		.block = ast_.get_program()
	};
}


void pyc::Semantic::analyze(void)
{
	TNodeBlock *program = ast_.get_program();
	
	assert(program != NULL && "Can't be null, should't be a empty program");

	// Sincie a program is an entire block of code it will analyze
	analyze_block(program);
}

void pyc::Semantic::analyze_expr(TNode *expr, TNodeBlock *block)
{
	// Base case
	if (!expr)
		return;

	switch (expr->type) {
	case TNodeType::TERM: 
		{
			// The Term 
			TNodeTerm *term = static_cast<TNodeTerm *>(expr);
			if (term->token->get_type() == TokenType::IDENTIFIER) {
				const Word *word = static_cast<const Word *>(term->token);

				// If the symbol does't exist or if the code block is not equal
				if (sym_table_.count(word->get_lexeme()) == 0
				    or std::find(scopes_.begin(), scopes_.end(),
						 sym_table_[word->get_lexeme()].block) == scopes_.end()) {
					std::cerr << "Definition not found: " << *word << std::endl;
					assert(0);
				}
			}
		}
		break;
	case TNodeType::CALL:
		{
			TNodeCall *call = static_cast<TNodeCall *>(expr);
			assert(call->token->get_type() == TokenType::IDENTIFIER && "Must be a definition");
			
			const Word *word = static_cast<const Word *>(call->token);

			if (sym_table_.count(word->get_lexeme()) == 0
			    or std::find(scopes_.begin(), scopes_.end(),
					 sym_table_[word->get_lexeme()].block) == scopes_.end()) {
				std::cerr << "Definition not found: " << *word << std::endl;
				assert(0);
			}

			// Iterate the expressions or arguments
			for (TNode *expr : call->args)
				analyze_expr(expr, block);
		}
		break;
	case TNodeType::OPER:
		{
			// Recursive case
			TNodeOp *oper = static_cast<TNodeOp *>(expr);
			
			analyze_expr(oper->left, block);
			analyze_expr(oper->right, block);
		}
		break;
	}
}


void pyc::Semantic::analyze_stmnt(TNode *stmnt, TNodeBlock *block)
{
	assert(stmnt != NULL && "Can't be null");
	
	
	switch (stmnt->type) {
	case TNodeType::INIT:
		{
			TNodeInit *init = static_cast<TNodeInit *>(stmnt);
			// Before creating a new symbol analyze the expression
			analyze_expr(init->expr, block);

			assert(init->id->token->get_type() == TokenType::IDENTIFIER);
			const Word *word = static_cast<const Word *>(init->id->token);
			
			// Insert or update the new symbol
			sym_table_[word->get_lexeme()] = {
				.token = init->id->token,
				.block = block
			};
		}
		break;
	case TNodeType::CALL:
		{
			TNodeCall *call = static_cast<TNodeCall *>(stmnt);
			assert(call->token->get_type() == TokenType::IDENTIFIER && "Must be a definition");
			
			const Word *word = static_cast<const Word *>(call->token);

			if (sym_table_.count(word->get_lexeme()) == 0
			    or std::find(scopes_.begin(), scopes_.end(), block) == scopes_.end()) {
				std::cerr << "Definition not found: " << *word << std::endl;
				assert(0);
			}

			// Iterate the expressions or arguments
			for (TNode *expr : call->args)
				analyze_expr(expr, block);
		}
		break;		
	case TNodeType::BLOCK:
		{
			analyze_block(static_cast<TNodeBlock *>(stmnt));
		}
		break;
	case TNodeType::IFBLOCK:
		{
			TNodeIf *if_ = static_cast<TNodeIf *>(stmnt);

			// First analyze the condition statement
			analyze_expr(if_->cond, block);
			
			// Then block of code
			analyze_block(if_->block);

			// If itis not null then analyze the code block or the nested if block
			if (if_->or_else)
				analyze_stmnt(if_->or_else, block);

		}
		break;
	case TNodeType::WHILEBLOCK:
		{
			TNodeWhile *while_ = static_cast<TNodeWhile *>(stmnt);

			// Analyze the condition
			analyze_expr(while_->cond, block);

			// Then the block
			analyze_block(while_->block);
		}
		break;

	case TNodeType::FORBLOCK:
		{
			TNodeFor *for_ = static_cast<TNodeFor *>(stmnt);
			TNodeTerm *id = static_cast<TNodeTerm *>(for_->element);

			const Word *word = static_cast<const Word *>(id->token);

			sym_table_[word->get_lexeme()] = {
				.token = id->token,
				.block = for_->block
			};

			analyze_expr(for_->iterate, block);
			analyze_block(for_->block);
		}
		break;
	case TNodeType::FUNCBLOCK:
		{
			TNodeFunc *func = static_cast<TNodeFunc *>(stmnt);
			const Word *word = static_cast<const Word *>(func->token);
			
			// Insert or Update the symbol

			sym_table_[word->get_lexeme()] = {
				.token = func->token,
				.block = block
			};

			// Insert or Update the params for the new code block

			for (const Token *param : func->params) {
				const Word *word = static_cast<const Word *>(param);
				sym_table_[word->get_lexeme()] = {
					.token = param,
					.block = func->block
				};
			}

			// analyze the block of code
			analyze_block(func->block);
		}
		break;
	}
}



void pyc::Semantic::analyze_block(TNodeBlock *block)
{
	assert(block != NULL && block->type == TNodeType::BLOCK && "Can't null");

	scopes_.push_back(block);
	
	for (TNode *stmnt : block->stmnts)
		analyze_stmnt(stmnt, block);

	scopes_.pop_back();
}



