#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

// Include the lexer headers
#include "tag.hpp"
#include "word.hpp"
#include "integer.hpp"
#include "real.hpp"
#include "lexer.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onButtonClick(); // Slot for handling button click

private:
    QTextEdit *textArea1; // Input area
    QTextEdit *textArea2; // Output area
    QPushButton *button;
};

#endif // MAINWINDOW_H
