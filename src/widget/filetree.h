#pragma once

#include <QtCore>
#include <QtWidgets>

namespace Sokar {

	class FileTree : public QTreeView {
	Q_OBJECT
		QFileSystemModel model;

	public:
		FileTree(QWidget *parent = nullptr);

	signals:
		void fileSelected(QString path);
	};

}