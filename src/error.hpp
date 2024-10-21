#ifndef ERROR_HPP
#define ERROR HPP

#include <stdexcept>

// The errors messages
#define ERROR_UNKNOW_TOKEN_MSG "Unknow token"
#define ERROR_NO_NEXT_TOKEN "there isn't exist a next token"
#define ERROR_IN_STREAM_NOT_OPENED "the in stream is not opened or can't opened"
#define ERROR_UNTERMINATED_STRING_MUL_LIT "unterminated multi-line string literal"
#define ERROR_UNTERMINATED_STRING_LIT "unterminated string literal"

#define ERROR_EXPECT_FUNC_NAME "expect to have a function name for a function definition"
#define ERROR_EXPECT_LPAREM "expect to have an left parentheses '('"
#define ERROR_EXPECT_RPAREM "expect to have an left parentheses ')'"
#define ERROR_EXPECT_PARAM "expect to have another parameter"
#define ERROR_EXPECT_COLON_FOR_CODE_BLOCK "expect to have a colon for a new code block"
#define ERROR_EXPECT_END_OF_LINE "expect to have an end of line"
#define ERROR_EXPECT_END_OF_LINE_OR_NEW_IDENT_BLOCK "expect to have an end of line or new ident block"
#define ERROR_EXPECT_OP_BLOCK "expect to have at least one operational block statement 'pass, continue, " \
	"return, break'"
#define ERROR_EXPECT_INITALIZATION "expect to have an initialization"
#define ERROR_EXPECT_PREV_IF_OR_ELIF_BLOCK "expect to have previous if or elif statement"
#define ERROR_EXPECT_ITERATOR_VAR "expect to have an iterator variable"
#define ERROR_EXPECT_INIT_OR_CALL "expect to be an initialization or a function call"
#define ERROR_EXPECT_IN "expect to have an in keyword"
#define ERROR_UNKNOW_NUMBER_TERM "unknow type of number"
#define ERROR_UNKNOW_BOOL_TERM "unknow boolean value"
#define ERROR_UNKNOW_TERM "unknow term"
#define ERROR_IDENT_NOT_MATCH "identation don't math"


// To hanlde runtime errors in the compiler
namespace pyc {
	class LogLexerError : public std::runtime_error {
	public:
		// Constructor that takes a message and passes it to std::runtime_error
		explicit LogLexerError(const std::string& message);
	};

	class LogParserError : public std::runtime_error {
	public:
		explicit LogParserError(const std::string &message, int line);
	};
}


#endif


