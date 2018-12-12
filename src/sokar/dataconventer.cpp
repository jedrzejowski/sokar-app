#include "dataconventer.h"
#include "exception.h"

using namespace Sokar;

static char StringSplitter = '\\';

DataConventer::DataConventer(const gdcm::File &file) :
		file(file) {

	stringFilter.SetFile(file);

}

QString DataConventer::asAgeString(const gdcm::Tag &tag) {
	/**
	 * A string of characters with one of the following formats -- nnnD, nnnW, nnnM, nnnY;
	 * where nnn shall contain the number of days for D, weeks for W, months for M, or years for Y.
	 * Example: "018M" would represent an age of 18 months.
	 */

	auto temp = toString(tag);

	if (temp.length() != 4) throw DicomTagParseError(tag);

	auto unitChar = temp[3];
	QString unitName;

	if (unitChar == "Y")
		unitName = tr("years");
	else if (unitChar == "M")
		unitName = tr("months");
	else if (unitChar == "W")
		unitName = tr("weeks");
	else if (unitChar == "D")
		unitName = tr("days");
	else
		throw DicomTagParseError(tag);

	// Usunięcie zer
	temp = QString::number(temp.left(3).toInt());

	return tr("%1 %2").arg(temp, unitName);
}

QDate DataConventer::asDate(const gdcm::Tag &tag) {
	/**
	 * A string of characters of the format YYYYMMDD;
	 * where YYYY shall contain year,
	 * MM shall contain the month,
	 * and DD shall contain the day,
	 * interpreted as a date of the Gregorian calendar system.
	 *
	 * Example:
	 * 	"19930822" would represent August 22, 1993.
	 *
	 * 	Note
	 * 	The ACR-NEMA Standard 300 (predecessor to DICOM) supported a string of characters of the format YYYY.MM.DD
	 * 	for this VR. Use of this format is not compliant.
	 * 	See also DT VR in this table.
	 */

	auto str = toString(tag);

	switch (str.length()) {
		case 8:
			return QDate::fromString(str, "yyyyMMdd");

		case 10:
			return QDate::fromString(str, "yyyy.MM.dd");

		default:
			throw DicomTagParseError(tag);
	}
}

QVector<qreal> DataConventer::asDecimalString(const gdcm::Tag &tag) {
	auto vec = QVector<qreal>();
	bool ok;
	qreal db;

	for (auto &str : toString(tag).split(StringSplitter)) {
		db = str.toDouble(&ok);
		if (ok) vec << db;
		else throw DicomTagParseError(tag);
	}

	return vec;
}
