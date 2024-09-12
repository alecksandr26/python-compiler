
#include "real.hpp"


pyc::Real::Real(const double &value, const Token &token)
	: Token(token.get_type(), token.get_type()),
	  value_(value)
{
	
}
	  

