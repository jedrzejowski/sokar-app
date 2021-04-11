#include <QtCore>
#include <QApplication>
#include <sokar/settings.h>

#include <QGuiApplication>
#include <3d/Renderer.h>
#include <win/mainwindow.h>

QSettings *Sokar::qSettings;

int main(int argc, char *argv[]) {

	QCoreApplication::setOrganizationName("Gauganian");
	QCoreApplication::setOrganizationDomain("sokar.gauganian.com");
	QCoreApplication::setApplicationName("Sokar");

	QApplication app(argc, argv);

	Sokar::qSettings = new QSettings();

	Sokar::MainWindow w;
	w.show();

	return app.exec();
}
