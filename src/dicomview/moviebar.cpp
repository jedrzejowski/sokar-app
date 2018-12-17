#include "moviebar.h"

#include "ui_moviebar.h"

using namespace Sokar;

MovieBar::MovieBar(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::MovieBar){
	ui->setupUi(this);
}

MovieBar::~MovieBar() {
	delete ui;
}
