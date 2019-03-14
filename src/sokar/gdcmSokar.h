#pragma once

#include <QtCore>

#include <gdcmDataSet.h>
#include <gdcmTag.h>
#include <gdcmImageHelper.h>
#include <gdcmFile.h>
#include <gdcmImageHelper.h>
#include "exception.h"

namespace gdcm {
	inline void assertTagPresence(const DataSet &dataSet, const Tag &tag) {

		if (!dataSet.FindDataElement(tag))
			throw Sokar::DicomTagMissing(tag);
	}

	inline ushort getPixelByteSize(const gdcm::File &file) {

		switch (ImageHelper::GetPixelFormatValue(file)) {

			case gdcm::PixelFormat::INT8:
			case gdcm::PixelFormat::UINT8:
				return 1;

			case gdcm::PixelFormat::INT12:
			case gdcm::PixelFormat::INT16:
			case gdcm::PixelFormat::UINT12:
			case gdcm::PixelFormat::UINT16:
			case gdcm::PixelFormat::FLOAT16:
				return 2;

			case gdcm::PixelFormat::INT32:
			case gdcm::PixelFormat::UINT32:
			case gdcm::PixelFormat::FLOAT32:
				return 4;

			case gdcm::PixelFormat::INT64:
			case gdcm::PixelFormat::UINT64:
			case gdcm::PixelFormat::FLOAT64:
				return 8;

			case gdcm::PixelFormat::SINGLEBIT:
			case gdcm::PixelFormat::UNKNOWN:
			default:
				return 0;
		}
	}
}