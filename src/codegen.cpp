#include "codegen.hpp"
#include <llvm/IR/Verifier.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdio> 
#include <stdexcept> 

using namespace pyc;

CodeGen::CodeGen(const std::string &moduleName)
    : context_(), builder_(context_), module_(moduleName, context_) {
    assert(!moduleName.empty() && "Module name cannot be empty");
    std::cerr << "[DEBUG] CodeGen initialized with module: " << moduleName << "\n";

    // Create a default function and set up the builder's insertion point
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder_.getVoidTy(), false);
    mainFunction_ = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module_);
    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(context_, "entry", mainFunction_);
    builder_.SetInsertPoint(entryBlock);

    std::cerr << "[DEBUG] Created default main function and entry block.\n";
}

void CodeGen::generateCode(const AST &ast) {
    auto *program = ast.get_program();
    assert(program != nullptr && "Program AST node cannot be null");

    std::cerr << "[DEBUG] Starting to process program statements...\n";

    for (auto *stmt : program->stmnts) {
        if (!stmt) {
            std::cerr << "[ERROR] Null statement encountered.\n";
            continue;
        }

        std::cerr << "[DEBUG] Processing statement...\n";
        generateStmt(stmt);
    }

    // Finish function with a return statement
    builder_.CreateRetVoid();

    // Validate the generated LLVM IR
    if (llvm::verifyModule(module_, &llvm::errs())) {
        std::cerr << "[ERROR] Invalid LLVM IR generated.\n";
        return;
    }

    std::cerr << "[DEBUG] LLVM IR generation completed successfully.\n";
}

llvm::Value *CodeGen::generateExpr(TNode *expr) {
    if (!expr) {
        std::cerr << "[ERROR] Null expression encountered.\n";
        return nullptr;
    }

    std::cerr << "[DEBUG] Generating expression of type: " << expr->type << "\n";

    switch (expr->type) {
        case TNodeType::TERM: {
            auto *term = static_cast<TNodeTerm *>(expr);
            if (!term || !term->token) {
                std::cerr << "[ERROR] Null term or token in TERM node.\n";
                return nullptr;
            }

            if (term->token->get_type() == TokenType::NUMBER) {
                auto *integer = static_cast<const Integer *>(term->token);
                return llvm::ConstantInt::get(
                    llvm::Type::getInt32Ty(context_), integer->get_value());
            } else if (term->token->get_type() == TokenType::IDENTIFIER) {
                const Word *word = static_cast<const Word *>(term->token);
                if (symbolTable_.count(word->get_lexeme()) == 0) {
                    std::cerr << "[ERROR] Variable not found in symbol table: " << word->get_lexeme() << "\n";
                    return nullptr;
                }
                return builder_.CreateLoad(
                    llvm::Type::getInt32Ty(context_), symbolTable_[word->get_lexeme()], word->get_lexeme());
            }
            break;
        }
        case TNodeType::OPER: {
            auto *op = static_cast<TNodeOp *>(expr);
            if (!op || !op->token) {
                std::cerr << "[ERROR] Null operation or token in OPER node.\n";
                return nullptr;
            }

            std::cerr << "[DEBUG] Generating operation of type: " << op->token->get_tag() << "\n";

            llvm::Value *lhs = generateExpr(op->left);
            llvm::Value *rhs = generateExpr(op->right);

            if (!lhs || !rhs) {
                std::cerr << "[ERROR] Null operand encountered in binary operation.\n";
                return nullptr;
            }

            switch (op->token->get_tag()) {
                case TagType::ADD:
                    return builder_.CreateAdd(lhs, rhs, "addtmp");
                case TagType::SUB:
                    return builder_.CreateSub(lhs, rhs, "subtmp");
                case TagType::MUL:
                    return builder_.CreateMul(lhs, rhs, "multmp");
                case TagType::DIV:
                    return builder_.CreateSDiv(lhs, rhs, "divtmp");
                default:
                    std::cerr << "[ERROR] Unsupported operator type.\n";
                    return nullptr;
            }
        }
        default:
            std::cerr << "[ERROR] Unsupported expression type: " << expr->type << "\n";
            return nullptr;
    }

    return nullptr;
}

