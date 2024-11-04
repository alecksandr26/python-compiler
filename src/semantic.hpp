#ifndef SEMANTIC_INCLUDED
#define SEMANTIC_INCLUDED

#include <fstream>
#include <unordered_map>
#include <cassert>
#include <string>

#include "ast.hpp"
#include "token.hpp"


// At the moment the semantic analysis just builds the symbol table
// Containing the the scope where the definitions

namespace pyc {
	struct Symbol {
		const Token *token;
		const TNodeBlock *block;

		// It could contain more things like line numbers from where this symbol is defined etc
	};
	
	class Semantic {
	private:
		const AST &ast_;
		std::unordered_map<std::string, Symbol> sym_table_;
		std::vector<TNodeBlock *> scopes_;

		void analyze_expr(TNode *expr, TNodeBlock *block);
		void analyze_stmnt(TNode *stmnt, TNodeBlock *block);
		void analyze_block(TNodeBlock *block);
		
	public:
		Semantic(const AST &ast);


		friend std::ostream &operator<<(std::ostream &os, const Semantic &semantic)
		{

			os << "Symbol Table: \n";
			for (const auto &pair : semantic.sym_table_) {
				assert(pair.second.token->get_type() == TokenType::IDENTIFIER);
				const Word *word = static_cast<const Word *>(pair.second.token);
				os << *word << '\n';
			}
			return os;
		}
		
		void analyze(void);
	};
}



#endif



