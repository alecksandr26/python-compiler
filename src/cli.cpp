#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <regex>

#include "semantic.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "ident.hpp"
#include "codegen.hpp"

struct {
    std::string output_file, source_file;
} compiler_params = {
    .output_file = "a.out",
    .source_file = "",
};

void test_lexer(std::ifstream &source)
{
    std::cerr << "[DEBUG] Starting lexer test...\n";
    pyc::Lexer lexer(source);
    
    while (lexer.is_token_available()) {
        const pyc::Token &token = lexer.next_token();
        std::cerr << "[DEBUG] Token: " << token << std::endl;

        switch (token.get_type()) {
        case pyc::TokenType::IDENTIFIER:
        case pyc::TokenType::KEYWORD:
        case pyc::TokenType::STRING: {
            const pyc::Word &word = static_cast<const pyc::Word &>(token);
            std::cout << word << std::endl;
            break;
        }
        case pyc::TokenType::NUMBER: {
            if (token.get_tag() == pyc::TagType::INTEGER) {
                const pyc::Integer &integer = static_cast<const pyc::Integer &>(token);
                std::cout << integer << std::endl;
            } else {
                const pyc::Real &real = static_cast<const pyc::Real &>(token);
                std::cout << real << std::endl;
            }
            break;
        }
        case pyc::TokenType::DELIMITER: {
            if (token.get_tag() == pyc::TagType::IDENT) {
                const pyc::Ident &ident = static_cast<const pyc::Ident &>(token);
                std::cout << ident << std::endl;
                break;
            }
        }
        default:
            std::cout << token << std::endl;
            break;
        }
    }
    std::cerr << "[DEBUG] Lexer test completed.\n";
}

int main(int argc, char *argv[])
{
    std::cerr << "[DEBUG] Starting CLI...\n";

    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);

        if (arg == "-o") {
            assert(i + 1 < argc && "CLI: Error compiler needs to have an output file");
            compiler_params.output_file = std::string(argv[i + 1]);
            i++;    // Move to the next
        } else {
            compiler_params.source_file = std::string(argv[i]);
            std::regex pattern(R"(\.py$)");

            assert(std::regex_search(compiler_params.source_file, pattern)
                   && "CLI: Error compiler needs to have a valid input file");
        }
    }

    assert(compiler_params.source_file != ""
           && "CLI: Error compiler needs to have an input file");

    // Debugging: Log the input file and output file
    std::cerr << "[DEBUG] Source file: " << compiler_params.source_file << "\n";
    std::cerr << "[DEBUG] Output file: " << compiler_params.output_file << ".ll\n";

    std::ifstream source(compiler_params.source_file);
    if (!source.is_open()) {
        std::cerr << "Error: Could not open source file.\n";
        return 1;
    }

    // Debugging: Parser and Semantic Analysis
    std::cerr << "[DEBUG] Starting parsing and semantic analysis...\n";
    pyc::Parser parser(source);
    try {
        parser.parse();
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Parser error: " << e.what() << "\n";
        return 1;
    }
    std::cerr << "[DEBUG] Parsing completed.\n";

    pyc::Semantic semantic(parser.get_ast());
    try {
        semantic.analyze();
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Semantic analysis error: " << e.what() << "\n";
        return 1;
    }
    std::cerr << "[DEBUG] Semantic analysis completed.\n";

    // Debugging: LLVM Code Generation
    std::cerr << "[DEBUG] Starting LLVM code generation...\n";
    pyc::CodeGen codegen("PythonCompiler");
    try {
        codegen.generateCode(parser.get_ast());
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Code generation error: " << e.what() << "\n";
        return 1;
    }
    std::cerr << "[DEBUG] Code generation completed.\n";

    try {
        codegen.saveToFile(compiler_params.output_file + ".ll");
    } catch (const std::exception &e) {
        std::cerr << "[ERROR] Error saving LLVM IR to file: " << e.what() << "\n";
        return 1;
    }
    std::cerr << "[DEBUG] LLVM IR saved to " << compiler_params.output_file + ".ll\n";

    std::cout << "LLVM IR saved to " << compiler_params.output_file + ".ll" << std::endl;
    return 0;
}
