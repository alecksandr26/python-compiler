#ifndef REAL_INCLUDED
#define REAL_INCLUDED

#include "token.hpp"

namespace pyc {
	class Real : public Token {
	private:
		double value_;
	public:
		Real(const double &value, const Token &token);
	};
}


#endif
