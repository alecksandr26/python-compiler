// gui.cpp
#include "gui.hpp"
#include "integer.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "real.hpp"
#include "word.hpp"
#include <QVBoxLayout>
#include <QWidget>
#include <sstream>
#include <iostream>
#include <algorithm> // For std::replace

namespace pycgui {

GUI::GUI(QWidget *parent) : QMainWindow(parent) {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    textArea1 = new QTextEdit(this);
    textArea2 = new QTextEdit(this);
    textArea3 = new QTextEdit(this);

    textArea1->setPlaceholderText("Enter your code here...");
    textArea2->setPlaceholderText("Lexer output will appear here...");
    textArea3->setPlaceholderText("Parser output will appear here...");

    textArea2->setReadOnly(true);
    textArea3->setReadOnly(true);

    button = new QPushButton("Submit", this);

    layout->addWidget(textArea1);
    layout->addWidget(button);
    layout->addWidget(textArea2);
    layout->addWidget(textArea3);

    setCentralWidget(centralWidget);
    setWindowTitle("Python Compiler");

    connect(button, &QPushButton::clicked, this, &GUI::onButtonClick);
}

GUI::~GUI() {}

void GUI::onButtonClick() {
    QString inputText = textArea1->toPlainText();
    std::string inputStdString = inputText.toUtf8().constData();

    // Normalize line endings
    size_t pos = 0;
    while ((pos = inputStdString.find("\r\n", pos)) != std::string::npos) {
        inputStdString.replace(pos, 2, "\n");
    }
    std::replace(inputStdString.begin(), inputStdString.end(), '\r', '\n');

    // Trim leading and trailing whitespace
    inputStdString.erase(inputStdString.begin(), std::find_if(inputStdString.begin(), inputStdString.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    inputStdString.erase(std::find_if(inputStdString.rbegin(), inputStdString.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), inputStdString.end());

    // Create the source stream
    std::istringstream source(inputStdString);

    // Lexer processing
    pyc::Lexer lexer(source);
    std::stringstream lexerOutputStream;

    try {
        while (lexer.is_token_available()) {
            auto token = lexer.next_token();
            lexerOutputStream << token->to_string() << std::endl;
        }
    } catch (const std::exception &e) {
        lexerOutputStream << "Error while processing tokens: " << e.what() << std::endl;
    }

    QString lexerOutputText = QString::fromStdString(lexerOutputStream.str());
    textArea2->setPlainText(lexerOutputText);

   // Parser processing
    std::stringstream parserOutputStream;

    try {
        // Reset the source stream for the parser
        std::istringstream sourceForParser(inputStdString);
        pyc::Lexer lexerForParser(sourceForParser);

        // Pass the stringstream to the parser
        pyc::Parser parser(lexerForParser, parserOutputStream);

        parser.parse();
        parserOutputStream << "<span style=\"color:green;\">Parsing successful.</span><br>" << std::endl;
    } catch (const std::exception &e) {
        parserOutputStream << "<span style=\"color:red;\">Error while parsing: " << e.what() << "</span><br>" << std::endl;
    }

    QString parserOutputText = QString::fromStdString(parserOutputStream.str());

    // Set the formatted HTML text directly
    textArea3->setHtml(parserOutputText);
}

} // namespace pycgui
