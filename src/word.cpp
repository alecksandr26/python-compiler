// word.cpp
#include "word.hpp"

namespace pyc {

    Word::Word(const std::string &lexeme, Token token)
        : Token(token), lexeme_(lexeme) {}

    Word::~Word() {}

    const std::string &Word::get_lexeme() const {
        return lexeme_;
    }

    std::string Word::to_string() const {
        return "Word<" + lexeme_ + ", " + get_tag_str() + ">";
    }

    // Initialization of static words
    Word Word::eq("==", Token(TokenType::OPERATOR, TagType::EQ));
    Word Word::ne("!=", Token(TokenType::OPERATOR, TagType::NE));
    Word Word::ge(">=", Token(TokenType::OPERATOR, TagType::GE));
    Word Word::le("<=", Token(TokenType::OPERATOR, TagType::LE));
    Word Word::gt(">", Token(TokenType::OPERATOR, TagType::GT));
    Word Word::lt("<", Token(TokenType::OPERATOR, TagType::LT));

    Word Word::and_("and", Token(TokenType::OPERATOR, TagType::AND));
    Word Word::or_("or", Token(TokenType::OPERATOR, TagType::OR));
    Word Word::not_("not", Token(TokenType::OPERATOR, TagType::NOT));

}
