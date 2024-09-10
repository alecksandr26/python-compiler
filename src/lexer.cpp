#include <fstream>
#include <map>
#include <cassert>


#include "token.hpp"
#include "lexer.hpp"

using namespace pyc;

pyc::Lexer::Lexer(std::ifstream &source) : source_(source)
{
	if (!source_) {
		std::cerr << "Error the file is not opened" << std::endl;
		// TODO: Create exceptions here
		assert(0);
        }

	// Reserve keywords

	// Structure keywords
	keywors_.insert(std::make_pair("if", Token(TokenType::KEYWORD, "if")));
	keywors_.insert(std::make_pair("else", Token(TokenType::KEYWORD, "else")));
	keywors_.insert(std::make_pair("elif", Token(TokenType::KEYWORD, "elif")));
	keywors_.insert(std::make_pair("while", Token(TokenType::KEYWORD, "while")));
	keywors_.insert(std::make_pair("for", Token(TokenType::KEYWORD, "for")));
	keywors_.insert(std::make_pair("def", Token(TokenType::KEYWORD, "def")));

	// Boolean operations keywords
	keywors_.insert(std::make_pair("and", Token(TokenType::OPERATOR, "and")));
	keywors_.insert(std::make_pair("or", Token(TokenType::OPERATOR, "and")));
	keywors_.insert(std::make_pair("not", Token(TokenType::OPERATOR, "not")));

	line_ = 0;
}


void pyc::Lexer::readch(void)
{
	source_.get(peek_);
}

bool pyc::Lexer::expectch(char ch)
{
	readch();
	if (peek_ != ch)
		return false;
	return true;
}

Token pyc::Lexer::next_token(void)
{
	// Continues reading until it finds something
	for (;; readch()) {
		if (peek_ == ' ' || peek_ == '\t')
			continue;
		else if (peek_ == '\n')
			line_++;
		else
			break;
	}
	
	switch (peek_) {
	case '&':
		break;
	};
}










