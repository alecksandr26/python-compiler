# Python Compiler

This project is dedicated to building a fast and efficient Python compiler, designed to enhance the developer experience and deliver better performance in Python code execution. By leveraging advanced lexical analysis and tokenization techniques, this compiler provides a streamlined parsing mechanism while adhering to Python's syntax and style conventions, including support for lists, tuples, and variable naming in compliance with PEP 8.

## Key Features

- **Faster Compilation:** The compiler is optimized to parse Python code quickly, offering faster compilation times compared to standard interpreters.
- **PEP 8 Compliance:** The compiler follows Python’s PEP 8 guidelines for variable naming, allowing for identifiers with underscores and uppercase constants, while rejecting variable names that start with digits.
- **Comprehensive Lexical Analysis:** The lexer is designed to handle a variety of Python constructs, including:
  - Identifiers (e.g., variables and function names)
  - Keywords (e.g., `if`, `else`, `for`)
  - Operators (e.g., `+`, `-`, `*`, `/`, `==`, `!=`)
  - Delimiters (e.g., `[]`, `()`, `,`)
  - String literals (supporting both single-line and multi-line strings)
  - Numeric literals (integers and floating-point numbers)
  - Comments (both inline and multi-line)
- **Support for Lists and Tuples:** The compiler supports Python data structures such as lists and tuples, recognizing square brackets (`[]`) and parentheses (`()`) as delimiters and handling commas (`,`) as separators within these structures.
- **Enhanced Tokenization:** Through detailed tokenization, the compiler efficiently parses Python code into individual tokens such as keywords, identifiers, numbers, strings, and operators, making it easier to analyze and compile the code.
  
## Getting Started

To use the compiler: