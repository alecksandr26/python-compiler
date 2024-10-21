#include <iostream>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <sstream>    // For std::stringstream
#include <string>
#include <stdexcept>  // For runtime_error and custom exception
#include <sstream>    // For std::istringstream

#include "gui.hpp"

// Include the lexer headers
#include "tag.hpp"
#include "word.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "lexer.hpp"
#include "parser.hpp"

pycgui::GUI::GUI(QWidget *parent)
	: QMainWindow(parent)
{
	// Create the central widget and set layout
	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(centralWidget);
	
	// Create text areas
	textArea1 = new QTextEdit(this);
	textArea2 = new QTextEdit(this);
	textArea3 = new QTextEdit(this);

	// Optionally set placeholder text
	textArea1->setPlaceholderText("Enter your code here...");
	textArea2->setPlaceholderText("Lexer output will appear here...");
	textArea3->setPlaceholderText("Parser output will appear here...");
	
	textArea2->setReadOnly(true); // Make the output area read-only
	textArea3->setReadOnly(true);

	// Create button
	button = new QPushButton("Submit", this);

	// Add widgets to the layout
	layout->addWidget(textArea1);
	layout->addWidget(button);
	layout->addWidget(textArea2);
	layout->addWidget(textArea3);
	
	// Set the central widget to be our layout container
	setCentralWidget(centralWidget);

	// Set the window title
	setWindowTitle("Python Compiler");

	// Connect the button click signal to the slot
	connect(button, &QPushButton::clicked, this, &GUI::onButtonClick);
}

pycgui::GUI::~GUI(void)
{
}

void pycgui::GUI::onButtonClick(void)
{
	// Get the text from textArea1 to an in stream
	QString inputText = textArea1->toPlainText();
	std::string input_std_string = inputText.toStdString();
	std::istringstream source(input_std_string);
	std::istream source_istream(source.rdbuf());
	
	// Run the output of the lexer
	
	pyc::Lexer lexer(source_istream);
	std::stringstream lexer_output_stream;

	try {
		while (lexer.is_token_available()) {
			const pyc::Token &token = lexer.next_token();

			switch (token.get_type()) {
			case pyc::TokenType::IDENTIFIER:
			case pyc::TokenType::KEYWORD:
			case pyc::TokenType::STRING: {  // Case for strings
				const pyc::Word &word = static_cast<const pyc::Word&>(token);
				lexer_output_stream << word << std::endl;
				break;
			}
			case pyc::TokenType::NUMBER: {
				if (token.get_tag() == pyc::TagType::INTEGER) {
					const pyc::Integer &integer = static_cast<const pyc::Integer &>(token);
					lexer_output_stream << integer << std::endl;
				} else {
					const pyc::Real &real = static_cast<const pyc::Real &>(token);
					lexer_output_stream << real << std::endl;
				}
				break;
			}
			default:
				lexer_output_stream << token << std::endl;
				break;
			}
		}
	} catch (const std::exception& e) {
		lexer_output_stream << e.what() << std::endl;
	}

	// Display the lexer output
	QString outputText = QString::fromStdString(lexer_output_stream.str());
	textArea2->setPlainText(outputText);

	std::istringstream source2(input_std_string);
	std::istream source_istream2(source2.rdbuf());

	// Run the output for the parser
	pyc::Parser parser(source_istream2);
	std::stringstream parser_output_stream;

	try {
		parser.parse();
		parser_output_stream << parser.get_ast() << std::endl;
	} catch (const std::exception &e) {
		parser_output_stream << e.what() << std::endl;
	}


	// Display the parser output
	outputText = QString::fromStdString(parser_output_stream.str());
	textArea3->setPlainText(outputText);
}
