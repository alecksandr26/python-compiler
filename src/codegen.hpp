#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#include <map>
#include <string>
#include <sstream> 

#include "ast.hpp"

namespace pyc {
    class CodeGen {
    private:
        llvm::LLVMContext context_;
        llvm::IRBuilder<> builder_;
        llvm::Module module_;
        llvm::Function *mainFunction_;  // Add this line

        std::map<std::string, llvm::Value*> symbolTable_;

        llvm::Value *generateExpr(TNode *expr);
        void generateStmt(TNode *stmt);

    public:
        CodeGen(const std::string &moduleName);
        void generateCode(const AST &ast);
        void saveToFile(const std::string &filename);
        void saveToFile(const std::string &filename, std::stringstream &output_stream_parser);
    };
}

#endif
