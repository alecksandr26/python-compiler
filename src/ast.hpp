#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <vector>
#include <ostream>
#include <stack>
#include "token.hpp"

namespace pyc {
	class TNodeType {
	public:
		enum TNodeTypeEnum {
			TERM,
			CALL,
			OPER,
			INIT,
			BLOCK,
			IFBLOCK,
			WHILEBLOCK,
			FORBLOCK,
			FUNCBLOCK,
			BLOCKOP,
			
			TNODETYPE_COUNT
		};

		static const std::string tnode_types_str[];
	};


	
	class TNode {
	public:
		uint8_t type_;
		TNode(uint8_t type) : type_(type) {}
	};


	class TNodeTerm : public TNode {
	public:
		// Number, String literals, Bool, None, Id (Variables or names or functions) or
		// if token is null means that it is an expression
		Token *token;

		// These nodes must be leafs
		TNodeTerm(void) : TNode(TNodeType::TERM), token(NULL) {}
	};

	class TNodeCall : public TNode {
	public:
		// Must to be word name
		Token *token;

		// These nodes must be leafs

		TNodeCall(void) : TNode(TNodeType::CALL), token(NULL) {}
	};


	class TNodeOp : public TNode {
	public:
		// Arithmetic And Boolean Operations
		Token *token;

		// All these operations must have two elements at least, this is not the same for the
		// real python grammar but what ever this is what it is
		TNode *left, *right;
		
		TNodeOp(void) : TNode(TNodeType::OPER), token(NULL) {}
	};


	class TNodeInit : public TNode {
	public:
		// The '=' assign token
		Token *token;
		
		// The id variable to be define
		TNodeTerm *id;


		// NOTICE: Then the expression, notice that an expression could a simple term, like
		// a harcode number, then this pointer could be in reality a TNodeTerm and not a
		// TNodeOp, rememebr always first check its type, so be alert :)
		TNode *expr;

		
		TNodeInit(void) : TNode(TNodeType::INIT) {}
	};
	
	class TNodeBlock : public TNode {
	public:
		// It will contain a lot of statements as a children nodes
		std::vector<TNode *> stms;
		
		TNodeBlock(void) : TNode(TNodeType::BLOCK), stms(0) {}
	};


	class TNodeBlockOp : public TNode {
	public:
		// This will be nodes to represent the keywords to operate with code blocks
		Token *token;
		
		TNodeBlockOp(void) : TNode(TNodeType::BLOCKOP) {}
	};

	class TNodeIf : public TNode {
	public:
		// The if, elif token
		Token *token;
		
		// The condition
		TNode *cond;

		// The body of structure all the statments inside of the If
		TNodeBlock *block;

		// If there is other else or elif, we dont use TNodeIf because we don't know if
		// there will be an elif or a simple else, in case of else will be a simple TNodeBlock
		// In case of be an elif will be another TNodeIf
		TNode *or_else;
		
		TNodeIf(void) : TNode(TNodeType::IFBLOCK) {}
	};


	class TNodeWhile : public TNode {
	public:
		// The while token
		Token *token;

		// The condition
		TNode *cond;

		// The body of structure all the statments inside of the If
		TNodeBlock *block;
		
		TNodeWhile(void) : TNode(TNodeType::WHILEBLOCK) {}
	};


	class TNodeFor : public TNode {
	public:
		// For token
		Token *token;
		// IDK how this thing will work, but i think it should be something like this

		// The iterable element
		TNodeTerm *element;
		
		// The function that will make advance to the next element
		TNodeCall *iterate;
		
		// The body of the structure all the statement inside of the if
		TNodeBlock *block;
		
		TNodeFor(void) : TNode(TNodeType::FORBLOCK) {}
	};

	class TNodeFunc : public TNode {
	public:
		// TODO: Instaed of putting just the token try to use TNodeTerm's
		// In the future we could have objec.metho_1() that will be a entire func call
		// we need to parse the dot points
		// The name of the function
		Token *token;



		std::vector<Token *> params;
		
		// The block of code 
		TNodeBlock *block;

		TNodeFunc(void) : TNode(TNodeType::FUNCBLOCK) {}
	};
	
	
	
	class AST {
	private:
		// The root node of the whole program
		TNodeBlock *program_, *curr_block_;
		

		// To keep track in which block of code we are constructing
		std::stack<TNodeBlock *> blocks_;
		
		
	public:
		AST(void);
		
		// IT will stack the previos one
		void set_new_block(TNodeBlock *block);

		// Appends stmnt to the current block
		void append_new_stmt(TNode *stmnt);

		
		// Pops back and move back to the previous code block
		void pop_block(void);
		
		
		friend std::ostream &operator<<(std::ostream &os, const AST &ast)
		{
			os << "AST ---- printing";
			return os;
		}
	};
}

#endif


