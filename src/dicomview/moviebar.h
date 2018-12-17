#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"


namespace Ui {
	class MovieBar;
}

namespace Sokar {

	class MovieBar : public QWidget {
	Q_OBJECT
	protected:
		Ui::MovieBar *ui;
	public:
		explicit MovieBar(QWidget *parent = nullptr);
		~MovieBar();

	};
}
