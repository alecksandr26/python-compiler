#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>

#include "integer.hpp"
#include "lexer.hpp"
#include "real.hpp"
#include "tag.hpp"
#include "token.hpp"
#include "word.hpp"
#include "lexer_error.hpp" 

using namespace pyc;

pyc::Lexer::Lexer(std::ifstream &source) : source_(source)
{
	if (!source_)
		{
			std::cerr << "Error the file is not opened" << std::endl;
			// TODO: Create exception
			throw LexerError("The file could not be opened");
		}

	// Reserve keywords

	// Structure keywords
	keywords_.insert(std::make_pair("if", Word("if", Token(TokenType::KEYWORD, TagType::IF))));
	keywords_.insert(std::make_pair("else", Word("else", Token(TokenType::KEYWORD, TagType::ELSE))));
	keywords_.insert(std::make_pair("elif", Word("elif", Token(TokenType::KEYWORD, TagType::ELIF))));
	keywords_.insert(std::make_pair("while", Word("while", Token(TokenType::KEYWORD, TagType::WHILE))));
	keywords_.insert(std::make_pair("for", Word("for", Token(TokenType::KEYWORD, TagType::FOR))));
	keywords_.insert(std::make_pair("def", Word("def", Token(TokenType::KEYWORD, TagType::DEF))));

	// Boolean operations keywords
	keywords_.insert(std::make_pair("and", Word("and", Token(TokenType::OPERATOR, TagType::AND))));
	keywords_.insert(std::make_pair("or", Word("or", Token(TokenType::OPERATOR, TagType::OR))));
	keywords_.insert(std::make_pair("not", Word("not", Token(TokenType::OPERATOR, TagType::NOT))));

	line_ = 0;
	peek_ = ' ';
}

void pyc::Lexer::readch(void) { source_.get(peek_); }

bool pyc::Lexer::expectch(char ch)
{
	readch();
	if (peek_ != ch) {
		peek_ = ' ';
		return false;
	}

	peek_ = ' ';
	return true;
}

bool pyc::Lexer::reads_until_finds_something(void)
{
	for (; !source_.eof(); readch()) {
		if (peek_ == ' ' || peek_ == '\t')
			continue;
		else if (peek_ == '\n')
			line_++;
		else
			break;
	}

	return !source_.eof();
}

bool pyc::Lexer::is_token_available(void)
{
	return reads_until_finds_something();
}

