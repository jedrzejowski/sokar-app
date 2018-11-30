#include "main_win.h"
#include "ui_main_win.h"

#include <QtGlobal>
#include <QCoreApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollBar>

#include <gdcmImageReader.h>

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow) {
	ui->setupUi(this);
	this->setMouseTracking(true);
	ui->centralWidget->setMouseTracking(true);

	QObject::connect(ui->pushButton, SIGNAL(clicked()), this,
					 SLOT(selectFile()));
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

	ui->dicomView->addDicomImage(ir);
}