
#include <QtCore>
#include <QApplication>

#include "win/main_win.h"


int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

	MainWindow w;
	w.show();

	return app.exec();
}
