#pragma once

#include <QMainWindow>
#include <gdcmImage.h>

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

private slots:
	void selectFile();

private:
	void loadImage(const QString &path);

};

