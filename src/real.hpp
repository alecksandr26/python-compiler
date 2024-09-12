#ifndef REAL_INCLUDED
#define REAL_INCLUDED

#include "token.hpp"

namespace pyc {
	class Real : public Token {
	private:
		double value_;
	public:
		Real(const double &value, const Token &token);

		
		friend std::ostream &operator<<(std::ostream &os, const Real &real)
		{
			os << "Real<" << TokenType::token_types_str[real.get_type()]
			   << ", \"" << real.value_ << "\">";
			return os;
		}
	};
}


#endif
