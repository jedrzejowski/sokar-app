#include "hospitaldata.h"

using namespace Sokar;

HospitalDataIndicator::HospitalDataIndicator(DataConverter &dataConverter) : SceneIndicator(dataConverter) {

	text = newText();
	addToGroup(text);

	initData();
}

void HospitalDataIndicator::reposition() {
	text->setPos(scene()->width() - text->document()->size().width(), 0);
}

void HospitalDataIndicator::initData() {

	const static gdcm::Tag
			TagModality(0x0008, 0x0060),
			TagManufacturer(0x0008, 0x0070),
			TagInstitutionalDepartmentName(0x0008, 0x1040),
			TagOperatorsName(0x0008, 0x1070),
			TagManufacturerModelName(0x0008, 0x1090),
			TagReferringPhysicianName(0x0008, 0x0090);

	QString htmlTags = " style='white-space: nowrap;' align='right' ";

	QStringList lines;
	QString temp;

	if (dataConverter.hasTagWithData(TagInstitutionalDepartmentName)) {
		temp = dataConverter.toString(TagInstitutionalDepartmentName).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	if (dataConverter.hasTagWithData(TagManufacturer)) {
		QString manufacture = dataConverter.toString(TagManufacturer).trimmed();
		QString model = dataConverter.toString(TagManufacturerModelName).trimmed();

		lines << QObject::tr("<div %1>%2 %3</div>").arg(htmlTags, manufacture, model).trimmed();
	}

	if (dataConverter.hasTagWithData(TagReferringPhysicianName)) {
		temp = dataConverter.toString(TagReferringPhysicianName).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	if (dataConverter.hasTagWithData(TagOperatorsName)) {
		temp = dataConverter.toString(TagOperatorsName).trimmed();
		lines << QObject::tr("<div %1>%2</div>").arg(htmlTags, temp);
	}

	text->setHtml(lines.join(""));
	text->adjustSize();
}
