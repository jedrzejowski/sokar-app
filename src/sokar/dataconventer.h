#pragma once

#include <gdcmFile.h>
#include <gdcmStringFilter.h>

#include <QtCore>

namespace Sokar {

	class DataConventer : public QObject {
	Q_OBJECT
		const gdcm::File &file;
		gdcm::StringFilter stringFilter;
	public:
		explicit DataConventer(const gdcm::File &file);

		inline QString toString(const gdcm::Tag &tag) {
			return QString::fromStdString(stringFilter.ToString(tag));
		}

		/**
		 * http://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
		 */

		QString asAgeString(const gdcm::Tag &tag);
		QDate asDate(const gdcm::Tag &tag);
		QVector<qreal> asDecimalString(const gdcm::Tag &tag);

		//region Aliases

		inline QString asAS(const gdcm::Tag &tag) { return asAgeString(tag); }

		gdcm::Tag asAT(const gdcm::Tag &tag);
		QString asCS(const gdcm::Tag &tag);

		inline QDate asDA(const gdcm::Tag &tag) { return asDate(tag); }

		inline QVector<qreal> asDS(const gdcm::Tag &tag) { return asDecimalString(tag); }

		QDateTime asDT(const gdcm::Tag &tag);
		short asSS(const gdcm::Tag &tag);
		QString asST(const gdcm::Tag &tag);
		QTime asTM(const gdcm::Tag &tag);
		ushort asUS(const gdcm::Tag &tag);
		QString asUT(const gdcm::Tag &tag);
		//endregion
	};
}