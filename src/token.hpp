#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

#include <ostream>
#include <cstdint>

#include "tag.hpp"

namespace pyc {
	class TokenType {
	public:
		enum TokenTypeEnum {
			KEYWORD = 0,   // Keywords like 'if', 'else', etc.
			IDENTIFIER,    // Variable names, function names, etc.
			NUMBER,        // Numeric literals like 123, 3.14
			STRING,        // String literals like "hello"
			OPERATOR,      // Operators like +, -, *, /
			DELIMITER,     // [], (),  or code blocks ,
			UNKNOWN        // For any token that doesn't match known types
		};
		
		static const std::string token_types_str[];
	};
	
	class Token {
	private:
		uint8_t type_, tag_;
		
	public:
		Token(uint8_t type, uint8_t tag);
		Token(void);
		virtual ~Token(void) = default;
		
		uint8_t get_type() const;
		uint8_t get_tag() const;
		const std::string &get_tag_str() const;

		// Operator to print token details
		friend std::ostream &operator<<(std::ostream &os, const Token &token)
		{
			os << "Token<" << TokenType::token_types_str[token.get_type()]
			   << ", " << token.get_tag_str() << ">";
			return os;
		}
		
		// Static tokens to represent common operators, delimiters, and unknown token
		static Token init, add, sub, mul, div, mod, pow, two_points, end_of_line, end_of_file;
		static Token lbracket, rbracket, lparen, rparen, comma, unknown;  // Add comma token
	};
}

#endif
