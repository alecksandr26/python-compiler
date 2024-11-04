#ifndef AST_INCLUDED
#define AST_INCLUDED

#include <vector>
#include <ostream>
#include <stack>
#include <cassert>
#include <sstream>
#include <string>

#include "token.hpp"
#include "word.hpp"
#include "integer.hpp"
#include "real.hpp"

namespace pyc {
	class TNodeType {
	public:
		enum TNodeTypeEnum {
			TERM = 0,
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
	
	std::stringstream ast_fetch_stream_node(TNode *node);

	class TNodeTerm : public TNode {
	public:
		// Number, String literals, Bool, None, Id (Variables or names or functions) or
		// if token is null means that it is an expression
		const Token *token;

		// These nodes must be leafs
		TNodeTerm(void) : TNode(TNodeType::TERM), token(NULL) {}


		friend std::ostream &operator<<(std::ostream &os, const TNodeTerm &node)
		{
			os << "<Term>\n";
			switch (node.token->get_type()) {
			case TokenType::STRING:
			case TokenType::IDENTIFIER:
				{
					const Word *word = static_cast<const Word *>(node.token);
					os << '\t' << *word << '\n';
				}
				break;
			case TokenType::NUMBER:
				switch (node.token->get_tag()) {
				case TagType::INTEGER:
					{
						const Integer *integer = \
							static_cast<const Integer *>(node.token);
						os << '\t' << *integer << '\n';
					}
					break;
				case TagType::REAL:
					{
						const Real *real = static_cast<const Real *>(node.token);
						os << '\t' << *real << '\n';
					}
					break;					
				}
				break;
			default:
				// This could fail
				os << '\t' << *node.token << '\n';
				break;
			}
			
			os << "<EndTerm>\n";
			return os;
		}
	};

	class TNodeCall : public TNode {
	public:
		// Must to be word name
		const Token *token;

		// These nodes must be leaf
		std::vector<TNode *> args;
		

		TNodeCall(void) : TNode(TNodeType::CALL), token(NULL) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeCall &call)
		{
			os << "<Call>\n";
			assert(call.token->get_type() == TokenType::IDENTIFIER && "It must be an identifier");
			const Word *word = static_cast<const Word *>(call.token);
			os << '\t' << *word << '\n';

			os << '\t' << "<Arguments>" << '\n';

			for (TNode *node : call.args) {
				std::stringstream ss = ast_fetch_stream_node(node);
				std::string line;
				// Add another tab for each line, to ident the arguments sectionc
				while (std::getline(ss, line)) {
					os << '\t' << line << '\n';
				}
			}
			os << '\t' << "<EndArguments>" << '\n';
			
			os << "<EndCall>\n";
			return os;
		}
	};


	class TNodeOp : public TNode {
	public:
		// Arithmetic And Boolean Operations
		const Token *token;

		// All these operations must have two elements at least, this is not the same for the
		// real python grammar but what ever this is what it is
		TNode *left, *right;
		
		TNodeOp(void) : TNode(TNodeType::OPER), token(NULL) {}
		
		friend std::ostream &operator<<(std::ostream &os, const TNodeOp &op)
		{
			os << "<Op>\n";
			
			assert(op.token->get_type() == TokenType::OPERATOR && "It must be an operator");

			os << ast_fetch_stream_node(op.left).str();
			os << '\t' << *op.token << '\n';
			os << ast_fetch_stream_node(op.right).str();
			
			os << "<EndOp>\n";
			return os;
		}
	};


	class TNodeInit : public TNode {
	public:
		// The '=' assign token
		const Token *token;
		
		// The id variable to be define
		TNodeTerm *id;


		// NOTICE: Then the expression, notice that an expression could a simple term, like
		// a harcode number, then this pointer could be in reality a TNodeTerm and not a
		// TNodeOp, rememebr always first check its type, so be alert :)
		TNode *expr;

		
		TNodeInit(void) : TNode(TNodeType::INIT) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeInit &init)
		{
			os << "<Init>\n";

			// Must be the token '='
			assert(init.token->get_type() == TokenType::OPERATOR
			       && init.token->get_tag() == TagType::INIT);

			os << ast_fetch_stream_node(init.id).str();			
			os << '\t' << *init.token << '\n';
			os << ast_fetch_stream_node(init.expr).str();
			
			os << "<EndInit>\n";
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
			os << "<Block>\n";

			for (TNode *node : block.stmnts) {
				std::stringstream ss = ast_fetch_stream_node(node);
				os << ss.str();
			}
			
			os << "<EndBlock>\n";
			return os;
		}
	};


	class TNodeBlockOp : public TNode {
	public:
		// This will be nodes to represent the keywords to operate with code blocks
		const Token *token;

		// This coulde be null because only the 'return' could have an expression

		TNode *expr;
		
