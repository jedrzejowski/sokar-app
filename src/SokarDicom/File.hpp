//
// Created by adam on 09.06.2021.
//

#pragma once

#include "SokarDicom.hpp"
#include "SokarGdcm.hpp"
#include <QObject>
#include <QFuture>

namespace SokarDicom {

    class File : public QEnableSharedFromThis<File> {
        friend Image;

        QSharedPointer<gdcm::Reader> gdcmReader;
        gdcm::File &gdcmFile;

        explicit File(const QSharedPointer<gdcm::Reader> &gdcmReader);

    public:

    };

    class Image : public QEnableSharedFromThis<Image> {
        FilePtr file;
        QSharedPointer<gdcm::ImageReader> gdcmReader;
        gdcm::Image &gdcmImage;

        explicit Image(const QSharedPointer<gdcm::ImageReader> &gdcmReader);

    public:

        static QFuture<ImagePtr> read(const QString &path);
    };
}

