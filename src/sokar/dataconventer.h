#pragma once

#include <gdcmFile.h>
#include <gdcmStringFilter.h>

#include <QtCore>

namespace Sokar {

	class DataConverter : public QObject {
	Q_OBJECT
		const gdcm::File *file;
		const gdcm::DataSet *dataset;
		gdcm::StringFilter stringFilter;
	public:
		DataConverter();
		explicit DataConverter(const gdcm::File &file);
		~DataConverter() override;

		void setFile(const gdcm::File &file);

		inline QString toString(const gdcm::Tag &tag) {
			return QString::fromStdString(stringFilter.ToString(tag));
		}

		inline QStringList toStringList(const gdcm::Tag &tag) {
			return toString(tag).split('\\');
		}

		inline bool hasTagWithData(const gdcm::Tag &tag) {
			if (!dataset->FindDataElement(tag)) return false;

			return dataset->GetDataElement(tag).GetVL() > 0;
		}

		/**
		 * http://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
		 */

		gdcm::Tag toAttributeTag(const gdcm::Tag &tag);
		QString toAgeString(const gdcm::Tag &tag);
		QDate toDate(const gdcm::Tag &tag);
		QVector<qreal> toDecimalString(const gdcm::Tag &tag);
		qint32 toIntegerString(const gdcm::Tag &tag);
		QString toPersonName(const gdcm::Tag &tag);
		qint16 toShort(const gdcm::Tag &tag);
		quint16 toUShort(const gdcm::Tag &tag);

		//region Aliases

		inline QString toAS(const gdcm::Tag &tag) { return toAgeString(tag); }

		inline gdcm::Tag toAT(const gdcm::Tag &tag) { return toAttributeTag(tag); }

		inline QDate toDA(const gdcm::Tag &tag) { return toDate(tag); }

		inline QVector<qreal> toDS(const gdcm::Tag &tag) { return toDecimalString(tag); }

		inline qint32 toIS(const gdcm::Tag &tag) { return toIntegerString(tag); }

		QString toPN(const gdcm::Tag &tag) { return toPersonName(tag); }

		inline short toSS(const gdcm::Tag &tag) { return toShort(tag); }

		inline quint16 toUS(const gdcm::Tag &tag) { return toUShort(tag); }

		inline QString toUT(const gdcm::Tag &tag) { return toString(tag); }

		//endregion
	};
}