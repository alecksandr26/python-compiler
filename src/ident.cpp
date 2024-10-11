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

void pyc::Ident::set_ident_level(long ident_level)
{
	ident_level_ = ident_level;
}

long pyc::Ident::get_ident_level(void) const
{
	return ident_level_;
}


