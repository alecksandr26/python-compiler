// tag.cpp
#include "tag.hpp"

namespace pyc {
    const std::string TagType::tag_types_str[TagType::TAG_COUNT] = {
        // Operators
        "\"=\"",     // INIT
        "\"+\"",     // ADD
        "\"-\"",     // SUB
        "\"*\"",     // MUL
        "\"/\"",     // DIV
        "\"%\"",     // MOD
        "\"**\"",    // POW
        "AND",
        "OR",
        "NOT",
        "\"==\"",    // EQ
        "\"!=\"",    // NE
        "\">=\"",    // GE
        "\"<=\"",    // LE
        "\">\"",     // GT
        "\"<\"",     // LT

        // Delimiters
        "\"[\"",     // LBRACKET
        "\"]\"",     // RBRACKET
        "\"(\"",     // LPAREN
        "\")\"",     // RPAREN
        "\",\"",     // COMMA
        "\":\"",     // COLON
        "INDENT",    // INDENT
        "DEDENT",    // DEDENT

        // Keywords
        "IF",
        "ELSE",
        "ELIF",
        "WHILE",
        "FOR",
        "DEF",
        "RETURN",
        "BREAK",
        "CONTINUE",
        "PRINT",     // 'print' function

        // Identifiers and literals
        "IDENT",
        "STRL",
        "INTEGER",
        "REAL",

        // Unknown
        "UNKNOWN",

        // End of file/input
        "END"
    };
}