void CodeGen::generateStmt(TNode *stmt) {
    if (!stmt) {
        std::cerr << "[ERROR] Null statement encountered.\n";
        return;
    }

    std::cerr << "[DEBUG] Generating statement of type: " << stmt->type << "\n";

    switch (stmt->type) {
        case TNodeType::INIT: {
            auto *init = static_cast<TNodeInit *>(stmt);
            if (!init || !init->id || !init->expr) {
                std::cerr << "[ERROR] Null INIT node or its components.\n";
                return;
            }

            const Word *id_word = dynamic_cast<const Word *>(init->id->token);
            assert(id_word && "Token is not a Word");

            llvm::Value *value = generateExpr(init->expr);
            if (!value) {
                std::cerr << "[ERROR] Failed to generate expression for INIT statement.\n";
                return;
            }

            llvm::Value *allocaInst = builder_.CreateAlloca(
                llvm::Type::getInt32Ty(context_), nullptr, id_word->get_lexeme());
            if (!allocaInst) {
                std::cerr << "[ERROR] Failed to allocate memory for: " << id_word->get_lexeme() << "\n";
                return;
            }

            builder_.CreateStore(value, allocaInst);
            symbolTable_[id_word->get_lexeme()] = allocaInst;
            std::cerr << "[DEBUG] Variable added to symbol table: " << id_word->get_lexeme() << "\n";
            break;
        }
        case TNodeType::CALL: {
            auto *call = static_cast<TNodeCall *>(stmt);
            const Word *func_word = dynamic_cast<const Word *>(call->token);
            assert(func_word && "Token is not a Word");

            llvm::Function *func = module_.getFunction(func_word->get_lexeme());
            if (!func) {
                llvm::FunctionType *funcType = llvm::FunctionType::get(
                    builder_.getVoidTy(), {builder_.getInt32Ty()}, false);
                func = llvm::Function::Create(
                    funcType, llvm::Function::ExternalLinkage, func_word->get_lexeme(), module_);
            }

            std::vector<llvm::Value *> args;
            for (auto *arg : call->args) {
                llvm::Value *arg_value = generateExpr(arg);
                if (!arg_value) {
                    std::cerr << "[ERROR] Null argument in function call: " << func_word->get_lexeme() << "\n";
                    return;
                }
                args.push_back(arg_value);
            }

            builder_.CreateCall(func, args);
            break;
        }
        default:
            std::cerr << "[ERROR] Unsupported statement type.\n";
            break;
    }
}

void CodeGen::saveToFile(const std::string &filename) {
    std::error_code ec;
    llvm::raw_fd_ostream file(filename, ec, llvm::sys::fs::OF_None);
    if (ec) {
        std::cerr << "[ERROR] Error opening file: " << ec.message() << "\n";
        return;
    }

    module_.print(file, nullptr);
    std::cerr << "[DEBUG] LLVM IR written successfully.\n";
}


void runBinaryAndCaptureOutput(std::stringstream &output_stream_log_parser) {
    const std::string command = "./example_binary";
    std::cerr << "[DEBUG] Running command: " << command << "\n";

    // Open a pipe to the command using popen
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "[ERROR] Failed to open pipe for command: " << command << "\n";
        throw std::runtime_error("Failed to open pipe");
    }

    // Read the output of the command line by line
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output_stream_log_parser << buffer; // Append each line to the stringstream
    }

    // Close the pipe and check the command's return status
    int return_status = pclose(pipe);

    std::cerr << "[DEBUG] Command output captured successfully.\n";
}



void CodeGen::saveToFile(const std::string &filename, std::stringstream &output_stream_parser, std::stringstream &output_stream_log_parser) {
    // Save LLVM IR to file
    std::error_code ec;
    llvm::raw_fd_ostream file(filename, ec, llvm::sys::fs::OF_None);
    if (ec) {
        std::cerr << "[ERROR] Error opening file: " << ec.message() << "\n";
        return;
    }

    // Use an intermediate string to pass to raw_string_ostream
    std::string asmOutput;
    llvm::raw_string_ostream string_stream(asmOutput);

    // Write LLVM IR to both the file and the stringstream
    module_.print(file, nullptr);

    // Flush the raw_string_ostream and copy the content to the output stream
    string_stream.flush();
    output_stream_parser << asmOutput;

    std::cerr << "[DEBUG] LLVM IR written successfully to file and stream.\n";

    std::string asmFile = filename + ".s"; // Output assembly file

    std::string commands [] = {"llc -filetype=asm " + filename + " -o " + asmFile, 
                            "clang -c print.c -o print.o", 
                            "llc -filetype=obj " + filename + " -o example.o", 
                            "clang example.o print.o -o example_binary"
                            };

    int size = sizeof(commands) / sizeof(commands[0]);

    for (int i = 0; i < size; i++) {
         std::cerr << "[DEBUG] Running command: " << commands[i] << "\n";
         if (std::system(commands[i].c_str()) != 0) {
            std::cerr << "[ERROR] Failed to run llc command.\n";
            return;
        }
    }

    std::cerr << "[DEBUG] Running command: ./example_binary " << "\n";
    runBinaryAndCaptureOutput(output_stream_log_parser);
    
    // Read the generated assembly file
    std::ifstream asmStream(asmFile);
    if (!asmStream.is_open()) {
        std::cerr << "[ERROR] Failed to open generated assembly file: " << asmFile << "\n";
        return;
    }

    // Append the assembly content to the output stream
    std::string line;
    while (std::getline(asmStream, line)) {
        output_stream_parser << line << "\n";
    }

    asmStream.close();

    std::cerr << "[DEBUG] Assembly content appended to the output stream.\n";
}

