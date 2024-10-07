#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <vector>
#include <ostream>
#include "token.hpp"

namespace pyc {
	class TNodeType {
	public:
		enum TNodeTypeEnum {
			TERM,
			CALL,
			ARITH,
			
			TNODETYPE_COUNT
		};

		static const std::string tnode_types_str[];
	};


	
	class TNode {
	public:
		std::vector<TNode *> children;
		uint8_t type_;
	};


	class TNodeTerm : public TNode {
	public:
		// Number, String literals, Bool, None, Id (Variables or names or functions) or
		// if token is null means that it is an expression
		Token *token;
		
		TNodeTerm(void) : token(NULL) {}
	};

	class TNodeCall : public TNode {
	public:
		// Must to be word name
		Token *token;

		TNodeCall(void) : token(NULL) {}
	};


	class TNodeArith : public TNode {
	public:
		// Arithmetic Operations
		Token *token;
		
		TNodeArith(void) : token(NULL) {}
	};
	
	
	class AST {
	private:
		TNode *root_;
		
	public:
		AST(void);
		
		friend std::ostream &operator<<(std::ostream &os, const AST &ast)
		{
			os << "AST ---- printing";
			return os;
		}
		
	};
	
}


#endif


