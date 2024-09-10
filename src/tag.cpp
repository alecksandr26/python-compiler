#include <string>
#include "tag.hpp"


// Define the static array outside the class
const std::string pyc::Tag::tag_types_str[TAG_COUNT] = {
	"ID",
	"INIT",
	"NUM",
	"REAL",
	"INDEX",
	"TRUE",
	"FALSE",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"POW",
	"GT",
	"GE",
	"LT",
	"LE",
	"EQ",
	"AND",
	"OR",
	"NOT",
	"IF",
	"ELSE",
	"ELIF",
	"WHILE",
	"FOR",
	"BREAK",
	"CONTINUE",
	"DEF"
};


