#pragma once

#include <gdcmFile.h>
#include <gdcmStringFilter.h>

#include <QtCore>

namespace Sokar {

	class DataConverter : public QObject {
	Q_OBJECT
		const gdcm::File &file;
		const gdcm::DataSet &dataset;
		gdcm::StringFilter stringFilter;
	public:
		explicit DataConverter(const gdcm::File &file);

		inline QString toString(const gdcm::Tag &tag) {
			return QString::fromStdString(stringFilter.ToString(tag));
		}

		/**
		 * http://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
		 */

		QString toAgeString(const gdcm::Tag &tag);
		QDate toDate(const gdcm::Tag &tag);
		QVector<qreal> toDecimalString(const gdcm::Tag &tag);
		qint16 toShort(const gdcm::Tag &tag);
		quint16 toUShort(const gdcm::Tag &tag);

		//region Aliases

		inline QString toAS(const gdcm::Tag &tag) { return toAgeString(tag); }

		gdcm::Tag toAT(const gdcm::Tag &tag);
		QString toCS(const gdcm::Tag &tag);

		inline QDate toDA(const gdcm::Tag &tag) { return toDate(tag); }

		inline QVector<qreal> toDS(const gdcm::Tag &tag) { return toDecimalString(tag); }

		QDateTime toDT(const gdcm::Tag &tag);

		inline short toSS(const gdcm::Tag &tag) { return toShort(tag); }

		QString toST(const gdcm::Tag &tag);
		QTime toTM(const gdcm::Tag &tag);

		inline quint16 toUS(const gdcm::Tag &tag) { return toUShort(tag); }

		inline QString toUT(const gdcm::Tag &tag) { return toString(tag); }

		//endregion
	};
}