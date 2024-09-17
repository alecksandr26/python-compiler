#include <string>
#include "tag.hpp"

// Define the static array outside the class
const std::string pyc::TagType::tag_types_str[TAG_COUNT] = {
	// Names and literals 
	"ID",        // Identifier
	"INTEGER",   // Integer literal
	"REAL",      // Real number literal
	"STRL",      // String literal
	"INDEX",     // Indexing
	"TRUE",      // Boolean true
	"FALSE",     // Boolean false

	// Arithmetic Operations
	"\"=\"",     // INIT
	"\"+\"",     // ADD
	"\"-\"",     // SUB
	"\"*\"",     // MUL
	"\"/\"",     // DIV
	"\"%\"",     // MOD
	"\"**\"",    // POW

	// Basic Logical Operations
	"\">\"",     // GT
	"\">=\"",    // GE
	"\"<\"",     // LT
	"\"<=\"",    // LE
	"\"==\"",    // EQ
	"\"!=\"",    // NEQ
	
	// Boolean Operations
	"AND",
	"OR",
	"NOT",

	// Structural Paradigm
	"IF",
	"ELSE",
	"ELIF",
	"WHILE",
	"FOR",
	"BREAK",
	"CONTINUE",
	"DEF",
	"\":\"",      // IDENT
	
	// List and tuple delimiters
	"\"[\"",      // LBRACKET
	"\"]\"",      // RBRACKET
	"\"(\"",      // LPAREN
	"\")\"",      // RPAREN
	"\",\"",      // COMMA
	
	"UNKNOWN",
};
