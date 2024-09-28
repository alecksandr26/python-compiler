#include <QApplication>

#include "gui.hpp"

using namespace pycgui;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	GUI window;
	window.show();

	return app.exec();
}
