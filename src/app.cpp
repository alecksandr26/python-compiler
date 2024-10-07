// app.cpp
#include <QApplication>

#include "gui.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    pycgui::GUI window;
    window.show();

    return app.exec();
}
