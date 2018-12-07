#include <gdcmStringFilter.h>
#include "patientdata.h"

using namespace Sokar;

PatientDataIndicator::PatientDataIndicator() {

	text = newText();
	addToGroup(text);

}

void PatientDataIndicator::reposition() {
	text->setPos(scene()->width() - text->document()->size().width(), 0);
}

void PatientDataIndicator::loadData(const gdcm::File &file) {

	const static gdcm::Tag
			TagPatientName(0x0010, 0x0010),
			TagPatientID(0x0010, 0x0020),
			TagPatientBirthDate(0x0010, 0x0030),
			TagPatientSex(0x0010, 0x0040),
			TagPatientAge(0x0010, 0x1010);

	auto &dataset = file.GetDataSet();
	auto stringFilter = gdcm::StringFilter();
	stringFilter.SetFile(file);

	QStringList lines;
	QString temp;

	{
		QString line = "";
		if (dataset.FindDataElement(TagPatientName)) {
			temp += QString::fromStdString(
					stringFilter.ToString(dataset.GetDataElement(TagPatientName))
			).replace('^', ' ');

			line += QObject::tr("%1").arg(temp);
		}

		if (dataset.FindDataElement(TagPatientSex)) {
			temp = QString::fromStdString(
					stringFilter.ToString(dataset.GetDataElement(TagPatientSex))
			).trimmed();

			if (temp == "M")
				line += QObject::tr(" ♂");

			if (temp == "F")
				line += QObject::tr(" ♀");
		}

		lines << QObject::tr("<h3>%1</h3>").arg(line);
	}

	if (dataset.FindDataElement(TagPatientID)) {
		temp = QString::fromStdString(stringFilter.ToString(dataset.GetDataElement(TagPatientID)));
		lines << temp;
	}

	{
		QString line = "";

		if (dataset.FindDataElement(TagPatientBirthDate)) {
			/**
			 * A string of characters of the format YYYYMMDD;
			 * where YYYY shall contain year, MM shall contain the month, and DD shall contain the day,
			 * interpreted as a date of the Gregorian calendar system.
			 */
			temp = QString::fromStdString(stringFilter.ToString(dataset.GetDataElement(TagPatientBirthDate)));

			auto date = QDate::fromString(temp, "yyyyMMdd");
			line += QObject::tr("born %1").arg(date.toString(Qt::DefaultLocaleLongDate));

		}

		if (dataset.FindDataElement(TagPatientAge)) {
			/**
			 * A string of characters with one of the following formats -- nnnD, nnnW, nnnM, nnnY;
			 * where nnn shall contain the number of days for D, weeks for W, months for M, or years for Y.
			 * Example: "018M" would represent an age of 18 months.
			 */
			temp = QString::fromStdString(stringFilter.ToString(dataset.GetDataElement(TagPatientAge)));

			auto unitChar = temp[3];
			QString unitName;

			if (unitChar == "Y")
				unitName = QObject::tr("years");
			else if (unitChar == "M")
				unitName = QObject::tr("months");
			else if (unitChar == "W")
				unitName = QObject::tr("weeks");
			else if (unitChar == "D")
				unitName = QObject::tr("days");
			else unitName = QObject::tr("?");

			line += QObject::tr(", %1 %2").arg(QString::number(temp.left(3).toInt()), unitName);

		}
		lines << line;
	}
	text->setHtml(lines.join(""));
}
