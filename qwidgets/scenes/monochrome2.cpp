#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmValue.h>
#include <gdcmTag.h>
#include <gdcmImageApplyLookupTable.h>
#include <sokar/dicomtags.h>


#include "monochrome2.h"

using namespace Sokar;


Monochrome2DicomScene::Monochrome2DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams)
		: DicomScene(imageReader, sceneParams) {

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

	std::stringstream strm;
	ushort us;


	if (!gdcmDataSet.FindDataElement(gdcm::TagBitsStored))
		throw DicomTagMissing(gdcm::TagBitsStored);

	us = (ushort) *(gdcmDataSet.GetDataElement(gdcm::TagBitsStored).GetByteValue()->GetPointer());
	sceneParams->imgWindow.setMax((1 << us) - 1);

	//

	if (!gdcmDataSet.FindDataElement(gdcm::TagWindowWidth))
		throw DicomTagMissing(gdcm::TagWindowWidth);

	gdcmDataSet.GetDataElement(gdcm::TagWindowWidth).GetValue().Print(strm);
	sceneParams->imgWindow.setWidth(std::stoi(strm.str()));

	strm.str("");

	//

	if (!gdcmDataSet.FindDataElement(gdcm::TagWindowCenter))
		throw DicomTagMissing(gdcm::TagWindowCenter);

	gdcmDataSet.GetDataElement(gdcm::TagWindowCenter).GetValue().Print(strm);
	sceneParams->imgWindow.setCenter(std::stoi(strm.str()));


}

bool Monochrome2DicomScene::genQPixmap() {

	uchar *buffer = targetBuffer;

	auto *origin8 = (uchar *) &originVectorBuffer[0];
	auto *origin16 = (ushort *) &originVectorBuffer[0];

	uchar *lut;
	ushort lutLength;
	sceneParams->imgWindow.genLUT();
	sceneParams->imgWindow.getLUT(lut, lutLength);

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
