#pragma once

#include <QtCore>

#include <gdcmDataSet.h>
#include <gdcmTag.h>
#include <gdcmImageHelper.h>
#include <gdcmFile.h>
#include <gdcmImageHelper.h>
#include "exception.h"

namespace gdcm {

	const static gdcm::Tag
			TagImageType(0x0008, 0x0008),
			TagImageOrientationPatient(0x0020, 0x0037),
			TagPlanarConfiguration(0x0028, 0x0006),
			TagPixelSpacing(0x0028, 0x0030),
			TagBitsAllocated(0x0028, 0x0100),
			TagHighBit(0x0028, 0x0102),
			TagBitsStored(0x0028, 0x0101),
			TagPixelPaddingValue(0x0028, 0x0120),
			PixelIntensityRelationship(0x0028, 0x1040),
			TagWindowCenter(0x0028, 0x1050),
			TagWindowWidth(0x0028, 0x1051),
			TagWindowCenterWidthExplanation(0x0028, 0x1055),
			TagRescaleIntercept(0x0028, 0x1052),
			TagRescaleSlope(0x0028, 0x1053),
			TagRescaleType(0x0028, 0x1054);

	static char StringSplitter = '\\';

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