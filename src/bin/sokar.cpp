#include <QtCore>
#include <QApplication>

#include <QGuiApplication>
#include <win/mainwindow.h>
#include <SokarUi/SegmentationWindow.hpp>

namespace Sokar {
    QSettings *qSettings;
}

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
