#ifndef INTEGER_INCLUDED
#define INTEGER_INCLUDED

#include <ostream>
#include <cstdint>

#include "token.hpp"

namespace pyc {
	class Integer : public Token {
	private:
		long value_;
		
	public:
		Integer(const long &value, const Token &token);
		long get_value() const; 
		
		friend std::ostream &operator<<(std::ostream &os, const Integer &integer)
		{
			os << "Integer<" << TokenType::token_types_str[integer.get_type()]
			   << ", \"" << integer.value_ << "\">";
			return os;
		}
	};
}



#endif

