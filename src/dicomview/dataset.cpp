
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

	headerLabels << "Tag" << "VL" << "VR" << "Keyword" << "Value";
	standardModel.setHorizontalHeaderLabels(headerLabels);

	initTree();

	resizeColumnToContents(0);//Tag
	resizeColumnToContents(1);//VL
	resizeColumnToContents(2);//VR
	resizeColumnToContents(3);//Keyword
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


	for (auto elem : dataset.GetDES()) {

		QList<QStandardItem *> row;

		auto &tag = elem.GetTag();
		auto tagItem = new QStandardItem(tagIdToString(tag));

		auto &vl = elem.GetVL();
		auto vlItem = new QStandardItem(vl.IsUndefined() ? "" : QString::number(vl));
		vlItem->setTextAlignment(Qt::AlignRight);

		auto &vr = elem.GetVR();
		auto vrItem = new QStandardItem(gdcm::VR::GetVRStringFromFile(vr));

		auto keyword = pubdict.GetKeywordFromTag(tag);
		auto keywordItem = new QStandardItem(keyword == nullptr ? "" : QString::fromStdString(keyword));

		auto valueItem = new QStandardItem();
		if (vr == gdcm::VR::SQ) {
			auto sq = elem.GetValueAsSQ();

			for (int i = 1; i <= sq->GetNumberOfItems(); i++) {
				auto childItem = new QStandardItem(QString::number(i));
				childItem->setFlags(childItem->flags() & ~Qt::ItemIsEditable);

				auto &nestedDS = sq->GetItem(i).GetNestedDataSet();
				forEachDataSet(nestedDS, childItem);
				tagItem->appendRow(childItem);
			}

		} else {
			auto str = QString::fromStdString(stringFilter.ToString(elem));
			valueItem->setText(str.left(128));
		}

		row << tagItem << vlItem << vrItem << keywordItem << valueItem;

		for (auto &item:row)
			item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		parent->appendRow(row);
	}
}

DataSetViewer *DataSetViewer::openAsWindow(DicomSceneSet *sceneSet) {
	auto widget = new DataSetViewer(sceneSet);
	sceneSet->getDataSetViewers() << widget;

	widget->setAttribute(Qt::WA_DeleteOnClose);
	widget->show();

	return widget;
}


