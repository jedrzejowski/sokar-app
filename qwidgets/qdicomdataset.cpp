
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <gdcmStringFilter.h>


#include "qdicomdataset.h"

using namespace Sokar;


QDicomDataSet::QDicomDataSet(QWidget *parent) : QTreeView(parent) {

	setModel(&standardModel);
}

QDicomDataSet::~QDicomDataSet() {

}

void QDicomDataSet::setGdcmFile(const gdcm::File &file) {

	gdcm::StringFilter sf;
	sf.SetFile(file);

	static auto &dicts = gdcm::Global::GetInstance().GetDicts();
	static auto &pubdict = dicts.GetPublicDict();

	auto rootItem = standardModel.invisibleRootItem();

	auto &dataset = file.GetDataSet();

	for (auto it = dataset.Begin(); it != dataset.End(); ++it) {
		auto &elem = *it;
		auto &tag = elem.GetTag();

		QList<QStandardItem *> list;

		std::pair<std::string, std::string> pss = sf.ToStringPair(tag);

		auto item = new QStandardItem(QString::fromStdString(pss.first));
		list << item;
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		item = new QStandardItem(QString::fromStdString(pss.second));
		list << item;
		item->setFlags(item->flags() & ~Qt::ItemIsEditable);

		rootItem->appendRow(list);
	}
}

