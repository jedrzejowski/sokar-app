#pragma once

#include <gdcmDataSet.h>
#include <gdcmStringFilter.h>

#include <QtWidgets>

namespace Sokar {

	class QDicomDataSet : public QTreeView {
	Q_OBJECT

	private:
		QStandardItemModel standardModel;
		QStringList headerLabels;
		gdcm::StringFilter stringFilter;

	public:
		explicit QDicomDataSet(QWidget *parent);

		~QDicomDataSet();

		void setGdcmFile(const gdcm::File &file);

		void forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent = nullptr);
	};

}
