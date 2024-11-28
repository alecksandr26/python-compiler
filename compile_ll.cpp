#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FormattedStream.h>

#include <memory>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <input.ll> <output.o>" << std::endl;
		return 1;
	}

	// Initialize LLVM components
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmPrinters();
	llvm::InitializeAllAsmParsers();

	std::string inputFilename = argv[1];
	std::string outputFilename = argv[2];

	llvm::LLVMContext context;
	llvm::SMDiagnostic error;

	// Parse the LLVM IR file
	std::unique_ptr<llvm::Module> module = llvm::parseIRFile(inputFilename, error, context);
	if (!module) {
		std::cerr << "Error reading LLVM IR file: " << inputFilename << std::endl;
		error.print(argv[0], llvm::errs());
		return 1;
	}

	// Get the target triple
	std::string targetTriple = llvm::sys::getDefaultTargetTriple();
	module->setTargetTriple(targetTriple);

	std::string errorMessage;
	const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, errorMessage);
	if (!target) {
		std::cerr << "Error: " << errorMessage << std::endl;
		return 1;
	}

	// Create a TargetMachine
	llvm::TargetOptions opt;
	auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, llvm::Reloc::PIC_);
	if (!targetMachine) {
		std::cerr << "Could not allocate TargetMachine!" << std::endl;
		return 1;
	}

	module->setDataLayout(targetMachine->createDataLayout());

	// Open the output file
	std::error_code ec;
	llvm::raw_fd_ostream dest(outputFilename, ec, llvm::sys::fs::OF_None);
	if (ec) {
		std::cerr << "Could not open file: " << ec.message() << std::endl;
		return 1;
	}

	// Set up the pass manager
	llvm::legacy::PassManager pass;
	if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile)) {
		std::cerr << "TargetMachine can't emit a file of this type!" << std::endl;
		return 1;
	}

	pass.run(*module);
	dest.flush();

	std::cout << "Successfully compiled " << inputFilename << " to " << outputFilename << std::endl;

	return 0;
}
