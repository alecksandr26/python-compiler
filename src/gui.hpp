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
		QTextEdit *textArea1; // Input area
		QTextEdit *textArea2; // Output area
		QPushButton *button;
	};
}


#endif
