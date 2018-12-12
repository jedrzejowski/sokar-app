#include <gdcmStringFilter.h>
#include "sokar/dataconventer.h"
#include "sokar/qt.h"
#include "patientdata.h"

using namespace Sokar;

PatientDataIndicator::PatientDataIndicator() {

	text = newText();
	addToGroup(text);

}

void PatientDataIndicator::reposition() {
	text->setPos(0, 0);
}

void PatientDataIndicator::loadData(const gdcm::File &file) {

	const static gdcm::Tag
			TagPatientName(0x0010, 0x0010),
			TagPatientID(0x0010, 0x0020),
			TagPatientBirthDate(0x0010, 0x0030),
			TagPatientSex(0x0010, 0x0040),
			TagPatientAge(0x0010, 0x1010),
			TagStudyDescription(0x0008, 0x1030),
			TagSeriesDescription(0x0008, 0x103E);

	auto &dataset = file.GetDataSet();
	auto dataConv = DataConverter(file);

	QStringList lines;
	QString temp;

	{
		QString name = "", sex = "";
		if (dataset.FindDataElement(TagPatientName)) {
			temp += dataConv.toString(TagPatientName).replace('^', ' ');

			name += QObject::tr("%1").arg(temp);
		}

		if (dataset.FindDataElement(TagPatientSex)) {
			temp = dataConv.toString(TagPatientSex).trimmed();

			if (temp == "M")
				sex += QObject::tr(" ♂");

			if (temp == "F")
				sex += QObject::tr(" ♀");
		}

		lines << QObject::tr("<div style='font-size:x-large;'><b>%1</b>%2</div>").arg(name, sex);
	}

	if (dataset.FindDataElement(TagPatientID)) {
		temp = dataConv.toString(TagPatientID);
		lines << QObject::tr("<div>%1</div>").arg(temp);
	}

	{
		QString line = "";

		if (dataset.FindDataElement(TagPatientBirthDate)) {
			auto date = dataConv.toDate(TagPatientBirthDate);

			line += QObject::tr("born %1").arg(date.toString("yyyy-MM-dd"));

		}

		if (dataset.FindDataElement(TagPatientAge)) {

			temp = dataConv.toAgeString(TagPatientAge);

			line += QObject::tr(", %1").arg(temp);

		}
		lines << QObject::tr("<div>%1</div>").arg(line);
	}

	{
		if (dataset.FindDataElement(TagStudyDescription)) {
			temp = dataConv.toString(TagStudyDescription);

			lines << QObject::tr("<div>%1</div>").arg(temp);
		}

		if (dataset.FindDataElement(TagSeriesDescription)) {
			temp = dataConv.toString(TagSeriesDescription);

			lines << QObject::tr("<div>%1</div>").arg(temp);
		}
	}

	text->setHtml(lines.join(""));
	text->adjustSize();
}
