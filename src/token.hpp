// token.hpp
#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

#include <ostream>
#include <string>
#include "tag.hpp"

namespace pyc {
    class TokenType {
    public:
        enum TokenTypeEnum {
            KEYWORD = 0,
            IDENTIFIER,
            NUMBER,
            STRING,
            OPERATOR,
            DELIMITER,
            UNKNOWN,
            END_OF_FILE
        };

        static const std::string token_types_str[];
    };

    class Token {
    private:
        TokenType::TokenTypeEnum type_;
        TagType::TagTypeEnum tag_;

    public:
        Token(TokenType::TokenTypeEnum type, TagType::TagTypeEnum tag);
        Token();
        virtual ~Token(); // Make destructor virtual

        TokenType::TokenTypeEnum get_type() const;
        TagType::TagTypeEnum get_tag() const;
        const std::string &get_tag_str() const;

        virtual std::string to_string() const;

        friend std::ostream &operator<<(std::ostream &os, const Token &token);

        // Static tokens
        static Token init, add, sub, mul, div, mod, pow;
        static Token eq, ne, ge, le, gt, lt;
        static Token lbracket, rbracket, lparen, rparen, comma, colon;
        static Token indent, dedent;
        static Token unknown, end_of_file;
    };
}

#endif // TOKEN_INCLUDED
