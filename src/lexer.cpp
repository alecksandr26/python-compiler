// lexer.cpp
#include "lexer.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "word.hpp"
#include <cctype>
#include <stdexcept>
#include <iostream> 
#include <fstream>

namespace pyc {

	std::ofstream debug_log("debug_log.txt", std::ios::app); 

class LexerError : public std::runtime_error {
public:
  explicit LexerError(const std::string &message)
      : std::runtime_error("[LEXER ERROR]: " + message) {}
};

Lexer::Lexer(std::istream &source)
    : source_(source), peek_(' '), line_(1), at_line_start_(true),
      current_indent_(0) {

  if (!source_) {
    throw LexerError("Input stream is not opened");
  }

  // Reserve keywords
  keywords_.insert({"if", Word("if", Token(TokenType::KEYWORD, TagType::IF))});
  keywords_.insert(
      {"else", Word("else", Token(TokenType::KEYWORD, TagType::ELSE))});
  keywords_.insert(
      {"elif", Word("elif", Token(TokenType::KEYWORD, TagType::ELIF))});
  keywords_.insert(
      {"while", Word("while", Token(TokenType::KEYWORD, TagType::WHILE))});
  keywords_.insert(
      {"for", Word("for", Token(TokenType::KEYWORD, TagType::FOR))});
  keywords_.insert(
      {"def", Word("def", Token(TokenType::KEYWORD, TagType::DEF))});
  keywords_.insert(
      {"return", Word("return", Token(TokenType::KEYWORD, TagType::RETURN))});
  keywords_.insert(
      {"break", Word("break", Token(TokenType::KEYWORD, TagType::BREAK))});
  keywords_.insert({"continue", Word("continue", Token(TokenType::KEYWORD,
                                                       TagType::CONTINUE))});
  keywords_.insert(
      {"print", Word("print", Token(TokenType::KEYWORD, TagType::PRINT))});

  keywords_.insert(
      {"and", Word("and", Token(TokenType::OPERATOR, TagType::AND))});
  keywords_.insert({"or", Word("or", Token(TokenType::OPERATOR, TagType::OR))});
  keywords_.insert(
      {"not", Word("not", Token(TokenType::OPERATOR, TagType::NOT))});

  indent_levels_.push_back(0); // Initialize with zero indentation
}

Lexer::~Lexer() {
  // No need to manually delete tokens since we're using smart pointers
}

void Lexer::readch() {
    peek_ = source_.get();
	 debug_log << "Read character: '" << peek_ << "' at line " << line_ << std::endl;
    if (peek_ == '\n') {
        line_++;
        at_line_start_ = true;
    }
    // Do not set 'at_line_start_' to false here
}

bool Lexer::expectch(char ch) {
  readch();
  if (peek_ != ch) {
    return false;
  }
  peek_ = ' ';
  return true;
}
bool Lexer::reads_until_finds_something() {
    while (!source_.eof()) {
        if (peek_ == ' ' || peek_ == '\t' || peek_ == '\r') {
            if (at_line_start_) {
                if (line_ == 1) {
                    // Ignore indentation on the first line
                    current_indent_ = 0;
                } else {
                    if (peek_ == ' ')
                        current_indent_ += 1;
                    else if (peek_ == '\t')
                        current_indent_ += 4; // Assuming a tab is 4 spaces
                }
            }
            readch();
        } else if (peek_ == '\n') {
            readch();
            at_line_start_ = true;
            current_indent_ = 0; // Reset indentation level for the new line
        } else if (peek_ == '#') {
            // Skip comment until end of line
            while (peek_ != '\n' && !source_.eof()) {
                readch();
            }
        } else {
            // Non-whitespace character found
            break;
        }
    }
    return !source_.eof();
}


void Lexer::handle_indentation() {
    if (at_line_start_) {
        int last_indent = indent_levels_.back();
        std::cout << "Handling indentation at line " << line_ << ": current_indent_ = " << current_indent_ << ", last_indent = " << last_indent << std::endl;
        if (current_indent_ > last_indent) {
            indent_levels_.push_back(current_indent_);
            token_queue_.push_back(std::make_unique<Token>(TokenType::DELIMITER, TagType::INDENT));
            std::cout << "Indentation increased. INDENT token added." << std::endl;
        } else if (current_indent_ < last_indent) {
            while (current_indent_ < last_indent) {
                indent_levels_.pop_back();
                token_queue_.push_back(std::make_unique<Token>(TokenType::DELIMITER, TagType::DEDENT));
                std::cout << "Indentation decreased. DEDENT token added." << std::endl;
                last_indent = indent_levels_.back();
            }
        }
        at_line_start_ = false;
    }
}


bool Lexer::is_token_available() {
  return !token_queue_.empty() || reads_until_finds_something();
}

int Lexer::get_line() const { return line_; }

std::unique_ptr<Token> Lexer::next_token() {
   // If there are tokens in the queue, return the next one
    if (!token_queue_.empty()) {
        auto token = std::move(token_queue_.front());
        token_queue_.pop_front();
        return token;
    }

    // Read until we find something, updating current_indent_
    if (!reads_until_finds_something()) {
        // Handle any remaining dedents
        while (indent_levels_.size() > 1) {
            indent_levels_.pop_back();
            return std::make_unique<Token>(TokenType::DELIMITER, TagType::DEDENT);
        }
        return std::make_unique<Token>(TokenType::END_OF_FILE, TagType::END);
    }

    // Handle indentation
    handle_indentation();

    // If indentation tokens were added, return them first
    if (!token_queue_.empty()) {
        auto token = std::move(token_queue_.front());
        token_queue_.pop_front();
        return token;
    }

  // Handle single-character tokens and operators
  switch (peek_) {
  case '=':
    if (expectch('=')) {
      return std::make_unique<Word>(Word::eq);
    } else {
      peek_ = ' ';
      return std::make_unique<Token>(Token::init);
    }
  case '!':
    if (expectch('=')) {
      return std::make_unique<Word>(Word::ne);
    } else {
      throw LexerError("Unknown token '!' at line " + std::to_string(line_));
    }
  case '>':
    if (expectch('=')) {
      return std::make_unique<Word>(Word::ge);
    } else {
      peek_ = ' ';
      return std::make_unique<Word>(Word::gt);
    }
  case '<':
    if (expectch('=')) {
      return std::make_unique<Word>(Word::le);
    } else {
      peek_ = ' ';
      return std::make_unique<Word>(Word::lt);
    }
  case '+':
    peek_ = ' ';
    return std::make_unique<Token>(Token::add);
  case '-':
    peek_ = ' ';
    return std::make_unique<Token>(Token::sub);
  case '*':
    if (expectch('*')) {
      return std::make_unique<Token>(Token::pow);
    } else {
      peek_ = ' ';
      return std::make_unique<Token>(Token::mul);
    }
  case '/':
    peek_ = ' ';
    return std::make_unique<Token>(Token::div);
  case '%':
    peek_ = ' ';
    return std::make_unique<Token>(Token::mod);
  case ':':
    peek_ = ' ';
    return std::make_unique<Token>(Token::colon);
  case ',':
    peek_ = ' ';
    return std::make_unique<Token>(Token::comma);
  case '(':
    peek_ = ' ';
    return std::make_unique<Token>(Token::lparen);
  case ')':
    peek_ = ' ';
    return std::make_unique<Token>(Token::rparen);
  case '[':
    peek_ = ' ';
    return std::make_unique<Token>(Token::lbracket);
  case ']':
    peek_ = ' ';
    return std::make_unique<Token>(Token::rbracket);
  case '#':
    // Skip comment until end of line
    while (peek_ != '\n' && !source_.eof()) {
      readch();
    }
    return next_token();
  case '"':
  case '\'': {
    char quote_type = peek_;
    std::string value;
    readch();
    if (peek_ == quote_type && expectch(quote_type)) {
      // Multi-line string literal (triple quotes)
      // Skip the third quote
      readch();
      while (!source_.eof()) {
        if (peek_ == quote_type && expectch(quote_type) &&
            expectch(quote_type)) {
          peek_ = ' ';
          return std::make_unique<Word>(
              value, Token(TokenType::STRING, TagType::STRL));
        }
        value += peek_;
        readch();
      }
      throw LexerError("Unterminated multi-line string literal at line " +
                       std::to_string(line_));
    } else {
      // Single-line string literal
      while (peek_ != quote_type && peek_ != '\n' && !source_.eof()) {
        if (peek_ == '\\') {
          readch();
          switch (peek_) {
          case 'n':
            value += '\n';
            break;
          case 't':
            value += '\t';
            break;
          case '\\':
            value += '\\';
            break;
          case '"':
            value += '"';
            break;
          case '\'':
            value += '\'';
            break;
          default:
            value += peek_;
            break;
          }
        } else {
          value += peek_;
        }
        readch();
      }
      if (peek_ == quote_type) {
        peek_ = ' ';
        return std::make_unique<Word>(value,
                                      Token(TokenType::STRING, TagType::STRL));
      } else {
        throw LexerError("Unterminated string literal at line " +
                         std::to_string(line_));
      }
    }
  }
  default:
    break;
  }

  if (std::isdigit(peek_)) {
    // Number literal
    long int_part = 0;
    while (std::isdigit(peek_)) {
      int_part = 10 * int_part + (peek_ - '0');
      readch();
    }
    if (peek_ != '.') {
      return std::make_unique<Integer>(
          int_part, Token(TokenType::NUMBER, TagType::INTEGER));
    }
    // Real number
    double real_part = int_part;
    double factor = 0.1;
    readch();
    while (std::isdigit(peek_)) {
      real_part += (peek_ - '0') * factor;
      factor *= 0.1;
      readch();
    }
    return std::make_unique<Real>(real_part,
                                  Token(TokenType::NUMBER, TagType::REAL));
  }

  if (std::isalpha(peek_) || peek_ == '_') {
    // Identifier or keyword
    std::string lexeme;
    do {
      lexeme += peek_;
      readch();
    } while (std::isalnum(peek_) || peek_ == '_');

    auto it = keywords_.find(lexeme);
    if (it != keywords_.end()) {
      return std::make_unique<Word>(it->second);
    } else {
      return std::make_unique<Word>(
          lexeme, Token(TokenType::IDENTIFIER, TagType::IDENT));
    }
  }

  // Unknown token
  char unknown_char = peek_;
  peek_ = ' ';
  throw LexerError(std::string("Unknown token '") + unknown_char +
                   "' at line " + std::to_string(line_));
}

} // namespace pyc
