#ifndef WORD_INCLUDED
#define WORD_INCLUDED

#include <ostream>
#include <string>
#include "token.hpp"

namespace pyc
{

	class Word : public Token
	{
	private:
		std::string lexeme_;

	public:
		Word(const std::string &lexeme, const Token &token);
		Word(void);
		virtual ~Word(void) = default;

		friend std::ostream &operator<<(std::ostream &os, const Word &word)
		{
			os << "Word<" << TokenType::token_types_str[word.get_type()]
			   << ", \"" << word.lexeme_ << "\">";
			return os;
		}

		const std::string &get_lexeme(void) const;
		

		static Word eq, ne, gt, ge, lt, le;
	};
}

#endif
