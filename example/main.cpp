#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>  // For runtime_error and custom exception

#include "tag.hpp"
#include "word.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "lexer.hpp"

// Custom exception class for file errors
class FileError : public std::runtime_error {
public:
	explicit FileError(const std::string& message)
		: std::runtime_error("File Error: " + message) {}
};

using namespace pyc;

int main(int argc, char* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
		return 1;
	}

	const std::string file_path = argv[1];
	std::ifstream source(file_path);

	if (!source.is_open()) {
		throw FileError("The file '" + file_path + "' could not be opened.");
	}

	Lexer lexer(source);

	try {
		while (lexer.is_token_available()) {
			const Token &token = lexer.next_token();

			switch (token.get_type()) {
			case TokenType::IDENTIFIER:
			case TokenType::KEYWORD:
			case TokenType::STRING: {  // Case for strings
				const Word &word = static_cast<const Word&>(token);
				std::cout << word << std::endl;
				break;
			}
			case TokenType::NUMBER: {
				if (token.get_tag() == TagType::INTEGER) {
					const Integer &inter = static_cast<const Integer &>(token);
					std::cout << inter << std::endl;
				} else {
					const Real &real = static_cast<const Real &>(token);
					std::cout << real << std::endl;
				}
				break;
			}
			default:
				std::cout << token << std::endl;
				break;
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "Error while processing tokens: " << e.what() << std::endl;
		return 1;
	}

	source.close();
	return 0;
}
