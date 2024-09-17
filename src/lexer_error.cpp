#include "lexer_error.hpp"

// Implementation of the LexerError constructor
LexerError::LexerError(const std::string& message)
    : std::runtime_error("Lexer Error: " + message) {}
