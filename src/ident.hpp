#ifndef IDENT_INCUDED
#define IDENT_INCUDED

#include "token.hpp"

namespace pyc {
	class Ident : public Token {
	private:
		long ident_level_;
		
	public:
		Ident(const long &ident_level);
		Ident(void);

		friend std::ostream &operator<<(std::ostream &os, const Ident &ident)
		{
			os << "Ident<" << TokenType::token_types_str[ident.get_type()]
			   << ", \"" << ident.ident_level_ << "\">";
			return os;
		}
	};
}


#endif



