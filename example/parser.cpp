#include <iostream>
#include <vector>
#include <string>
#include <memory>


// Enum for different token types
enum class TokenType {
	Number,
	Identifier,
	Operator,
	ParenOpen,
	ParenClose,
	End
};

// Token structure
struct Token {
	TokenType type;
	std::string value;
};


enum class NodeType {
	Number,
	Variable,
	Operator,
	FunctionCall,
	Boolean
};


// Tokenizer function
std::vector<Token> tokenize(const std::string& expression) {
	std::vector<Token> tokens;
	size_t i = 0;

	while (i < expression.length()) {
		char ch = expression[i];

		if (std::isspace(ch)) {
			i++;
			continue;
		}

		// Number token
		if (std::isdigit(ch)) {
			std::string number;
			while (i < expression.length() && std::isdigit(expression[i])) {
				number += expression[i++];
			}
			tokens.push_back({TokenType::Number, number});
		}
		// Identifier or variable token
		else if (std::isalpha(ch)) {
			std::string id;
			while (i < expression.length() && std::isalnum(expression[i])) {
				id += expression[i++];
			}
			tokens.push_back({TokenType::Identifier, id});
		}
		// Operator token
		else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')' || ch == '%') {
			// Check for multi-character operators like '**'
			if (ch == '*' && i + 1 < expression.length() && expression[i + 1] == '*') {
				tokens.push_back({TokenType::Operator, "**"});
				i += 2;
			} else if (ch == '(') {
				tokens.push_back({TokenType::ParenOpen, "("});
				i++;
			} else if (ch == ')') {
				tokens.push_back({TokenType::ParenClose, ")"});
				i++;
			} else {
				tokens.push_back({TokenType::Operator, std::string(1, ch)});
				i++;
			}
		}
	}

	tokens.push_back({TokenType::End, ""});
	return tokens;
}



// Base AST node class
class ASTNode {
public:
	virtual ~ASTNode() {}
	virtual void print() const = 0;
	virtual NodeType getType() const = 0;
};

// Number node (e.g. 1, 2.5)
class NumberNode : public ASTNode {
	double value;
public:
	NumberNode(double val) : value(val) {}
	void print() const override { std::cout << value; }
	NodeType getType() const override { return NodeType::Number; }
};

// Variable node (e.g. var, x)
class VariableNode : public ASTNode {
	std::string name;
public:
	VariableNode(const std::string& n) : name(n) {}
	void print() const override { std::cout << name; }
	NodeType getType() const override { return NodeType::Variable; }
};

// Operator node (e.g. +, *, and)
class OperatorNode : public ASTNode {
	char op;
	std::shared_ptr<ASTNode> left, right;
public:
	OperatorNode(char oper, std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
		: op(oper), left(lhs), right(rhs) {}

	void print() const override {
		std::cout << "(";
		left->print();
		std::cout << " " << op << " ";
		right->print();
		std::cout << ")";
	}

	NodeType getType() const override { return NodeType::Operator; }
};

// Boolean node (e.g. and, or)
class BooleanNode : public ASTNode {
	std::string op;
	std::shared_ptr<ASTNode> left, right;
public:
	BooleanNode(const std::string& oper, std::shared_ptr<ASTNode> lhs, std::shared_ptr<ASTNode> rhs)
		: op(oper), left(lhs), right(rhs) {}

	void print() const override {
		std::cout << "(";
		left->print();
		std::cout << " " << op << " ";
		right->print();
		std::cout << ")";
	}

	NodeType getType() const override { return NodeType::Boolean; }
};


// Enums and classes from previous example

std::shared_ptr<ASTNode> parseExpression(const std::vector<Token>& tokens, size_t& pos);


std::shared_ptr<ASTNode> parseFactor(const std::vector<Token>& tokens, size_t& pos) {
	Token token = tokens[pos];

	if (token.type == TokenType::Number) {
		pos++;
		return std::make_shared<NumberNode>(std::stod(token.value));
	} else if (token.type == TokenType::Identifier) {
		pos++;
		return std::make_shared<VariableNode>(token.value);
	} else if (token.type == TokenType::ParenOpen) {
		pos++;
		auto expr = parseExpression(tokens, pos);
		pos++;  // Skip closing paren ')'
		return expr;
	}

	return nullptr;
}

std::shared_ptr<ASTNode> parseExponent(const std::vector<Token>& tokens, size_t& pos) {
	auto node = parseFactor(tokens, pos);

	while (tokens[pos].type == TokenType::Operator && tokens[pos].value == "**") {
		char op = tokens[pos].value[0];
		pos++;
		node = std::make_shared<OperatorNode>('^', node, parseFactor(tokens, pos));  // Use '^' for exponentiation
	}

	return node;
}

std::shared_ptr<ASTNode> parseTerm(const std::vector<Token>& tokens, size_t& pos) {
	auto node = parseExponent(tokens, pos);

	while (tokens[pos].type == TokenType::Operator &&
	       (tokens[pos].value == "*" || tokens[pos].value == "/" || tokens[pos].value == "%")) {
		char op = tokens[pos].value[0];
		pos++;
		node = std::make_shared<OperatorNode>(op, node, parseExponent(tokens, pos));
	}

	return node;
}

std::shared_ptr<ASTNode> parseExpression(const std::vector<Token>& tokens, size_t& pos) {
	auto node = parseTerm(tokens, pos);

	while (tokens[pos].type == TokenType::Operator && (tokens[pos].value == "+" || tokens[pos].value == "-")) {
		char op = tokens[pos].value[0];
		pos++;
		node = std::make_shared<OperatorNode>(op, node, parseTerm(tokens, pos));
	}

	return node;
}

// Tokenization code is the same as before

int main() {
	std::string expression = "3 * 2 ** 4 / 2 % 5";
	auto tokens = tokenize(expression);
	size_t pos = 0;
	auto ast = parseExpression(tokens, pos);

	std::cout << "AST: ";
	ast->print();
	std::cout << std::endl;

	return 0;
}
