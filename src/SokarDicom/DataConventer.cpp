#include <SokarMacro.hpp>
#include <SokarException.hpp>
#include "DataConventer.hpp"
#include "sokar/exception.h"

using namespace SokarDicom;

DataConverter::DataConverter() {

    sokarTrace();
}

DataConverter::DataConverter(const gdcm::File &file) {

    setFile(file);
}

DataConverter::~DataConverter() {

    sokarTrace();
}


void DataConverter::setFile(const gdcm::File &file) {

    this->file = &file;
    this->dataset = &(file.GetDataSet());
    stringFilter.SetFile(file);
}


QString DataConverter::toAgeString(const gdcm::Tag &tag) const {
    /**
     * A string of characters with one of the following formats -- nnnD, nnnW, nnnM, nnnY;
     * where nnn shall contain the number of days for D, weeks for W, months for M, or years for Y.
     * Example: "018M" would represent an age of 18 months.
     */

    auto temp = toString(tag);

    if (temp.length() != 4) throw sokarException("dicom tag parse rrror");

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
        throw sokarException("dicom tag parse rrror");

    // Usunięcie zer
    temp = QString::number(temp.leftRef(3).toInt());

    return tr("%1 %2").arg(temp, unitName);
}

QDate DataConverter::toDate(const gdcm::Tag &tag) const {
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
            throw sokarException("dicom tag parse rrror");
    }
}

QVector<qreal> DataConverter::toDecimalString(const gdcm::Tag &tag) const {
    /**
     * A string of characters representing either a fixed point number or a floating point number.
     * A fixed point number shall contain only the characters 0-9 with an optional leading "+" or "-"
     * and an optional "." to mark the decimal point. A floating point number shall be conveyed as
     * defined in ANSI X3.9, with an "E" or "e" to indicate the start of the exponent.
     * Decimal Strings may be padded with leading or trailing spaces. Embedded spaces are not allowed.
     */

    auto vec = QVector<qreal>();
    bool ok;

    for (auto &str : toStringList(tag)) {
        vec << str.toDouble(&ok);

        if (!ok) throw sokarException("dicom tag parse rrror");
    }

    return vec;
}

qint16 DataConverter::toShort(const gdcm::Tag &tag) const {

    return *((qint16 *) dataset->GetDataElement(tag).GetByteValue()->GetPointer());
}

quint16 DataConverter::toUShort(const gdcm::Tag &tag) const {

    return *((quint16 *) dataset->GetDataElement(tag).GetByteValue()->GetPointer());
}

gdcm::Tag DataConverter::toAttributeTag(const gdcm::Tag &tag) const {

    quint16 *group, *element;

    auto &elem = dataset->GetDataElement(tag);

    group = (quint16 *) elem.GetByteValue()->GetPointer();
    element = group + 1;

    return gdcm::Tag(*group, *element);
}

QString DataConverter::toPersonName(const gdcm::Tag &tag) const {
    /*
     *       0           1            2        3      4
     * [family name^given names^middle name^prefix^suffixes]
     */

    auto str = toString(tag).trimmed();

    auto list = str.split('^');

    QStringList full;

    if (list.length() >= 4)
        full << list[3].trimmed();

    if (list.length() >= 1)
        full << list[0].trimmed();

    if (list.length() >= 3)
        full << list[2].trimmed();

    if (list.length() >= 2)
        full << list[1].trimmed();

    if (list.length() >= 5)
        full << list[4].trimmed();

    return full.join(' ');
}

qint32 DataConverter::toIntegerString(const gdcm::Tag &tag) const {

    return toString(tag).toInt();
}
