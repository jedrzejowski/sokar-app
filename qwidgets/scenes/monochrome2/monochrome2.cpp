#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmValue.h>
#include <gdcmTag.h>
#include <gdcmImageApplyLookupTable.h>
#include <gdcmStringFilter.h>

#include <QMenu>

#include "sokar/dicomtags.h"

#include "../../qdicomgraphics.h"

#include "monochrome2.h"

using namespace Sokar;


Monochrome2::Scene::Scene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams)
		: DicomScene(imageReader, sceneParams) {

//	qDebug() << gdcmImage.GetNumberOfDimensions();
	const unsigned int *dimension = gdcmImage.GetDimensions();

	dimX = dimension[0];
	dimY = dimension[1];

	originVectorBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&originVectorBuffer[0]);
	targetBuffer = new Pixel[dimX * dimY];

//	qDebug() << gdcmImage.GetBufferLength();

	readAttributes();

	reloadPixmap();
}


void Monochrome2::Scene::readAttributes() {

	bool ok;
	ushort us;

	// Tworzenie odpowiedzniego okienka
	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8:
		case gdcm::PixelFormat::UINT12:
		case gdcm::PixelFormat::UINT16:
		case gdcm::PixelFormat::UINT32:
		case gdcm::PixelFormat::UINT64:
			imgWindowInt = new WindowINT();
			break;


		case gdcm::PixelFormat::INT8:
		case gdcm::PixelFormat::INT12:
		case gdcm::PixelFormat::INT16:
		case gdcm::PixelFormat::INT32:
		case gdcm::PixelFormat::INT64:
			imgWindowInt = new WindowINT();
			imgWindowInt->setSigned(true);
			break;

		case gdcm::PixelFormat::FLOAT16:
		case gdcm::PixelFormat::FLOAT32:
		case gdcm::PixelFormat::FLOAT64:
			throw Sokar::ImageTypeNotSupportedException("gdcm::PixelFormat::FLOATXX");

		case gdcm::PixelFormat::UNKNOWN:
		case gdcm::PixelFormat::SINGLEBIT:
		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	//

	if (!gdcmDataSet.FindDataElement(gdcm::TagBitsStored))
		throw DicomTagMissing(gdcm::TagBitsStored);

	us = (ushort) *(gdcmDataSet.GetDataElement(gdcm::TagBitsStored).GetByteValue()->GetPointer());
	imgWindowInt->setLength((1 << us) - 1);

	//
	{
		if (gdcmDataSet.FindDataElement(gdcm::TagWindowCenter)) {
			gdcm::assertTagPresence(gdcmDataSet, gdcm::TagWindowWidth);

			auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
			auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');

			if (centers.size() != widths.size())
				throw DicomTagParseError(gdcm::TagWindowWidth, "Number of WindowCenter's and WindowWidth's not match");

			auto ds = centers[0].toDouble(&ok);

			if (!ok) throw DicomTagParseError(gdcm::TagWindowCenter);

			imgWindowInt->setCenter(ds);

			ds = widths[0].toDouble(&ok);

			if (!ok) throw DicomTagParseError(gdcm::TagWindowWidth);

			imgWindowInt->setWidth(ds);

			QObject::connect(imgWindowInt, SIGNAL(centerChanged()), this, SLOT(refreshText33()));
			QObject::connect(imgWindowInt, SIGNAL(widthChanged()), this, SLOT(refreshText33()));

			refreshText33();

		} else {
			//TODO VOILUT
			throw ImageTypeNotSupportedException();
		}
	}

	//

	if (gdcmDataSet.FindDataElement(gdcm::TagRescaleIntercept)) {
		gdcm::assertTagPresence(gdcmDataSet, gdcm::TagRescaleSlope);

		auto b = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagRescaleIntercept)).toDouble(&ok);
		if (!ok) throw DicomTagParseError(gdcm::TagRescaleIntercept);

		auto m = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagRescaleSlope)).toDouble(&ok);
		if (!ok) throw DicomTagParseError(gdcm::TagRescaleSlope);


		imgWindowInt->setRescaleIntercept(b);
		imgWindowInt->setRescaleSlope(m);
	}
}

bool Monochrome2::Scene::genQPixmap() {

	imgWindowInt->genLUT();

	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::INT8:
			genQPixmapOfType<int8_t>();
			break;
		case gdcm::PixelFormat::UINT8:
			genQPixmapOfType<uint8_t>();
			break;

		case gdcm::PixelFormat::INT16:
			genQPixmapOfType<int16_t>();
			break;
		case gdcm::PixelFormat::UINT16:
			genQPixmapOfType<uint16_t>();
			break;

		case gdcm::PixelFormat::INT32:
			genQPixmapOfType<int32_t>();
			break;
		case gdcm::PixelFormat::UINT32:
			genQPixmapOfType<uint32_t>();
			break;

		case gdcm::PixelFormat::INT64:
			genQPixmapOfType<int64_t>();
			break;
		case gdcm::PixelFormat::UINT64:
			genQPixmapOfType<uint64_t>();
			break;

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	pixmap.convertFromImage(QImage((uchar *) targetBuffer, dimX, dimY, QImage::Format_RGB888));

	return true;
}

template<typename T>
void Monochrome2::Scene::genQPixmapOfType() {

	Pixel *buffer = targetBuffer;

	auto *origin = (T *) &originVectorBuffer[0];

	for (uint i = 0; i < dimX * dimY; i++) {
		*buffer++ = imgWindowInt->getLUT(*origin);

		origin++;
	}
}

Monochrome2::Scene::~Scene() {

	delete targetBuffer;
}

QString Monochrome2::Scene::genText33() {
	QString str = DicomScene::genText33();

	//TODO zamienić __int128 na tekst

	str.append("<b>W</b> ").append(QString::number((qlonglong) imgWindowInt->getWidth()));
	str.append("<br>");
	str.append("<b>C</b> ").append(QString::number((qlonglong) imgWindowInt->getCenter()));

	return str;
}

void Monochrome2::Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {

	event->accept();
	isWindowEditing = true;

	DicomScene::mousePressEvent(event);
}

void Monochrome2::Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	event->accept();

	isWindowEditing = false;

	DicomScene::mouseReleaseEvent(event);
}

void Monochrome2::Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	if (isWindowEditing) {


		int dx = event->lastScreenPos().x() - event->screenPos().x(),
				dy = event->lastScreenPos().y() - event->screenPos().y();

		imgWindowInt->setWidth(imgWindowInt->getWidth() + dx);
		imgWindowInt->setCenter(imgWindowInt->getCenter() + dy);


		reloadPixmap();
	}

	DicomScene::mouseMoveEvent(event);
}


void Monochrome2::Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	auto item = this->itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());

	if (item == text33) {
		event->accept();

		imgWindowInt->selectWindowingIndicator(this->parentGraphics(), event->screenPos());
		reloadPixmap();
	} else {
		DicomScene::mouseDoubleClickEvent(event);
	}
}
