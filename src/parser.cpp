// parser.cpp
#include "parser.hpp"
#include <stdexcept>

namespace pyc {

Parser::Parser(Lexer &lexer, std::ostream &debug_output)
    : lexer_(lexer), current_token_(nullptr), debug_output_(debug_output) {
  debug_output_ << "Parser initialized." << std::endl;
  advance();
}

void Parser::log(const std::string &message, const std::string &color) {
        debug_output_ << "<span style=\"color:" << color << ";\">"
                      << message << "</span><br>" << std::endl;
    }

void Parser::advance() {
    if (lexer_.is_token_available()) {
        current_token_ = lexer_.next_token();
    } else {
        current_token_ = std::make_unique<Token>(TokenType::END_OF_FILE, TagType::END);
    }
    log("Advanced to token: " + current_token_->to_string(), "blue");
}
void Parser::error(const std::string &msg) {
  std::string error_msg =
      "Syntax Error at line " + std::to_string(lexer_.get_line()) + ": " + msg;
  log(error_msg, "red");
  throw std::runtime_error(error_msg);
}

void Parser::parse() {
  debug_output_ << "<span style=\"color:green;\">Starting parse...</span><br>"
                << std::endl;
  program();
  debug_output_ << "<span style=\"color:green;\">Parsing completed.</span><br>"
                << std::endl;
}

void Parser::program() {
  while (current_token_->get_type() != TokenType::END_OF_FILE) {
    statement();
  }
}

void Parser::statement() {
  debug_output_ << "<span style=\"color:blue;\">Parsing statement at line "
                << lexer_.get_line() << "</span><br>" << std::endl;
  if (current_token_->get_type() == TokenType::KEYWORD) {
    if (current_token_->get_tag() == TagType::IF) {
      parse_if_statement();
    } else if (current_token_->get_tag() == TagType::ELSE) {
      parse_else_statement();
    } else if (current_token_->get_tag() == TagType::PRINT) {
      debug_output_ << "Parsing print statement" << std::endl;
      advance(); // Consume 'print'
      parse_function_call(current_token_.get());
    } else {
      error("Unexpected keyword");
    }
  } else if (current_token_->get_type() == TokenType::IDENTIFIER) {
    auto id_token = current_token_.get();
    debug_output_ << "Found identifier: " << id_token->to_string() << std::endl;
    advance();
    if (current_token_->get_tag() == TagType::INIT) { // '=' operator
      debug_output_ << "Found assignment operator '=' after identifier"
                    << std::endl;
      advance();
      expression();
    } else {
      error("Expected '=' after identifier");
    }
  } else {
    error("Unexpected token");
  }
}

void Parser::parse_if_statement() {
  debug_output_ << "Parsing 'if' statement at line " << lexer_.get_line()
                << std::endl;
  advance();    // Consume 'if'
  expression(); // Parse condition
  if (current_token_->get_tag() != TagType::COLON) {
    error("Expected ':' after 'if' condition");
  }
  debug_output_ << "Found ':' after 'if' condition" << std::endl;
  advance(); // Consume ':'
  if (current_token_->get_tag() != TagType::INDENT) {
    error("Expected indented block after ':'");
  }
  debug_output_ << "Entering 'if' block" << std::endl;
  advance(); // Consume 'INDENT'
  while (current_token_->get_tag() != TagType::DEDENT &&
         current_token_->get_type() != TokenType::END_OF_FILE) {
    statement();
  }
  if (current_token_->get_tag() == TagType::DEDENT) {
    debug_output_ << "Exiting 'if' block" << std::endl;
    advance(); // Consume 'DEDENT'
  } else {
    error("Expected 'DEDENT' after 'if' block");
  }
}

void Parser::parse_else_statement() {
  debug_output_ << "Parsing 'else' statement at line " << lexer_.get_line()
                << std::endl;
  advance(); // Consume 'else'
  if (current_token_->get_tag() != TagType::COLON) {
    error("Expected ':' after 'else'");
  }
  debug_output_ << "Found ':' after 'else'" << std::endl;
  advance(); // Consume ':'
  if (current_token_->get_tag() != TagType::INDENT) {
    error("Expected indented block after ':'");
  }
  debug_output_ << "Entering 'else' block" << std::endl;
  advance(); // Consume 'INDENT'
  while (current_token_->get_tag() != TagType::DEDENT &&
         current_token_->get_type() != TokenType::END_OF_FILE) {
    statement();
  }
  if (current_token_->get_tag() == TagType::DEDENT) {
    debug_output_ << "Exiting 'else' block" << std::endl;
    advance(); // Consume 'DEDENT'
  } else {
    error("Expected 'DEDENT' after 'else' block");
  }
}

void Parser::expression() {
  log("Parsing expression starting with token: " + current_token_->to_string());
  arithmetic_expression();
  if (current_token_->get_tag() == TagType::EQ ||
      current_token_->get_tag() == TagType::NE ||
      current_token_->get_tag() == TagType::GT ||
      current_token_->get_tag() == TagType::GE ||
      current_token_->get_tag() == TagType::LT ||
      current_token_->get_tag() == TagType::LE) {
    auto op = current_token_->get_tag();
    log("Found comparison operator: " + current_token_->to_string(), "purple");
    advance();
    arithmetic_expression();
    // Build AST node for comparison
  }
}

void Parser::arithmetic_expression() {
  debug_output_ << "Parsing arithmetic expression starting with token: "
                << current_token_->to_string() << std::endl;
  term();
  while (current_token_->get_tag() == TagType::ADD ||
         current_token_->get_tag() == TagType::SUB) {
    auto op = current_token_->get_tag();
    debug_output_ << "Found additive operator: " << current_token_->to_string()
                  << std::endl;
    advance();
    term();
    // Build AST node for the operation
  }
}

void Parser::term() {
  debug_output_ << "Parsing term starting with token: "
                << current_token_->to_string() << std::endl;
  factor();
  while (current_token_->get_tag() == TagType::MUL ||
         current_token_->get_tag() == TagType::DIV) {
    auto op = current_token_->get_tag();
    debug_output_ << "Found multiplicative operator: "
                  << current_token_->to_string() << std::endl;
    advance();
    factor();
    // Build AST node for the operation
  }
}

void Parser::factor() {
  debug_output_ << "Parsing factor starting with token: "
                << current_token_->to_string() << std::endl;
  if (current_token_->get_type() == TokenType::NUMBER ||
      current_token_->get_type() == TokenType::STRING) {
    debug_output_ << "Found literal: " << current_token_->to_string()
                  << std::endl;
    advance();
  } else if (current_token_->get_type() == TokenType::IDENTIFIER) {
    auto id_token = current_token_.get();
    debug_output_ << "Found identifier: " << id_token->to_string() << std::endl;
    advance();
    if (current_token_->get_tag() == TagType::LPAREN) {
      parse_function_call(id_token);
    } else {
      // Variable usage
    }
  } else if (current_token_->get_tag() == TagType::LPAREN) {
    debug_output_ << "Found '('" << std::endl;
    advance();
    expression();
    if (current_token_->get_tag() != TagType::RPAREN) {
      error("Expected ')'");
    }
    debug_output_ << "Found ')'" << std::endl;
    advance();
  } else if (current_token_->get_tag() == TagType::LBRACKET) {
    parse_list_literal();
  } else {
    error("Expected number, string, identifier, or '('");
  }
}

void Parser::parse_function_call(Token *function_name) {
  debug_output_ << "Parsing function call: " << function_name->to_string()
                << std::endl;
  advance(); // Consume '('
  if (current_token_->get_tag() != TagType::RPAREN) {
    // Parse arguments
    parse_argument_list();
  }
  if (current_token_->get_tag() != TagType::RPAREN) {
    error("Expected ')' at the end of function call");
  }
  debug_output_ << "Found ')' after function call" << std::endl;
  advance(); // Consume ')'
}

void Parser::parse_argument_list() {
  debug_output_ << "Parsing argument list starting with token: "
                << current_token_->to_string() << std::endl;
  expression();
  while (current_token_->get_tag() == TagType::COMMA) {
    advance(); // Consume ','
    expression();
  }
}

void Parser::parse_list_literal() {
  debug_output_ << "Parsing list literal starting at line " << lexer_.get_line()
                << std::endl;
  advance(); // Consume '['
  if (current_token_->get_tag() != TagType::RBRACKET) {
    expression();
    while (current_token_->get_tag() == TagType::COMMA) {
      advance(); // Consume ','
      expression();
    }
  }
  if (current_token_->get_tag() != TagType::RBRACKET) {
    error("Expected ']' at the end of list literal");
  }
  debug_output_ << "Found ']' closing list literal" << std::endl;
  advance(); // Consume ']'
}

} // namespace pyc
