#include <cassert>
#include "ast.hpp"


using namespace pyc;

const std::string pyc::TNodeType::tnode_types_str[TNODETYPE_COUNT] = {
	"term",
	"call func",
	"operation",
	"code block",
	"if statement",
	"while statement",
	"for statement",
	"func statement",
	"block operation",
};

pyc::AST::AST(void)
{
	curr_block_ = program_ = new TNodeBlock();
	
}


void pyc::AST::set_new_block(TNodeBlock *new_block)
{
	assert(new_block != NULL && "Can't be null");
	assert(curr_block_ != NULL && "It should have a code block");
	blocks_.push(curr_block_);
	curr_block_ = new_block;
}


void pyc::AST::append_new_stmt(TNode *stmnt)
{
	assert(stmnt != NULL && "Can't be null");
	assert(curr_block_ != NULL && "It should have a code block");
	curr_block_->stms.push_back(stmnt);
}

void pyc::AST::pop_block(void)
{
	curr_block_ = blocks_.top();
	blocks_.pop();
}
