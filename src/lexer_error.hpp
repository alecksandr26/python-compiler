#ifndef LEXER_ERROR_INCLUDED
#define LEXER_ERROR_INCLUDED

#include <stdexcept>
#include <string>

class LexerError : public std::runtime_error {
public:
    // Constructor that takes a message and passes it to std::runtime_error
    explicit LexerError(const std::string& message);
};

#endif // LEXER_ERROR_INCLUDED
