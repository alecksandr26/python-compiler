// tag.hpp
#ifndef TAG_INCLUDED
#define TAG_INCLUDED

#include <string>

namespace pyc {
    class TagType {
    public:
        enum TagTypeEnum {
            // Operators
            INIT,
            ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            POW,
            AND,
            OR,
            NOT,
            EQ,
            NE,
            GE,
            LE,
            GT,
            LT,

            // Delimiters
            LBRACKET,
            RBRACKET,
            LPAREN,
            RPAREN,
            COMMA,
            COLON,
            INDENT,   // Added for indentation handling
            DEDENT,   // Added for indentation handling

            // Keywords
            IF,
            ELSE,
            ELIF,
            WHILE,
            FOR,
            DEF,
            RETURN,
            BREAK,
            CONTINUE,
            PRINT,    // Added for 'print' function

            // Identifiers and literals
            IDENT,
            STRL,
            INTEGER,
            REAL,

            // Unknown
            UNKNOWN,

            // End of file/input
            END,

            TAG_COUNT // Used for array size
        };

        static const std::string tag_types_str[TAG_COUNT];
    };
}

#endif // TAG_INCLUDED
