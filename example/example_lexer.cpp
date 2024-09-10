#include <iostream>
#include <string>
#include <vector>
#include <cctype>
 
 
// Define the possible token types
enum class TokenType {
	KEYWORD, IDENTIFIER, NUMBER, OPERATOR, UNKNOWN
};
 
// Class representing a token
class Token {
public:
	TokenType type;
	std::string value;
 
	Token(TokenType t, std::string v) : type(t), value(v) {}
 
	void print() const {
		std::cout << "Token(" << value << ", ";
		switch (type) {
		case TokenType::KEYWORD:
			std::cout << "KEYWORD";
			break;
		case TokenType::IDENTIFIER:
			std::cout << "IDENTIFIER";
			break;
		case TokenType::NUMBER:
			std::cout << "NUMBER";
			break;
		case TokenType::OPERATOR:
			std::cout << "OPERATOR";
			break;
		default:
			std::cout << "UNKNOWN";
			break;
		}
		std::cout << ")" << std::endl;
	}
};
 
 
class Lexer {
private:
	std::string input;
	size_t pos;
	std::vector<std::string> keywords = {"if", "else", "while", "for"};
 
	// Helper to check if a string is a keyword
	bool isKeyword(const std::string &str) {
		for (const auto &keyword : keywords) {
			if (str == keyword) return true;
		}
		return false;
	}
 
public:
	Lexer(const std::string &inputCode) : input(inputCode), pos(0) {}
 
	// Method to get the next token
	Token getNextToken() {
		while (pos < input.length() && isspace(input[pos])) pos++;
 
		if (pos >= input.length()) {
			return Token(TokenType::UNKNOWN, ""); // End of input
		}
 
		char currentChar = input[pos];
 
		// Recognize a number
		if (isdigit(currentChar)) {
			std::string number;
			while (pos < input.length() && isdigit(input[pos])) {
				number += input[pos++];
			}
			return Token(TokenType::NUMBER, number);
		}
 
		// Recognize an identifier or keyword
		if (isalpha(currentChar)) {
			std::string identifier;
			while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
				identifier += input[pos++];
			}
			if (isKeyword(identifier)) {
				return Token(TokenType::KEYWORD, identifier);
			}
			return Token(TokenType::IDENTIFIER, identifier);
		}
 
		// Recognize operators (we'll just handle +, -, *, / for simplicity)
		if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/') {
			pos++;
			return Token(TokenType::OPERATOR, std::string(1, currentChar));
		}
 
		// If nothing matched, return unknown token
		pos++;
		return Token(TokenType::UNKNOWN, std::string(1, currentChar));
	}
 
	// Method to scan the whole input and return a list of tokens
	std::vector<Token> tokenize() {
		std::vector<Token> tokens;
		Token token = getNextToken();
		while (token.type != TokenType::UNKNOWN || !token.value.empty()) {
			tokens.push_back(token);
			token = getNextToken();
		}
		return tokens;
	}
};
 
 
int main() {
	std::string code = "if x + 123";
	
	Lexer lexer(code);
	
	std::vector<Token> tokens = lexer.tokenize();
	
	for (const auto &token : tokens) {
		token.print();
	}
 
	return 0;
}


