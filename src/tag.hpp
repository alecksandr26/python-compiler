#ifndef TAG_INCLUDED
#define TAG_INCLUDED

#include <string>

namespace pyc {
	class Tag {
	public:
		enum TagTypes {
			// Variable and literals 
			ID = 0,	// variables
			INIT,	// =
			INTEGER,
			REAL,
			STRL,	// strings lieterals
			INDEX,
			TRUE,
			FALSE,
			
			// Arithmetic Operations
			ADD,
			SUB,
			MUL,
			DIV,
			MOD,
			POW,


			// Basic Logical Operations
			GT,	// >
			GE,	// >=
			LT,	// <
			LE,	// <=
			EQ,	// ==

			// Boolean Operations
			AND,
			OR,
			NOT,

			// Structural Paradigm
			IF,
			ELSE,
			ELIF,
			WHILE,
			FOR,
			BREAK,
			CONTINUE,
			DEF,	// functions
			
			// -------------------------------
			TAG_COUNT  // Total number of tags
		};
		
		static const std::string tag_types_str[TAG_COUNT];
	};
}

#endif





