#include <gdcmStringFilter.h>
#include "hospitaldata.h"

using namespace Sokar;

HospitalDataIndicator::HospitalDataIndicator() {

	text = newText();
	addToGroup(text);

}

void HospitalDataIndicator::reposition() {
	text->setPos(scene()->width() - text->document()->size().width(), 0);
}

void HospitalDataIndicator::loadData(const gdcm::File &file) {

	const static gdcm::Tag
			TagModality(0x0008, 0x0060),
			TagManufacturer(0x0008, 0x0070),
			TagInstitutionalDepartmentName(0x0008, 0x1040),
			TagOperatorsName(0x0008, 0x1070),
			TagManufacturerModelName(0x0008, 0x1090),
			TagReferringPhysicianName(0x0008, 0x0090);

	auto &dataset = file.GetDataSet();
	auto stringFilter = gdcm::StringFilter();
	stringFilter.SetFile(file);

	QString htmlTags = " style='white-space: nowrap;' align='right' ";

	QStringList lines;
	QString temp;

	if (dataset.FindDataElement(TagInstitutionalDepartmentName)) {
		temp = QString::fromStdString(stringFilter.ToString(TagInstitutionalDepartmentName)).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	if (dataset.FindDataElement(TagManufacturer)) {
		QString manufacture = QString::fromStdString(stringFilter.ToString(TagManufacturer)).trimmed();
		QString model = QString::fromStdString(stringFilter.ToString(TagManufacturerModelName)).trimmed();

		lines << QObject::tr("<div %1>%2 %3</div>").arg(htmlTags, manufacture, model).trimmed();
	}

	if (dataset.FindDataElement(TagReferringPhysicianName)) {
		temp = QString::fromStdString(stringFilter.ToString(TagReferringPhysicianName)).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	if (dataset.FindDataElement(TagOperatorsName)) {
		temp = QString::fromStdString(stringFilter.ToString(TagOperatorsName)).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	text->setHtml(lines.join(""));
	text->adjustSize();
}
