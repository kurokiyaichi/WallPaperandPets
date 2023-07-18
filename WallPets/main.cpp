#include <qapplication.h>
#include <qwidget.h>
#include "widget.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	Widget wid;
	wid.show();

	return app.exec();
}