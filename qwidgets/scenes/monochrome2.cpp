#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmValue.h>
#include <gdcmImageApplyLookupTable.h>


#include "monochrome2.h"

using namespace Sokar;


Monochrome2DicomScene::Monochrome2DicomScene(const gdcm::File &gdcmFile,
											 const gdcm::Image &gdcmImage)
		: DicomScene(gdcmFile, gdcmImage) {

	const unsigned int *dimension = gdcmImage.GetDimensions();

	dimX = dimension[0];
	dimY = dimension[1];

	originVectorBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&originVectorBuffer[0]);
	targetBuffer = new unsigned char[dimX * dimY * 3];

	readAttributes();

	reloadPixmap();
}


void Monochrome2DicomScene::readAttributes() {

	static bool tagsFound = false;
	static auto &dicts = gdcm::Global::GetInstance().GetDicts();
	static auto &pubdict = dicts.GetPublicDict();
	static gdcm::Tag bitsStoredTag, windowWidthTag, windowCenterTag;

	if (!tagsFound) {
		pubdict.GetDictEntryByKeyword("BitsStored", bitsStoredTag);
		pubdict.GetDictEntryByKeyword("WindowWidth", windowWidthTag);
		pubdict.GetDictEntryByKeyword("WindowCenter", windowCenterTag);
	}

	bitsStored = (ushort) *(gdcmDataSet.GetDataElement(bitsStoredTag)
			.GetByteValue()->GetPointer());

	imgWindow.setMax((1 << bitsStored) - 1);

	std::stringstream strm;

	gdcmDataSet.GetDataElement(windowWidthTag).GetValue().Print(strm);
	imgWindow.setWidth(std::stoi(strm.str()));

	strm.str("");

	gdcmDataSet.GetDataElement(windowCenterTag).GetValue().Print(strm);
	imgWindow.setCenter(std::stoi(strm.str()));

}

bool Monochrome2DicomScene::genQPixmap() {

	uchar *buffer = targetBuffer;

	auto *origin8 = (uchar *) &originVectorBuffer[0];
	auto *origin16 = (ushort *) &originVectorBuffer[0];

	uchar* lut;
	ushort lutLength;
	imgWindow.genLUT(lut, lutLength);

	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8:

			for (uint i = 0; i < dimX * dimY; i++) {
				*buffer++ = lut[*origin8];
				*buffer++ = lut[*origin8];
				*buffer++ = lut[*origin8];

				origin8++;
			}
			break;

		case gdcm::PixelFormat::UINT16:

			for (uint i = 0; i < dimX * dimY; i++) {

				*buffer++ = lut[*origin16];
				*buffer++ = lut[*origin16];
				*buffer++ = lut[*origin16];
				origin16++;
			}
			break;

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	pixmap.convertFromImage(QImage(targetBuffer, dimX, dimY, QImage::Format_RGB888));

	return true;
}

Monochrome2DicomScene::~Monochrome2DicomScene() {
	delete targetBuffer;
}
