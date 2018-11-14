#pragma once

#include <gdcmTag.h>


namespace gdcm {
	static gdcm::Tag
			TagBitsAllocated(0x0028, 0x0100),
			TagHighBit(0x0028, 0x0102),
			TagBitsStored(0x0028, 0x0101),
			TagWindowCenter(0x0028, 0x1050),
			TagWindowWidth(0x0028, 0x1051),
			TagRescaleIntercept(0x0028, 0x1052),
			TagRescaleSlope(0x0028, 0x1053),
			TagRescaleType(0x0028, 0x1054);
}


