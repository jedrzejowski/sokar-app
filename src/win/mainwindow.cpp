#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore>
#include <QtWidgets>

#include <gdcmImageReader.h>

#include "sokar/settings.h"

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->splitter->setStretchFactor(0, 0);
	ui->splitter->setStretchFactor(1, 1);

	this->setMouseTracking(true);

	connect(ui->fileTree, SIGNAL(fileSelected(QString)), this, SLOT(loadImage(QString)));

	initMenuBar();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::selectFile() {

	QString fileName = QFileDialog::getOpenFileName(this,
													tr("Load DCM file"), "",
													tr("Dicom File (*.dcm);;All Files (*)"));
	if (fileName.isEmpty())
		return;

	loadImage(fileName);
}

void MainWindow::loadImage(const QString &path) {

	auto *ir = new gdcm::ImageReader;

	ir->SetFileName(path.toStdString().c_str());

	if (!ir->Read()) {
		QMessageBox::critical(this, "Error", "An error has occured !");
		delete ir;
		return;
	}

//	Sokar::Settings::bumpRecentOpen(path);

	ui->dicomTabs->addDicomFile(ir);
}

void MainWindow::initMenuBar() {

	connect(ui->actionExit, &QAction::triggered, [&] {
		QApplication::quit();
	});

	connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::selectFile);

	connect(ui->actionOpenRecent, &QAction::triggered, [&] {

	});
}
