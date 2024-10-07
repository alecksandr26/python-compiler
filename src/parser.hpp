// parser.hpp
#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <memory>
#include <ostream>

namespace pyc {

    class Parser {
    public:
        Parser(Lexer &lexer, std::ostream &debug_output);

        void parse();

    private:
        Lexer &lexer_;
        std::unique_ptr<Token> current_token_;
        std::ostream &debug_output_;

        void advance();
		// Corrected declaration of log function
        void log(const std::string &message, const std::string &color = "black");
        void error(const std::string &msg);
        void program();
        void statement();
        void parse_if_statement();
        void parse_else_statement();
        void expression();
        void arithmetic_expression();
        void term();
        void factor();
        void parse_function_call(Token* function_name);
        void parse_argument_list();
        void parse_list_literal();
    };

}

#endif // PARSER_HPP
