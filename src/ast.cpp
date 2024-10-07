
#include "ast.hpp"


using namespace pyc;

const std::string pyc::TNodeType::tnode_types_str[TNODETYPE_COUNT] = {
	"term",
	"factor",
	"arithmetic",
};


pyc::AST::AST(void)
{
	root_ = NULL;
}






