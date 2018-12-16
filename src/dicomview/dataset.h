#pragma once

#include <gdcmDataSet.h>
#include <gdcmStringFilter.h>

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "scenes/dicomscene.h"

namespace Sokar {

	class DataSetViewer : public QTreeView {
	Q_OBJECT

	private:
		DicomScene *dicomScene;

		QStandardItemModel standardModel;
		QStringList headerLabels;
		gdcm::StringFilter stringFilter;

	protected:
		void forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent = nullptr);

		void initTree();

	public:
		explicit DataSetViewer(DicomScene *dicomScene, QWidget *parent = nullptr);
		virtual ~DataSetViewer();

		static DataSetViewer *openAsWindow(DicomScene *scene);

		inline DicomScene *getDicomScene() {
			return dicomScene;
		}

		inline bool operator==(const DataSetViewer &b) const {
			return this == &b;
		}
	};

}