		TNodeBlockOp(void) : TNode(TNodeType::BLOCKOP) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeBlockOp &block_op)
		{
			os << "<BlockOp>\n";
			assert(block_op.token->get_type() == TokenType::KEYWORD);
			const Word *word = static_cast<const Word *>(block_op.token);
			os << '\t' << *word << '\n';
			if (block_op.token->get_tag() == TagType::RETURN) {
				assert(block_op.expr != NULL && "Since it is a return must to have an expr");
				std::stringstream ss = ast_fetch_stream_node(block_op.expr);
				os << ss.str();
			}
				
			os << "<EndBlockOp>\n";
			return os;
		}
	};

	class TNodeIf : public TNode {
	public:
		// The if, elif token
		const Token *token;
		
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
			os << "<If>\n";
			const Word *word = static_cast<const Word *>(if_block.token);
			os << '\t' << *word << '\n';
			os << '\t' << "<Cond>" << '\n';
			// So print the condition
			std::stringstream ss = ast_fetch_stream_node(if_block.cond);
			std::string line;
			while (std::getline(ss, line)) {
				os << '\t' << line << '\n';
			}
			os << '\t' << "<EndCond>" << '\n';
			
			// Print the code block
			ss = ast_fetch_stream_node(if_block.block);
			os << ss.str();

			if (if_block.or_else != NULL) {
				os << '\t' << "<Else>" << '\n';
				std::stringstream else_s = ast_fetch_stream_node(if_block.or_else);
				while (std::getline(else_s, line)) {
					os << '\t' << line << '\n';
				}
				os << '\t' << "<EndElse>" << '\n';
			}
				 
			os << "<EndIf>\n";
			return os;
		}
	};


	class TNodeWhile : public TNode {
	public:
		// The while token
		const Token *token;

		// The condition
		TNode *cond;

		// The body of structure all the statments inside of the If
		TNodeBlock *block;
		
		TNodeWhile(void) : TNode(TNodeType::WHILEBLOCK) {}
		

		friend std::ostream &operator<<(std::ostream &os, const TNodeWhile &while_block)
		{
			os << "<While>\n";
			const Word *word = static_cast<const Word *>(while_block.token);
			os << '\t' << *word << '\n';
			os << '\t' << "<Cond>" << '\n';
			// So print the condition
			std::stringstream ss = ast_fetch_stream_node(while_block.cond);
			std::string line;
			while (std::getline(ss, line)) {
				os << '\t' << line << '\n';
			}
			os << '\t' << "<EndCond>" << '\n';

			// Print the code block
			ss = ast_fetch_stream_node(while_block.block);
			os << ss.str();
			
			os << "<EndWhile>\n";
			return os;
		}
	};


	class TNodeFor : public TNode {
	public:
		// For token
		const Token *token;
		// IDK how this thing will work, but i think it should be something like this

		// The iterable element
		TNode *element;
		
		// The function or object to iterate
		TNode *iterate;
		
		// The body of the structure all the statement inside of the if
		TNodeBlock *block;
		
		TNodeFor(void) : TNode(TNodeType::FORBLOCK) {}
		
		friend std::ostream &operator<<(std::ostream &os, const TNodeFor &for_block)
		{
			os << "<For>\n";
			const Word *word = static_cast<const Word *>(for_block.token);
			os << '\t' << *word << '\n';
			os << '\t' << "<Element>" << '\n';
			std::stringstream ss = ast_fetch_stream_node(for_block.element);
			std::string line;
			while (std::getline(ss, line)) {
				os << '\t' << line << '\n';
			}
			os << '\t' << "<EndElement>" << '\n';
			os << '\t' << "<Iter>" << '\n';
			ss = ast_fetch_stream_node(for_block.iterate);
			while (std::getline(ss, line)) {
				os << '\t' << line << '\n';
			}
			os << '\t' << "<EndIter>" << '\n';
			ss = ast_fetch_stream_node(for_block.block);
			os << ss.str();
			
			os << "<EndFor>\n";
			return os;
		}
	};

	class TNodeFunc : public TNode {
	public:
		// TODO: Instaed of putting just the token try to use TNodeTerm's
		// In the future we could have objec.metho_1() that will be a entire func call
		// we need to parse the dot points
		// The name of the function
		const Token *token;
		

		std::vector<const Token *> params;
		
		// The block of code 
		TNodeBlock *block;

		TNodeFunc(void) : TNode(TNodeType::FUNCBLOCK) {}

		friend std::ostream &operator<<(std::ostream &os, const TNodeFunc &func_block)
		{
			os << "<Func>\n";
			
			assert(func_block.token->get_type() == TokenType::IDENTIFIER
			       && "It must be an identifier");
			
			
			const Word *word = static_cast<const Word *>(func_block.token);
			os << '\t' << *word << '\n';

			// Print the params for this function
			os << '\t' << "<Params>" << '\n';
			for (const Token *token : func_block.params) {
				const Word *param = static_cast<const Word *>(token);
				os << '\t' << '\t' << *param << '\n';
			}
			os << '\t' << "<EndParams>" << '\n';
			// Print the code block
			std::stringstream ss = ast_fetch_stream_node(func_block.block);
			os << ss.str();

			os << "<EndFunc>\n";
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
		void append_new_stmnt(TNode *stmnt);
		
		// Pops back and move back to the previous code block
		void pop_block(void);
		TNodeBlock *top_block(void);

		TNodeBlock *get_program(void) const;

		
		// TODO: we need to have a global function kind of static function
		// To print this asbtract syntax tree
		
		friend std::ostream &operator<<(std::ostream &os, const AST &ast)
		{

			
			assert(ast.program_ != NULL && "Can't be null");
			
			os << "<Program>" << '\n';
			
			for (TNode *node : ast.program_->stmnts) {
				std::stringstream ss = ast_fetch_stream_node(node);
				os << ss.str();
			}
			
			os << "<EndProgram>" << '\n';
			
			return os;
		}
	};
}

#endif


