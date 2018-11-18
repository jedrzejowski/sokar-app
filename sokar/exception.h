#pragma once

#include <exception>
#include <gdcmTag.h>

namespace Sokar {
	class ImageTypeNotSupportedException : public std::exception {

	};

	class DicomTagMissing : public std::exception {
	private:
		std::string msg;
		const gdcm::Tag &tag;
	public:
		DicomTagMissing(gdcm::Tag &tag) : tag(tag) {}

		DicomTagMissing(const gdcm::Tag &tag) : tag(tag) {}

		DicomTagMissing(gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

		DicomTagMissing(const gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}
	};

	class DicomTagParseError : public std::exception {
	private:
		std::string msg;
		const gdcm::Tag &tag;
	public:
		DicomTagParseError(gdcm::Tag &tag) : tag(tag) {}

		DicomTagParseError(const gdcm::Tag &tag) : tag(tag) {}

		DicomTagParseError(gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

		DicomTagParseError(const gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

	};
}