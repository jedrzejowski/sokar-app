//
// Created by adam on 09.06.2021.
//

#pragma once

#include <QtCore>

namespace SokarDicom {
	class File;

	using FileWPtr = QWeakPointer<const File>;
	using FilePtr = QSharedPointer<const File>;

	class Image;

	using ImageWPtr = QWeakPointer<const Image>;
	using ImagePtr = QSharedPointer<const Image>;

	class DataConverter;
}