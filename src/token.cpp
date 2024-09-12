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
	"UNKNOWN"
};

Token pyc::Token::init(TokenType::OPERATOR, TagType::INIT);
Token pyc::Token::add(TokenType::OPERATOR, TagType::ADD);
Token pyc::Token::sub(TokenType::OPERATOR, TagType::SUB);
Token pyc::Token::mul(TokenType::OPERATOR, TagType::MUL);
Token pyc::Token::div(TokenType::OPERATOR, TagType::DIV);
Token pyc::Token::mod(TokenType::OPERATOR, TagType::MOD);
Token pyc::Token::pow(TokenType::OPERATOR, TagType::POW);

// TODO: Probably we would need another category for this one
Token pyc::Token::ident(TokenType::OPERATOR, TagType::IDENT);
Token pyc::Token::unknown(TokenType::UNKNOWN, TagType::UNKNOWN);



