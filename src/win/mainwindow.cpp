#include <QtCore>
#include <QtWidgets>

#include <gdcmImageReader.h>

#include "sokar/settings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about.h"

using namespace Sokar;

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow) {
	ui->setupUi(this);
	ui->splitter->setStretchFactor(0, 0);
	ui->splitter->setStretchFactor(1, 1);

	this->setMouseTracking(true);

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

	ui->dicomTabs->addDicomFile(fileName);
}

void MainWindow::initMenuBar() {

	//region File

	connect(ui->menuFile, &QMenu::aboutToShow, this, [&]() {
		auto disable = false;

		auto dicomView = ui->dicomTabs->currentDicomView();

		if (dicomView == nullptr) {
			disable = true;
		} else {
			auto scene = dicomView->currentDicomScene();

			if (scene == nullptr)
				disable = true;
		}

		ui->menuExport->setDisabled(disable);
		ui->actionSaveAnonymously->setDisabled(disable);
	});

	connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::selectFile);

	for (auto &path : Settings::recentOpen()) {

		auto action = new QAction(path, this);

		connect(action, &QAction::triggered, this, [path, this] {
			ui->dicomTabs->addDicomFile(path);
		});

		ui->menuOpenRecent->addAction(action);
	}

	//region Export as

	auto exportAs = [this](QString title, QString filter, QString ext) {

		auto scene = ui->dicomTabs->currentDicomView()->currentDicomScene();

		auto fileName = QFileDialog::getSaveFileName(this, title, "", filter);

		if (fileName.right(ext.length()) != ext) fileName += ext;

		if (!scene->saveToFile(fileName)) {

			QMessageBox::critical(this, "Error", "An error has occurred while saving image.\nImage was not saved.");
		}
	};

	connect(ui->actionJPEG, &QAction::triggered, this, [this, exportAs]() {
		exportAs(tr("Save as JPEG"), tr("JPEG File (*.jpg)"), ".jpg");
	});

	connect(ui->actionPNG, &QAction::triggered, this, [this, exportAs]() {
		exportAs(tr("Save as PNG"), tr("PNG File (*.png)"), ".png");
	});

	connect(ui->actionGIF, &QAction::triggered, this, [this, exportAs]() {
		exportAs(tr("Save as GIF"), tr("GIF File (*.gif)"), ".gif");
	});

	connect(ui->actionBMP, &QAction::triggered, this, [this, exportAs]() {
		exportAs(tr("Save as BMP"), tr("BMP File (*.bmp)"), ".bmp");
	});

	//endregion

	connect(ui->actionExit, &QAction::triggered, this, &QApplication::quit);

	//endregion

	//region Help

	connect(ui->actionAboutQt, &QAction::triggered, this, [this](){
		QMessageBox::aboutQt(this, tr("About Qt - Sokar"));
	});

	connect(ui->actionAboutGDCM, &QAction::triggered, this, [this](){
		About::GDCM(this);
	});

	connect(ui->actionAboutSokar, &QAction::triggered, this, [this](){
		About::Sokar(this);
	});

	//endregion

}
