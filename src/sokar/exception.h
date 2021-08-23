#pragma once

#include <exception>
#include <gdcmTag.h>

namespace Sokar {

    class Exception : public std::exception {
    public:
        enum Type {
            WrongScope,
            ImageTypeNotSupported,
            DicomTagMissing,
            DicomTagParseError,
            IOException
        };

        virtual Type type() = 0;
    };

    class IOException : public Exception {
    public:
        const QString file;

        IOException(QString file) : file(file) {}

        Type type() { return Type::IOException; }
    };

    class WrongScopeException : public Exception {
    public:

        WrongScopeException(std::string file, int line) {}

        Type type() { return Type::WrongScope; }
    };

    class ImageTypeNotSupportedException : public Exception {
    public:
        ImageTypeNotSupportedException() {}

        ImageTypeNotSupportedException(std::string what) {}

        Type type() { return Type::ImageTypeNotSupported; }
    };

    class DicomTagMissing : public Exception {
    private:
        std::string msg;
        const gdcm::Tag &tag;
    public:
        DicomTagMissing(gdcm::Tag &tag) : tag(tag) {}

        DicomTagMissing(const gdcm::Tag &tag) : tag(tag) {}

        DicomTagMissing(gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

        DicomTagMissing(const gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

        Type type() { return Type::DicomTagMissing; }
    };

    class DicomTagParseError : public Exception {
    private:
        std::string msg;
        const gdcm::Tag &tag;
    public:
        DicomTagParseError(gdcm::Tag &tag) : tag(tag) {}

        DicomTagParseError(const gdcm::Tag &tag) : tag(tag) {}

        DicomTagParseError(gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

        DicomTagParseError(const gdcm::Tag &tag, std::string what) : tag(tag), msg(what) {}

        Type type() { return Type::DicomTagParseError; }

    };
}