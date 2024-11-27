#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>


// Just for the checkers
#ifndef slots
#define slots
#endif

// Define the space of the gui
namespace pycgui {
	class GUI : public QMainWindow {
		Q_OBJECT
	
	public:
		GUI(QWidget *parent = nullptr);
		~GUI(void);
		     
	private slots:
		void onButtonClick(void); // Slot for handling button click
	
	private:
		QTextEdit *input_text_area; // Input area
		QTextEdit *lexer_output_text_area; // Lexer Output area
		QTextEdit *parser_output_text_area; // Parser Output area
		QTextEdit *semantic_output_text_area; // Semantic Output area
		QTextEdit *asm_output_text_area; // llvm Output area
		QTextEdit *log_output_text_area; // General log error output text area
		
		QPushButton *submit_button;
	};
}


#endif
