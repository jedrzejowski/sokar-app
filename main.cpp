#include "win/main_win.h"
#include <QApplication>

#include "gdcmImageReader.h"
#include <QImage>
#include <QImageWriter>

#include <iostream>


int main(int argc, char *argv[]) {

	QApplication app(argc, argv);

	MainWindow w;
	w.show();

	return app.exec();
}
