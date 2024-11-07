#include <string>
#include "error.hpp"

pyc::LogLexerError::LogLexerError(const std::string& message) : std::runtime_error("Lexical Error: " + message)
{
	
}

pyc::LogParserError::LogParserError(const std::string &message, int line)
	: std::runtime_error("Syntax Error: " + message + " in line " + std::to_string(line))
{
	
}

pyc::LogSemanticError::LogSemanticError(const std::string &message)
	: std::runtime_error("Semantic Error: " + message)
{
	
}
