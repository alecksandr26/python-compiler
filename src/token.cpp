// token.cpp
#include "token.hpp"
#include "tag.hpp"

namespace pyc {

    Token::Token(TokenType::TokenTypeEnum type, TagType::TagTypeEnum tag)
        : type_(type), tag_(tag) {}

    Token::Token()
        : type_(TokenType::UNKNOWN), tag_(TagType::UNKNOWN) {}

    Token::~Token() {}

    TokenType::TokenTypeEnum Token::get_type() const {
        return type_;
    }

    TagType::TagTypeEnum Token::get_tag() const {
        return tag_;
    }

    const std::string &Token::get_tag_str() const {
        return TagType::tag_types_str[tag_];
    }

    std::string Token::to_string() const {
        return "Token<" + TokenType::token_types_str[get_type()] + ", " + get_tag_str() + ">";
    }

    std::ostream &operator<<(std::ostream &os, const Token &token) {
        os << token.to_string();
        return os;
    }

    const std::string TokenType::token_types_str[] = {
        "KEYWORD",
        "IDENTIFIER",
        "NUMBER",
        "STRING",
        "OPERATOR",
        "DELIMITER",
        "UNKNOWN",
        "END_OF_FILE"
    };

    // Initialization of static tokens
    Token Token::init(TokenType::OPERATOR, TagType::INIT);
    Token Token::add(TokenType::OPERATOR, TagType::ADD);
    Token Token::sub(TokenType::OPERATOR, TagType::SUB);
    Token Token::mul(TokenType::OPERATOR, TagType::MUL);
    Token Token::div(TokenType::OPERATOR, TagType::DIV);
    Token Token::mod(TokenType::OPERATOR, TagType::MOD);
    Token Token::pow(TokenType::OPERATOR, TagType::POW);

    Token Token::eq(TokenType::OPERATOR, TagType::EQ);
    Token Token::ne(TokenType::OPERATOR, TagType::NE);
    Token Token::ge(TokenType::OPERATOR, TagType::GE);
    Token Token::le(TokenType::OPERATOR, TagType::LE);
    Token Token::gt(TokenType::OPERATOR, TagType::GT);
    Token Token::lt(TokenType::OPERATOR, TagType::LT);

    Token Token::lbracket(TokenType::DELIMITER, TagType::LBRACKET);
    Token Token::rbracket(TokenType::DELIMITER, TagType::RBRACKET);
    Token Token::lparen(TokenType::DELIMITER, TagType::LPAREN);
    Token Token::rparen(TokenType::DELIMITER, TagType::RPAREN);
    Token Token::comma(TokenType::DELIMITER, TagType::COMMA);
    Token Token::colon(TokenType::DELIMITER, TagType::COLON);

    Token Token::indent(TokenType::DELIMITER, TagType::INDENT);
    Token Token::dedent(TokenType::DELIMITER, TagType::DEDENT);

    Token Token::unknown(TokenType::UNKNOWN, TagType::UNKNOWN);
    Token Token::end_of_file(TokenType::END_OF_FILE, TagType::END);

}
