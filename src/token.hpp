#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

#include <ostream>
#include <cstdint>

namespace pyc {
	class TokenType {
	public:
		enum TokenTypeEnum {
			KEYWORD = 0,
			IDENTIFIER,
			NUMBER,
			STRING,
			OPERATOR,
			UNKNOWN
		};
		
		static const std::string token_types_str[];
	};
	
	class Token {
	private:
		uint8_t type_, tag_;
		
	public:
		Token(uint8_t type, uint8_t tag);
		
		uint8_t get_type() const;
		uint8_t get_tag() const;
		const std::string &get_tag_str() const;
		
		friend std::ostream &operator<<(std::ostream &os, const Token &token)
		{
			os << "Token<" << TokenType::token_types_str[token.get_type()]
			   << ", " << token.get_tag_str() << ">";
			return os;
		}
		
	};
}

#endif

