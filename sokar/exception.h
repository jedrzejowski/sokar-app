#pragma once

#include <exception>
#include <gdcmTag.h>

namespace Sokar {
	class ImageTypeNotSupportedException : public std::exception {

	};

	class DicomTagMissing : public std::exception {
	public:
		DicomTagMissing() {}

		DicomTagMissing(gdcm::Tag tag) {}
	};
}