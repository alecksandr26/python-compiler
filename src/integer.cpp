#include "integer.hpp"

pyc::Integer::Integer(const long &value, const Token &token)
	: Token(token.get_type(), token.get_tag()), value_(value)
{
	
}


long pyc::Integer::get_value() const {
    return value_; // Return the stored integer value
}



