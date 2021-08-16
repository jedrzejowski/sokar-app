#pragma once

#include <gdcmDataSet.h>
#include <gdcmStringFilter.h>

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "SokarScene/DicomScene.hpp"

namespace Sokar {

    class DataSetViewer : public QTreeView {
    Q_OBJECT

    private:
        SokarScene::DicomScene *dicomScene;

        QStandardItemModel standardModel;
        QStringList headerLabels;
        gdcm::StringFilter stringFilter;

    protected:
        void forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent = nullptr);

        void initTree();

    public:
        explicit DataSetViewer(SokarScene::DicomScene *dicomScene, QWidget *parent = nullptr);
        virtual ~DataSetViewer();

        static DataSetViewer *openAsWindow(SokarScene::DicomScene *scene);

        inline SokarScene::DicomScene *getDicomScene() {

            return dicomScene;
        }

        inline bool operator==(const DataSetViewer &b) const {

            return this == &b;
        }
    };

}
