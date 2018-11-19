
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmTag.h>
#include <gdcmAttribute.h>
#include <gdcmPrivateTag.h>


#include "qdicomdataset.h"

using namespace Sokar;


QDicomDataSet::QDicomDataSet(QWidget *parent) : QTreeView(parent) {

	setModel(&standardModel);

	headerLabels << "Tag" << "Keyword" << "Value";
	standardModel.setHorizontalHeaderLabels(headerLabels);
}

QDicomDataSet::~QDicomDataSet() {

}

QString tagIdToString(const gdcm::Tag &tag) {
	auto grp = QString::number(tag.GetGroup(), 16).toUpper();
	auto elem = QString::number(tag.GetElement(), 16).toUpper();

	grp = QString(4 - grp.length(), '0') + grp;
	elem = QString(4 - elem.length(), '0') + elem;

	return grp + "," + elem;
}

void QDicomDataSet::setGdcmFile(const gdcm::File &file) {


	stringFilter.SetFile(file);

	auto rootItem = standardModel.invisibleRootItem();

	auto &dataset = file.GetDataSet();

	forEachDataSet(dataset, rootItem);

}

void QDicomDataSet::forEachDataSet(const gdcm::DataSet &dataset, QStandardItem *parent) {


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
					std::cout << elem << std::endl;
					item->setText(QString::fromStdString(stringFilter.ToString(elem)));
			}
		} catch (std::exception) {
			item->setText("Error");
		}

		list << item;


//		std::cout << elem << std::endl;

		parent->appendRow(list);
	}
}

