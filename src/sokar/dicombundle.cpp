#include "dicombundle.h"
#include "exception.h"
#include "settings.h"

Sokar::DicomBundle::DicomBundle(const gdcm::Reader *reader) :
		gdcmReader(reader),
		gdcmFile(reader->GetFile()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		dataConverter(reader->GetFile()) {

	type = UnSupported;
}

Sokar::DicomBundle::DicomBundle(const gdcm::ImageReader *imageReader) :
		DicomBundle((const gdcm::Reader *) imageReader) {

	type = Image;

	this->imageReader = imageReader;
	gdcmImage = &imageReader->GetImage();
}

Sokar::DicomBundle *Sokar::DicomBundle::create(const QString &path) {
	auto *cpath = path.toStdString().c_str();

	{ // Image
		auto *imgReader = new gdcm::ImageReader();

		imgReader->SetFileName(cpath);

		if (imgReader->Read())
			return new DicomBundle(imgReader);

		delete imgReader;
	}

	{ // Uns
		auto *reader = new gdcm::Reader();
		reader->SetFileName(cpath);

		if (!reader->Read())
			return new DicomBundle(reader);

		delete reader;
	}

	throw IOException(path);
}
