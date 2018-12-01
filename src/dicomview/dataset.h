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
		DicomSceneSet *dicomSceneSet;

		QStandardItemModel standardModel;
		QStringList headerLabels;
		gdcm::StringFilter stringFilter;

	protected:
		void forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent = nullptr);

		void initTree();

	public:
		explicit DataSetViewer(DicomSceneSet *dicomSceneSet, QWidget *parent = nullptr);

		static DataSetViewer *openAsWindow(DicomSceneSet *sceneSet);

		inline DicomSceneSet *getDicomSceneSet() {
			return dicomSceneSet;
		}

		inline bool operator==(const DataSetViewer &b) const {
			return this == &b;
		}
	};

}
