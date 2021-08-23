//
// Created by adam on 09.06.2021.
//

#include "File.hpp"
#include <QtConcurrent/QtConcurrent>
#include <SokarException.hpp>

using namespace SokarDicom;


File::File(const QSharedPointer<gdcm::Reader> &gdcmReader)
        : gdcmReader(gdcmReader),
          gdcmFile(gdcmReader->GetFile()) {
}

Image::Image(const QSharedPointer<gdcm::ImageReader> &gdcmReader)
        : file(new File(gdcmReader)),
          gdcmReader(gdcmReader),
          gdcmImage(gdcmReader->GetImage()) {
}


QMap<QString, ImageWPtr> imageCache;
QMutex cacheMutex;

QFuture<ImagePtr> Image::read(const QString &path) {

    return QtConcurrent::run([path]() -> ImagePtr {
        cacheMutex.lock();
        auto wptr = imageCache[path];
        if (auto ptr = wptr.lock()) {
            cacheMutex.unlock();
            return ptr;
        }
        cacheMutex.unlock();

        auto reader = QSharedPointer<gdcm::ImageReader>::create();

        reader->SetFileName(path.toStdString().c_str());

        if (!reader->Read()) {
            throw sokarException("cannot open file:" + path);
        }

        auto ptr = ImagePtr(new Image(reader));

        cacheMutex.lock();
        imageCache[path] = ptr;
        cacheMutex.unlock();

        return ptr;
    });
}
