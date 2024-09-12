#include "word.hpp"

using namespace pyc;

pyc::Word::Word(const std::string &lexeme, const Token &token)
	: Token(token.get_type(), token.get_tag()),
	  lexeme_(lexeme)
{
	
}

pyc::Word::Word(void) : Token()
{
	lexeme_ = "";
}

Word pyc::Word::eq("==", Token(TokenType::OPERATOR, TagType::EQ));
Word pyc::Word::ne("!=", Token(TokenType::OPERATOR, TagType::NEQ));
Word pyc::Word::gt(">", Token(TokenType::OPERATOR, TagType::GT));
Word pyc::Word::ge(">=", Token(TokenType::OPERATOR, TagType::GE));
Word pyc::Word::lt("<", Token(TokenType::OPERATOR, TagType::LT));
Word pyc::Word::le("<=", Token(TokenType::OPERATOR, TagType::LE));



