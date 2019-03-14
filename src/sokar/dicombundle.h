#pragma once

#include <gdcmImageReader.h>
#include "dataconventer.h"

namespace Sokar {

	class DicomBundle {
	private:
		explicit DicomBundle(const gdcm::Reader *reader);
		explicit DicomBundle(const gdcm::ImageReader *imageReader);
	public:

		static DicomBundle *create(const QString &path);

		enum BundleType {
			Image,
			UnSupported
		};

		BundleType type;

		const gdcm::Reader *gdcmReader;
		const gdcm::File &gdcmFile;
		const gdcm::DataSet &gdcmDataSet;
		DataConverter dataConverter;

		//Image:
		const gdcm::ImageReader *imageReader = nullptr;
		const gdcm::Image *gdcmImage = nullptr;

	};

	typedef QVector<const DicomBundle *> gdcmBundleVec;
}
