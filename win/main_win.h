#pragma once

#include <QMainWindow>
#include <gdcmImage.h>
#include <qwidgets/scenes/dicomscene.h>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow();

private:
	Ui::MainWindow *ui = nullptr;
	Sokar::DicomScene *dicomScene = nullptr;

private slots:
	void selectFile();

private:
	void loadImage(const QString &path);

};

