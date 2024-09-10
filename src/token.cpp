#include <iostream>

#include "tag.hpp"
#include "token.hpp"

using namespace pyc;

pyc::Token::Token(uint8_t type, uint8_t tag)
{
	type_ = type;
	tag_ = tag;
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
	return Tag::tag_types_str[tag_];
}

const std::string pyc::TokenType::token_types_str[] = {
	"KEYWORD",
	"IDENTIFIER",
	"NUMBER",
	"STRING",
	"OPERATOR",
	"UNKNOWN"
};


