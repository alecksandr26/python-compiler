#include <iostream>

#include "tag.hpp"
#include "token.hpp"

using namespace pyc;

pyc::Token::Token(uint8_t type, uint8_t tag)
{
	type_ = type;
	tag_ = tag;
}

pyc::Token::Token(void)
{
	type_ = TokenType::UNKNOWN;
	tag_ = TagType::UNKNOWN;
}

uint8_t pyc::Token::get_type() const
{
	return type_;
}

uint8_t pyc::Token::get_tag() const
{
	return tag_;
}

const std::string &pyc::Token::get_tag_str() const
{
	return TagType::tag_types_str[tag_];
}

const std::string pyc::TokenType::token_types_str[] = {
	"KEYWORD",
	"IDENTIFIER",
	"NUMBER",
	"STRING",
	"OPERATOR",
	"DELIMITER",
	"UNKNOWN"
};

// Initialization of static tokens
Token pyc::Token::init(TokenType::OPERATOR, TagType::INIT);
Token pyc::Token::add(TokenType::OPERATOR, TagType::ADD);
Token pyc::Token::sub(TokenType::OPERATOR, TagType::SUB);
Token pyc::Token::mul(TokenType::OPERATOR, TagType::MUL);
Token pyc::Token::div(TokenType::OPERATOR, TagType::DIV);
Token pyc::Token::mod(TokenType::OPERATOR, TagType::MOD);
Token pyc::Token::pow(TokenType::OPERATOR, TagType::POW);

// Tokens for list and tuple delimiters (added for Python lists and tuples)
Token pyc::Token::lbracket(TokenType::DELIMITER, TagType::LBRACKET);
Token pyc::Token::rbracket(TokenType::DELIMITER, TagType::RBRACKET);
Token pyc::Token::lparen(TokenType::DELIMITER, TagType::LPAREN);
Token pyc::Token::rparen(TokenType::DELIMITER, TagType::RPAREN);
Token pyc::Token::two_points(TokenType::DELIMITER, TagType::TWO_POINTS);
Token pyc::Token::end_of_line(TokenType::DELIMITER, TagType::EOL);
Token pyc::Token::end_of_file(TokenType::DELIMITER, TagType::ENDOFFILE);

// Token for comma
Token pyc::Token::comma(TokenType::DELIMITER, TagType::COMMA);  // Add initialization for the comma token

// Token for identifiers and unknown types
Token pyc::Token::unknown(TokenType::UNKNOWN, TagType::UNKNOWN);