const Token &pyc::Lexer::next_token(void)
{
	// Try to finds something otherwise throws an error
	if (!reads_until_finds_something()) {
		throw LexerError("Error there isn't other token");
	}

	// Simple lexemes
	switch (peek_) {
	case '=':
		if (expectch('=')) {
			token_seq_.push_back(&Word::eq);
			return *token_seq_.back();
		}

		token_seq_.push_back(&Token::init);
		return *token_seq_.back();
	case '!':
		if (expectch('=')) {
			token_seq_.push_back(&Word::ne);
			return *token_seq_.back();
		}

		// TODO: Create exception
		throw LexerError("Error Uknown token ");
	case '>':
		if (expectch('=')) {
			token_seq_.push_back(&Word::ge);
			return *token_seq_.back();
		}
		token_seq_.push_back(&Word::gt);
		return *token_seq_.back();
	case '<':
		if (expectch('=')) {
			token_seq_.push_back(&Word::le);
			return *token_seq_.back();
		}
		token_seq_.push_back(&Word::lt);
		return *token_seq_.back();
	case '+':
		peek_ = ' ';
		token_seq_.push_back(&Token::add);
		return *token_seq_.back();
	case '-':
		peek_ = ' ';
		// TODO: Check if there is no problem with integers with sign
		token_seq_.push_back(&Token::sub);
		return *token_seq_.back();
	case '*':
		if (expectch('*')) {
			token_seq_.push_back(&Token::pow);
			return *token_seq_.back();
		}

		token_seq_.push_back(&Token::mul);
		return *token_seq_.back();
	case '/':
		peek_ = ' ';
		token_seq_.push_back(&Token::div);
		return *token_seq_.back();
	case '%':
		peek_ = ' ';
		token_seq_.push_back(&Token::mod);
		return *token_seq_.back();
	case ':':
		peek_ = ' ';
		token_seq_.push_back(&Token::ident);
		return *token_seq_.back();
	case '#': {
		// Skip characters until the end of the line or EOF
		while (peek_ != '\n' && !source_.eof()) {
			readch();
		}

		// Once the comment is skipped, recursively call next_token to process the
		// next valid token
		return next_token();
	}
		// Handle list delimiters (square brackets)
	case '[':
		peek_ = ' ';
		token_seq_.push_back(&Token::lbracket); // lbracket represents `[`
		return *token_seq_.back();
	case ']':
		peek_ = ' ';
		token_seq_.push_back(&Token::rbracket); // rbracket represents `]`
		return *token_seq_.back();

		// Handle tuple delimiters (parentheses)
	case '(':
		peek_ = ' ';
		token_seq_.push_back(&Token::lparen); // lparen represents `(`
		return *token_seq_.back();
	case ')':
		peek_ = ' ';
		token_seq_.push_back(&Token::rparen); // rparen represents `)`
		return *token_seq_.back();

		// Handle comma delimiters
	case ',':
		peek_ = ' ';
		token_seq_.push_back(&Token::comma); // comma represents `,`
		return *token_seq_.back();

	case '"':
	case '\'': {
		char quote_type = peek_; // Store whether it's a single or double quote
		std::string value;

		readch(); // Move to the next character after the opening quote

		// Check if it is a triple-quoted string literal
		if (peek_ == quote_type && expectch(quote_type)) {
			// Detected a multi-line string literal (triple quotes)
			readch(); // Skip the third quote
			while (!source_.eof()) {
				if (peek_ == quote_type && expectch(quote_type) && expectch(quote_type)) {
					// Found closing triple quotes
					peek_ = ' '; // Consume the closing triple quote
					token_seq_.push_back(new Word(value, Token(TokenType::STRING,
										   TagType::STRL)));
					return *token_seq_.back();
				}
				value += peek_;
				readch();
			}

      
			throw LexerError("Error: Unterminated multi-line string literal");
		}

		// Handle single-line string literals
		while (peek_ != quote_type && peek_ != '\n' && !source_.eof()) {
			if (peek_ == '\\') {
				readch(); // Move past the backslash
				switch (peek_) {
				case 'n':
					value += '\n';
					break;
				case 't':
					value += '\t';
					break;
				case '\\':
					value += '\\';
					break;
				case '"':
					value += '"';
					break;
				case '\'':
					value += '\'';
					break;
				default:
					value += peek_;
					break;
				}
			}
			else {
				value += peek_;
			}
			readch();
		}

		// Check for closing quote
		if (peek_ == quote_type) {
			token_seq_.push_back(new Word(value, Token(TokenType::STRING, TagType::STRL)));
			peek_ = ' '; // Consume the closing quote
		}
		else {
			// Handle unterminated single-line string literal
			throw LexerError("Error: Unterminated string literal");
		}

		return *token_seq_.back();
	}
	}

	if (std::isdigit(peek_)) {

		// Gets the integer part
		long ivalue = 0;
		do {
			ivalue = 10 * ivalue + (peek_ - '0');
			readch();
		} while (std::isdigit(peek_));

		if (peek_ != '.') {
			token_seq_.push_back(
					     new Integer(ivalue, Token(TokenType::NUMBER, TagType::INTEGER)));
			return *token_seq_.back();
		}

		double fvalue = static_cast<double>(ivalue), d = 10.0;
		for (;;) {
			readch();
			if (!std::isdigit(peek_))
				break;
			fvalue = fvalue + (peek_ - '0') / d;
			d *= 10.0;
		}
		token_seq_.push_back(
				     new Real(fvalue, Token(TokenType::NUMBER, TagType::REAL)));
		return *token_seq_.back();
	}
	
	// Variable names must start with a letter (upper/lower) or underscore
	if (std::isalpha(peek_) || peek_ == '_')  {
		std::string name("");

		// Append the first valid character (a letter or underscore)
		name.append(1, peek_);
		readch();

		// Continue reading while characters are letters (upper/lower), digits, or underscores
		// Allow letters, digits, and underscores after the first character
		while (std::isalnum(peek_) || peek_ == '_') {
			name.append(1, peek_);
			readch();
		}

		// Check if the name is a keyword; if not, treat it as an identifier
		if (!keywords_.count(name)) {
			// Create a Word object for the identifier
			Word word = Word(name, Token(TokenType::IDENTIFIER, TagType::ID));
			keywords_.insert(std::make_pair(name, word));
		}

		token_seq_.push_back(&keywords_[name]);
		return *token_seq_.back();
	}

	peek_ = ' ';
	token_seq_.push_back(&Token::unknown);
	return *token_seq_.back();
}
