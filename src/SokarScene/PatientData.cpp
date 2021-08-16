#include <gdcmStringFilter.h>
#include "SokarDicom/DataConventer.hpp"
#include "sokar/qt.h"
#include "PatientData.hpp"

using namespace SokarScene;

PatientData::PatientData(SokarDicom::DataConverter &dataConverter) : Indicator(dataConverter) {

	text = newText();
	addToGroup(text);

	initData();
}

void PatientData::reposition() {
	text->setPos(0, 0);
}

void PatientData::initData() {

	const static gdcm::Tag
			TagPatientName(0x0010, 0x0010),
			TagPatientID(0x0010, 0x0020),
			TagPatientBirthDate(0x0010, 0x0030),
			TagPatientSex(0x0010, 0x0040),
			TagPatientAge(0x0010, 0x1010),
			TagStudyDescription(0x0008, 0x1030),
			TagSeriesDescription(0x0008, 0x103E);

	QStringList lines;
	{
		QString name = "", sex = "";
		if (dataConverter.hasTagWithData(TagPatientName)) {
			name += dataConverter.toPersonName(TagPatientName);
		}

		if (dataConverter.hasTagWithData(TagPatientSex)) {
			sex = dataConverter.toString(TagPatientSex).trimmed();

			if (sex == "M")
				sex = QObject::tr(" ♂");
			else if (sex == "F")
				sex = QObject::tr(" ♀");
			else sex = "";
		}

		lines << QObject::tr("<b>%1</b>%2").arg(name, sex);
	}

	if (dataConverter.hasTagWithData(TagPatientID)) {
		lines << dataConverter.toString(TagPatientID);
	}

	{
		QStringList data;

		if (dataConverter.hasTagWithData(TagPatientBirthDate)) {
			auto date = dataConverter.toDate(TagPatientBirthDate);

			data << QObject::tr("born %1").arg(date.toString("yyyy-MM-dd"));
		}

		if (dataConverter.hasTagWithData(TagPatientAge)) {

			data << QObject::tr("%1").arg(
					dataConverter.toAgeString(TagPatientAge)
			);
		}

		lines << data.join(", ");
	}

	{
		if (dataConverter.hasTagWithData(TagStudyDescription)) {
			lines << dataConverter.toString(TagStudyDescription);
		}

		if (dataConverter.hasTagWithData(TagSeriesDescription)) {
			lines << dataConverter.toString(TagSeriesDescription);
		}
	}

	text->setHtml(wrapAsHtml(lines));
	text->adjustSize();
}

bool PatientData::isAlive() {
	return true;
}
