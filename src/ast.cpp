#include <cassert>
#include "ast.hpp"

using namespace pyc;


const std::string pyc::TNodeType::tnode_types_str[TNODETYPE_COUNT] = {
	"term",
	"call func",
	"operation",
	"initialization",
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


void pyc::AST::append_new_stmnt(TNode *stmnt)
{
	assert(stmnt != NULL && "Can't be null");
	assert(curr_block_ != NULL && "It should have a code block");
	curr_block_->stmnts.push_back(stmnt);
}

void pyc::AST::pop_block(void)
{
	curr_block_ = blocks_.top();
	blocks_.pop();
}

TNodeBlock *pyc::AST::top_block(void)
{
	assert(curr_block_ != NULL && "Needs to be a code block");
	return curr_block_;
}

std::stringstream pyc::ast_fetch_stream_node(TNode *node)
{
	std::stringstream ss;
	
	switch (node->type) {
	case TNodeType::TERM:
		{
			const TNodeTerm *term = static_cast<TNodeTerm *>(node);
			ss << *term;
		}
		break;
	case TNodeType::CALL:
		{
			const TNodeCall *call = static_cast<TNodeCall *>(node);
			ss << *call;
		}
		break;
	case TNodeType::OPER:
		{
			const TNodeOp *op = static_cast<TNodeOp *>(node);
			ss << *op;
		}
		break;
	case TNodeType::INIT:
		{
			const TNodeInit *init = static_cast<TNodeInit *>(node);
			ss << *init;
		}
		break;
	case TNodeType::BLOCK:
		{
			const TNodeBlock *block = static_cast<TNodeBlock *>(node);
			ss << *block;
		}
		break;
	case TNodeType::BLOCKOP:
		{
			const TNodeBlockOp *block_op = static_cast<TNodeBlockOp *>(node);
			ss << *block_op;
		}
		break;
	case TNodeType::IFBLOCK:
		{
			const TNodeIf *if_block = static_cast<TNodeIf *>(node);
			ss << *if_block;
		}
		break;					
	case TNodeType::WHILEBLOCK:
		{
			const TNodeWhile *if_block = static_cast<TNodeWhile *>(node);
			ss << *if_block;
		}
		break;
	case TNodeType::FORBLOCK:
		{
			const TNodeFor *for_block = static_cast<TNodeFor *>(node);
			ss << *for_block;
		}
		break;
	case TNodeType::FUNCBLOCK:
		{
			const TNodeFunc *func_block = static_cast<TNodeFunc *>(node);
			ss << *func_block;
		}
		break;
	}

	std::stringstream ssout;
	std::string line;
	
	// Add a tab for each node
	while (std::getline(ss, line)) {
		// Add a tab for each node printing
		ssout << '\t' << line << '\n';
	}
	
	
	return ssout;
}

