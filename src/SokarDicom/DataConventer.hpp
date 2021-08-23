#pragma once

#include "SokarDicom.hpp"
#include "SokarGdcm.hpp"

#include <QtCore>

namespace SokarDicom {

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

        inline QString toString(const gdcm::Tag &tag) const {

            return QString::fromStdString(stringFilter.ToString(tag));
        }

        inline QStringList toStringList(const gdcm::Tag &tag) const {

            return toString(tag).split('\\');
        }

        inline bool hasTagWithData(const gdcm::Tag &tag) const {

            if (!dataset->FindDataElement(tag)) return false;

            return dataset->GetDataElement(tag).GetVL() > 0;
        }

        /**
         * http://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
         */

        gdcm::Tag toAttributeTag(const gdcm::Tag &tag) const;
        [[nodiscard]]
        QString toAgeString(const gdcm::Tag &tag) const;
        [[nodiscard]]
        QDate toDate(const gdcm::Tag &tag) const;
        [[nodiscard]]
        QVector<qreal> toDecimalString(const gdcm::Tag &tag) const;
        [[nodiscard]]
        qint32 toIntegerString(const gdcm::Tag &tag) const;
        [[nodiscard]]
        QString toPersonName(const gdcm::Tag &tag) const;
        [[nodiscard]]
        qint16 toShort(const gdcm::Tag &tag) const;
        [[nodiscard]]
        quint16 toUShort(const gdcm::Tag &tag) const;

        //region Aliases

        [[nodiscard]]
        inline QString toAS(const gdcm::Tag &tag) const { return toAgeString(tag); }

        [[nodiscard]]
        inline gdcm::Tag toAT(const gdcm::Tag &tag) const { return toAttributeTag(tag); }

        [[nodiscard]]
        inline QDate toDA(const gdcm::Tag &tag) const { return toDate(tag); }

        [[nodiscard]]
        inline QVector<qreal> toDS(const gdcm::Tag &tag) const { return toDecimalString(tag); }

        [[nodiscard]]
        inline qint32 toIS(const gdcm::Tag &tag) const { return toIntegerString(tag); }

        [[nodiscard]]
        QString toPN(const gdcm::Tag &tag) const { return toPersonName(tag); }

        [[nodiscard]]
        inline qint16 toSS(const gdcm::Tag &tag) const { return toShort(tag); }

        [[nodiscard]]
        inline quint16 toUS(const gdcm::Tag &tag) const { return toUShort(tag); }

        [[nodiscard]]
        inline QString toUT(const gdcm::Tag &tag) const { return toString(tag); }

        //endregion
    };
}