#pragma once

#include <QtWidgets>
#include <gdcmDataSet.h>

namespace Sokar {
	class QDicomDataSet;
}

class Sokar::QDicomDataSet : public QTreeView {
Q_OBJECT

private:
	QStandardItemModel standardModel;

public:
	explicit QDicomDataSet(QWidget *parent);

	~QDicomDataSet();

	void setGdcmFile(const gdcm::File &file);
};


