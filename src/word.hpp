// word.hpp
#ifndef WORD_INCLUDED
#define WORD_INCLUDED

#include <string>
#include "token.hpp"

namespace pyc {
    class Word : public Token {
    private:
        std::string lexeme_;

    public:
        Word(const std::string &lexeme, Token token);
        virtual ~Word();

        const std::string &get_lexeme() const;

        std::string to_string() const override;

        // Static words
        static Word eq, ne, ge, le, gt, lt;
        static Word and_, or_, not_;
    };
}

#endif // WORD_INCLUDED
