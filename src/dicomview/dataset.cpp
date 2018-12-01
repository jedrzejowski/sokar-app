
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmTag.h>
#include <gdcmAttribute.h>
#include <gdcmPrivateTag.h>


#include "dataset.h"

using namespace Sokar;


DataSetViewer::DataSetViewer(DicomSceneSet *dicomSceneSet, QWidget *parent)
		: QTreeView(parent),
		  dicomSceneSet(dicomSceneSet) {

	setModel(&standardModel);

	headerLabels << "Tag" << "VL" << "Keyword" << "VR" << "Value";
	standardModel.setHorizontalHeaderLabels(headerLabels);

	initTree();
}

QString tagIdToString(const gdcm::Tag &tag) {
	auto grp = QString::number(tag.GetGroup(), 16).toUpper();
	auto elem = QString::number(tag.GetElement(), 16).toUpper();

	grp = QString(4 - grp.length(), '0') + grp;
	elem = QString(4 - elem.length(), '0') + elem;

	return grp + "," + elem;
}

void DataSetViewer::initTree() {

	stringFilter.SetFile(dicomSceneSet->getGdcmFile());

	auto rootItem = standardModel.invisibleRootItem();

	auto &dataset = dicomSceneSet->getGdcmFile().GetDataSet();

	forEachDataSet(dataset, rootItem);
}


void DataSetViewer::forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent) {


	static auto &dicts = gdcm::Global::GetInstance().GetDicts();
	static auto &pubdict = dicts.GetPublicDict();

	QStandardItem *item;

	for (auto elem : dataset.GetDES()) {
		auto &tag = elem.GetTag();

		QList<QStandardItem *> list;

		item = new QStandardItem(tagIdToString(tag));
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		list << item;

		auto keyword = pubdict.GetKeywordFromTag(tag);
		item = new QStandardItem(keyword == nullptr ? "" : QString::fromStdString(keyword));
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);
		list << item;

		item = new QStandardItem();
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		try {

			switch (elem.GetVR()) {

				case gdcm::VR::SQ: {
					auto sq = elem.GetValueAsSQ();

					for (int i = 1; i <= sq->GetNumberOfItems(); i++) {

						auto nestedDS = sq->GetItem(i).GetNestedDataSet();

						forEachDataSet(nestedDS, item);

					}
				}
					break;

				default:
					item->setText(QString::fromStdString(stringFilter.ToString(elem)));
			}
		} catch (std::exception &) {
			item->setText("Error");
		}

		list << item;


		parent->appendRow(list);
	}
}

DataSetViewer *DataSetViewer::openAsWindow(DicomSceneSet *sceneSet) {
	auto widget = new DataSetViewer(sceneSet);
	sceneSet->getDataSetViewers() << widget;

	widget->setAttribute(Qt::WA_DeleteOnClose);
	widget->show();

	return widget;
}


