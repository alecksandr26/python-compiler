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
#include "semantic.hpp"

pycgui::GUI::GUI(QWidget *parent)
	: QMainWindow(parent)
{
	// Create the central widget and set layout
	QWidget *centralWidget = new QWidget(this);
	QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

	// Create the main input area and submit button
	input_text_area = new QTextEdit(this); // Input
	input_text_area->setPlaceholderText("Enter your code here...");
	submit_button = new QPushButton("Submit", this);

	// Create output areas
	lexer_output_text_area = new QTextEdit(this);
	lexer_output_text_area->setPlaceholderText("Lexer output will appear here...");
	lexer_output_text_area->setReadOnly(true);

	parser_output_text_area = new QTextEdit(this);
	parser_output_text_area->setPlaceholderText("Parser output will appear here...");
	parser_output_text_area->setReadOnly(true);
	parser_output_text_area->setWordWrapMode(QTextOption::NoWrap);
	parser_output_text_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	semantic_output_text_area = new QTextEdit(this);
	semantic_output_text_area->setPlaceholderText("Semantic output will appear here...");
	semantic_output_text_area->setReadOnly(true);
	
	asm_output_text_area = new QTextEdit(this);
	asm_output_text_area->setPlaceholderText("asm output will appear here...");
	asm_output_text_area->setReadOnly(true);

	log_output_text_area = new QTextEdit(this);
	log_output_text_area->setPlaceholderText("Log output will appear here...");
	log_output_text_area->setReadOnly(true);

	// Create a grid layout for the three specified widgets
	QGridLayout *gridLayout = new QGridLayout();

	// Add the widgets to the grid layout
	gridLayout->addWidget(lexer_output_text_area, 0, 0); // First column, first row
	gridLayout->addWidget(semantic_output_text_area, 1, 0); // First column, second row
	gridLayout->addWidget(parser_output_text_area, 0, 1, 1, 1); // Second column, first row
	gridLayout->addWidget(asm_output_text_area, 1, 1); // Second column, second row

	// Add other widgets to the main layout
	mainLayout->addWidget(input_text_area);
	mainLayout->addWidget(submit_button);
	mainLayout->addLayout(gridLayout);
	mainLayout->addWidget(log_output_text_area);

	// Set the central widget to be our layout container
	setCentralWidget(centralWidget);

	// Set the window title
	setWindowTitle("Python Compiler");

	// Connect the submit_button click signal to the slot
	connect(submit_button, &QPushButton::clicked, this, &GUI::onButtonClick);
}

pycgui::GUI::~GUI(void)
{
}

void pycgui::GUI::onButtonClick(void)
{
	// Get the text from input_text_area
	QString inputText = input_text_area->toPlainText();

	// Convert QString to std::string
	std::string input_std_string = inputText.toStdString();

	// Use a std::istringstream to read from the string
	std::istringstream source(input_std_string);

	pyc::Lexer lexer(source);

	// Process for the tokenizer, parser, semantic and log
	std::stringstream output_stream_tokens;
	std::stringstream output_stream_parser;
	std::stringstream output_stream_semantic;
	std::stringstream output_stream_log;

	try {
		while (lexer.is_token_available()) {
			const pyc::Token &token = lexer.next_token();

			switch (token.get_type()) {
			case pyc::TokenType::IDENTIFIER:
			case pyc::TokenType::KEYWORD:
			case pyc::TokenType::STRING: {  // Case for strings
				const pyc::Word &word = static_cast<const pyc::Word&>(token);
				output_stream_tokens << word << std::endl;
				break;
			}
			case pyc::TokenType::NUMBER: {
				if (token.get_tag() == pyc::TagType::INTEGER) {
					const pyc::Integer &integer = static_cast<const pyc::Integer &>(token);
					output_stream_tokens << integer << std::endl;
				} else {
					const pyc::Real &real = static_cast<const pyc::Real &>(token);
					output_stream_tokens << real << std::endl;
				}
				break;
			}
			default:
				output_stream_tokens << token << std::endl;
				break;
			}
		}
		// Reset the stream for parsing
		source.clear();
		source.seekg(0);

		pyc::Parser parser(source);
	
		parser.parse();

		output_stream_parser << parser.get_ast();

		pyc::Semantic semantic(parser.get_ast());
		semantic.analyze();

		output_stream_semantic << semantic;
	} catch (const std::exception& e) {
		output_stream_log << "[ERROR-LOG]: " << e.what() << std::endl;
	}

	// Display the output 
	QString outputTokenText = QString::fromStdString(output_stream_tokens.str());
	QString outputParserText = QString::fromStdString(output_stream_parser.str());
	QString outputSemanticText = QString::fromStdString(output_stream_semantic.str());
	QString outputLogText = QString::fromStdString(output_stream_log.str());
	
	lexer_output_text_area->setPlainText(outputTokenText);
	parser_output_text_area->setPlainText(outputParserText);
	semantic_output_text_area->setPlainText(outputSemanticText);
	log_output_text_area->setPlainText(outputLogText);
}
