#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "integer.hpp"
#include "lexer.hpp"
#include "real.hpp"
#include "tag.hpp"
#include "token.hpp"
#include "word.hpp"
#include "ident.hpp"
#include "error.hpp"

using namespace pyc;

pyc::Lexer::Lexer(std::istream &source) : source_(source)
{
	if (!source_)
		throw LogLexerError(ERROR_IN_STREAM_NOT_OPENED);
	
	// Reserve keywords

	// Structure keywords
	keywords_.insert(std::make_pair("if", Word("if", Token(TokenType::KEYWORD, TagType::IF))));
	keywords_.insert(std::make_pair("else", Word("else", Token(TokenType::KEYWORD, TagType::ELSE))));
	keywords_.insert(std::make_pair("elif", Word("elif", Token(TokenType::KEYWORD, TagType::ELIF))));
	keywords_.insert(std::make_pair("while", Word("while", Token(TokenType::KEYWORD, TagType::WHILE))));
	keywords_.insert(std::make_pair("for", Word("for", Token(TokenType::KEYWORD, TagType::FOR))));
	keywords_.insert(std::make_pair("in", Word("in", Token(TokenType::KEYWORD, TagType::IN))));
	keywords_.insert(std::make_pair("def", Word("def", Token(TokenType::KEYWORD, TagType::DEF))));
	keywords_.insert(std::make_pair("return", Word("return", Token(TokenType::KEYWORD, TagType::RETURN))));
	keywords_.insert(std::make_pair("break", Word("break", Token(TokenType::KEYWORD, TagType::BREAK))));
	keywords_.insert(std::make_pair("continue", Word("continue", Token(TokenType::KEYWORD,
									   TagType::CONTINUE))));
	keywords_.insert(std::make_pair("pass", Word("pass", Token(TokenType::KEYWORD, TagType::PASS))));

	// Boolean operations keywords
	keywords_.insert(std::make_pair("and", Word("and", Token(TokenType::OPERATOR, TagType::AND))));
	keywords_.insert(std::make_pair("or", Word("or", Token(TokenType::OPERATOR, TagType::OR))));
	keywords_.insert(std::make_pair("not", Word("not", Token(TokenType::OPERATOR, TagType::NOT))));
	
	keywords_.insert(std::make_pair("True", Word("True", Token(TokenType::KEYWORD, TagType::TRUE))));
	keywords_.insert(std::make_pair("False", Word("False", Token(TokenType::KEYWORD, TagType::FALSE))));

	line_ = 1;
	peek_ = '\0';
}

void pyc::Lexer::readch(void)
{
	source_.get(peek_);
}

bool pyc::Lexer::expectch(char ch)
{
	readch();
	if (peek_ != ch) {
		peek_ = '\0';
		return false;
	}

	peek_ = '\0';
	return true;
}

bool pyc::Lexer::reads_until_finds_something(void)
{
	for (; !source_.eof(); readch()) {
		if (peek_ == '\0')
			continue;
		// else if (peek_ == '\n')
		// 	line_++;
		else
			break;
	}
	
	return !source_.eof();
}

bool pyc::Lexer::is_token_available(void)
{
	return reads_until_finds_something();
}


int pyc::Lexer::get_line(void)
{
	if (token_seq_.front()->get_tag() == TagType::EOL)
		return line_ - 1 ;
	return line_;
}


// TODO: Divide this function in serveral parts
const Token &pyc::Lexer::next_token(void)
{
	// Try to finds something otherwise throws an error
	if (!reads_until_finds_something()) {
		if (token_seq_.back()->get_tag() == TagType::ENDOFFILE)
			throw LogLexerError(ERROR_NO_NEXT_TOKEN);
		token_seq_.push_back(&Token::end_of_file);
		return *token_seq_.back();
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
		
		throw LogLexerError(ERROR_UNKNOW_TOKEN_MSG);
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
		peek_ = '\0';
		token_seq_.push_back(&Token::add);
		return *token_seq_.back();
	case '-':
		peek_ = '\0';
		
		// TODO: Check if there is no problem with integers with sign, in the syntax analyzer
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
		peek_ = '\0';
		token_seq_.push_back(&Token::div);
		return *token_seq_.back();
	case '%':
		peek_ = '\0';
		token_seq_.push_back(&Token::mod);
		return *token_seq_.back();
	case ':':
		peek_ = '\0';
		token_seq_.push_back(&Token::two_points);
		return *token_seq_.back();
	case '#': {
		// Skip characters until the end of the line or EOF
		while (peek_ != '\n' && !source_.eof())
			readch();
		
		// Once the comment is skipped, recursively call next_token to process the
		// next valid token
		return next_token();
	}
		// Handle list delimiters (square brackets)
	case '[':
		peek_ = '\0';
		token_seq_.push_back(&Token::lbracket); // lbracket represents `[`
		return *token_seq_.back();
	case ']':
		peek_ = '\0';
		token_seq_.push_back(&Token::rbracket); // rbracket represents `]`
		return *token_seq_.back();

		// Handle tuple delimiters (parentheses)
	case '(':
		peek_ = '\0';
		token_seq_.push_back(&Token::lparen); // lparen represents `(`
		return *token_seq_.back();
	case ')':
		peek_ = '\0';
		token_seq_.push_back(&Token::rparen); // rparen represents `)`
		return *token_seq_.back();

		// Handle comma delimiters
	case ',':
		peek_ = '\0';
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
					peek_ = '\0'; // Consume the closing triple quote
					token_seq_.push_back(new Word(value, Token(TokenType::STRING,
										   TagType::STRL)));
					return *token_seq_.back();
				}
				value += peek_;
				readch();
			}

      
			throw LogLexerError(ERROR_UNTERMINATED_STRING_MUL_LIT);
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
			peek_ = '\0'; // Consume the closing quote
		} else {
			// Handle unterminated single-line string literal
			throw LogLexerError(ERROR_UNTERMINATED_STRING_LIT);
		}

		return *token_seq_.back();
	}
	case ' ': {
		// To verify the level of identation
		long level;
		for (level = 1; peek_ == ' ' and !source_.eof(); level++)
			readch();
		
		// Get the actual level of identation
		level /= 4;
		
		if (level == 0)
			return next_token();
		
		if (identation_.count(level) == 0)
			identation_.insert(std::make_pair(level, Ident(level)));
		
		token_seq_.push_back(&identation_[level]);
		return *token_seq_.back();
	}

	case '\n':
		line_++;
		// [[fallthrough]];  // Intentional fallthrough
	// case ';': // Probably for the future but right now none of this shit
		readch();
		token_seq_.push_back(&Token::end_of_line);
		return *token_seq_.back();
	}

	if (std::isdigit(peek_)) {

		// Gets the integer part
		long ivalue = 0;
		do {
			ivalue = 10 * ivalue + (peek_ - '0');
			readch();
			if (source_.eof())
				break;
		} while (std::isdigit(peek_));

		if (peek_ != '.') {
			token_seq_.push_back(new Integer(ivalue, Token(TokenType::NUMBER, TagType::INTEGER)));
			return *token_seq_.back();
		}

		double fvalue = static_cast<double>(ivalue), d = 10.0;
		for (;;) {
			readch();
			if (!std::isdigit(peek_) or source_.eof())
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
	
	// Uknow token
	peek_ = '\0';
	throw LogLexerError(ERROR_UNKNOW_TOKEN_MSG);
}
