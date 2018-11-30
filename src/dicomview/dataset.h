#pragma once

#include <gdcmDataSet.h>
#include <gdcmStringFilter.h>

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"
#include "scenes/dicomsceneset.h"

namespace Sokar {

	class DataSetViewer : public QTreeView {
	Q_OBJECT

	private:
		QStandardItemModel standardModel;
		QStringList headerLabels;

		DicomSceneSet &sceneSet;
		gdcm::StringFilter stringFilter;

	protected:
		void forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent = nullptr);

		void initTree();

	public:
		explicit DataSetViewer(DicomSceneSet &sceneSet, QWidget *parent = nullptr);
		~DataSetViewer() override;

		static DataSetViewer* openAsWindow(DicomSceneSet &sceneSet);

		inline bool operator==(const DataSetViewer &b) const {
			return this == &b;
		}
	};

}
