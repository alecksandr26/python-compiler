#include "ident.hpp"
#include "tag.hpp"

using namespace pyc;

pyc::Ident::Ident(const long &ident_level)
	: Token(TokenType::DELIMITER, TagType::IDENT), ident_level_(ident_level)
{
	
}

pyc::Ident::Ident(void)
{
	ident_level_ = 0;
}




