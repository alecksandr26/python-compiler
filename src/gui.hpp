// gui.hpp
#ifndef GUI_HPP
#define GUI_HPP

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

namespace pycgui {
    class GUI : public QMainWindow {
        Q_OBJECT

    public:
        explicit GUI(QWidget *parent = nullptr);
        ~GUI();

    private slots:
        void onButtonClick();

    private:
        QTextEdit *textArea1; // Input code
        QTextEdit *textArea2; // Lexer output
        QTextEdit *textArea3; // Parser output
        QPushButton *button;
    };
}

#endif // GUI_HPP
