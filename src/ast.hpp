#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <vector>
#include <ostream>
#include <stack>
#include <cassert>
#include <sstream>
#include <string>

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
		uint8_t type;
		TNode(uint8_t type) : type(type) {}
	};


	class TNodeTerm : public TNode {
	public:
		// Number, String literals, Bool, None, Id (Variables or names or functions) or
		// if token is null means that it is an expression
		Token *token;

		// These nodes must be leafs
		TNodeTerm(void) : TNode(TNodeType::TERM), token(NULL) {}


		friend std::ostream &operator<<(std::ostream &os, const TNodeTerm &node)
		{
			os << "<TNodeTerm>\n";
			os << '\t' << node.token << '\n';
			os << "<EndTNodeTerm>\n";
			return os;
		}
	};

	class TNodeCall : public TNode {
	public:
		// Must to be word name
		Token *token;

		// These nodes must be leaf
		std::vector<TNode *> args;
		

		TNodeCall(void) : TNode(TNodeType::CALL), token(NULL) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeCall &call)
		{
			os << "<TNodeCall>\n";
			os << '\t' << call.token << '\n';
			os << "<EndTNodeCall>\n";
			return os;
		}
	};


	class TNodeOp : public TNode {
	public:
		// Arithmetic And Boolean Operations
		Token *token;

		// All these operations must have two elements at least, this is not the same for the
		// real python grammar but what ever this is what it is
		TNode *left, *right;
		
		TNodeOp(void) : TNode(TNodeType::OPER), token(NULL) {}
		
		friend std::ostream &operator<<(std::ostream &os, const TNodeOp &op)
		{
			os << "<TNodeOp>\n";
			os << '\t' << op.token << '\n';
			os << "<EndTNodeOp>\n";
			return os;
		}
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

		friend std::ostream &operator<<(std::ostream &os, const TNodeInit &init)
		{
			os << "<TNodeInit>\n";
			os << '\t' << init.token << '\n';
			os << "<EndTNodeInit>\n";
			return os;
		}
	};
	
	class TNodeBlock : public TNode {
	public:
		// It will contain a lot of statements as a children nodes
		std::vector<TNode *> stmnts;
		
		TNodeBlock(void) : TNode(TNodeType::BLOCK), stmnts(0) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeBlock &block)
		{
			os << "<TNodeBlock>\n";
			os << '\t' << "address: " << &block << '\n';
			os << "<EndTNodeBlock>\n";
			return os;
		}
	};


	class TNodeBlockOp : public TNode {
	public:
		// This will be nodes to represent the keywords to operate with code blocks
		Token *token;
		
		TNodeBlockOp(void) : TNode(TNodeType::BLOCKOP) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeBlockOp &block_op)
		{
			os << "<TNodeBlockOp>\n";
			os << '\t' << block_op.token << '\n';
			os << "<EndTNodeBlockOp>\n";
			return os;
		}
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

		friend std::ostream &operator<<(std::ostream &os, const TNodeIf &if_block)
		{
			os << "<TNodeIf>\n";
			os << '\t' << if_block.token << '\n';
			os << "<EndTNodeIf>\n";
			return os;
		}
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


		friend std::ostream &operator<<(std::ostream &os, const TNodeWhile &while_block)
		{
			os << "<TNodeWhile>\n";
			os << '\t' << while_block.token << '\n';
			os << "<EndTNodeWhile>\n";
			return os;
		}
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

		friend std::ostream &operator<<(std::ostream &os, const TNodeFor &for_block)
		{
			os << "<TNodeFor>\n";
			os << '\t' << for_block.token << '\n';
			os << "<EndTNodeFor>\n";
			return os;
		}
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

		friend std::ostream &operator<<(std::ostream &os, const TNodeFunc &func_block)
		{
			os << "<TNodeFunc>\n";
			os << '\t' << func_block.token << '\n';
			os << "<EndTNodeFunc>\n";
			return os;
		}
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
		


		// TODO: we need to have a global function kind of static function
		// To print this asbtract syntax tree
		
		friend std::ostream &operator<<(std::ostream &os, const AST &ast)
		{
			long level = 1;
			
			assert(ast.program_ != NULL && "Can't be null");
			
			os << "<Program>" << '\n';
			
			// Create an stream
			std::stringstream ss;
			
			for (TNode *stmnt : ast.program_->stmnts) {
				switch (stmnt->type) {
				case TNodeType::TERM:
					{
						const TNodeTerm *term = static_cast<TNodeTerm *>(stmnt);
						ss << *term;
					}
					break;
				case TNodeType::CALL:
					{
						const TNodeCall *call = static_cast<TNodeCall *>(stmnt);
						ss << *call;
					}
					break;
				case TNodeType::OPER:
					{
						const TNodeOp *op = static_cast<TNodeOp *>(stmnt);
						ss << *op;
					}
					break;
				case TNodeType::INIT:
					{
						const TNodeInit *init = static_cast<TNodeInit *>(stmnt);
						ss << *init;
					}
					break;
				case TNodeType::BLOCK:
					{
						const TNodeBlock *block = static_cast<TNodeBlock *>(stmnt);
						ss << *block;
					}
					break;
				case TNodeType::BLOCKOP:
					{
						const TNodeBlockOp *block_op = static_cast<TNodeBlockOp *>(stmnt);
						ss << *block_op;
					}
					break;
				case TNodeType::IFBLOCK:
					{
						const TNodeIf *if_block = static_cast<TNodeIf *>(stmnt);
						ss << *if_block;
					}
					break;					
				case TNodeType::WHILEBLOCK:
					{
						const TNodeIf *if_block = static_cast<TNodeIf *>(stmnt);
						ss << *if_block;
					}
					break;
				case TNodeType::FORBLOCK:
					{
						const TNodeFor *for_block = static_cast<TNodeFor *>(stmnt);
						ss << *for_block;
					}
					break;
				case TNodeType::FUNCBLOCK:
					{
						const TNodeFunc *func_block = static_cast<TNodeFunc *>(stmnt);
						ss << *func_block;
					}
					break;
				}
			}

			// Build kind of identation for each node to generate kind of tag representaion
			std::string line;
			std::string tabs(level, '\t');  // Create a string with 'level' number of tabs

			// Read the input stream line by line
			while (std::getline(ss, line)) {
				// Append the level number of tabs and print the line
				os << tabs << line << '\n';
			}
			
			
			os << "<EndProgram>" << '\n';
			
			return os;
		}
	};
}

#endif


