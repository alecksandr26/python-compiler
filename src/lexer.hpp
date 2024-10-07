// lexer.hpp
#ifndef LEXER_INCLUDED
#define LEXER_INCLUDED

#include <istream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include "token.hpp"
#include "word.hpp"

namespace pyc {
    class Lexer {
    private:
        std::istream &source_;
        std::map<std::string, Word> keywords_;
        char peek_;
        int line_;

        // For indentation handling
        std::vector<int> indent_levels_;
        bool at_line_start_;
        int current_indent_;

        // Token queue for pending tokens
        std::deque<std::unique_ptr<Token>> token_queue_;

        void readch();
        bool expectch(char ch);
        bool reads_until_finds_something();

        void handle_indentation();

    public:
        Lexer(std::istream &source);
        ~Lexer();

        bool is_token_available();
        std::unique_ptr<Token> next_token();
        int get_line() const;
    };
}

#endif // LEXER_INCLUDED
