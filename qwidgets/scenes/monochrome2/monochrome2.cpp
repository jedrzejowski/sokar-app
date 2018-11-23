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
#include "windowing/windowintdynamic.h"
#include "windowing/windowintstatic.h"

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
	WindowInt *imgWindowInt;

	if (!gdcmDataSet.FindDataElement(gdcm::TagBitsStored))
		throw DicomTagMissing(gdcm::TagBitsStored);

	auto bitsStored = (ushort) *(gdcmDataSet.GetDataElement(gdcm::TagBitsStored).GetByteValue()->GetPointer());

	{ // Tworzenie obiekty okienka

		bool isDynamic = dimX * dimY < ((1 << bitsStored) - 1), isSigned;

		// Tworzenie odpowiedzniego okienka
		switch (gdcmImage.GetPixelFormat()) {
			case gdcm::PixelFormat::UINT8:
			case gdcm::PixelFormat::UINT12:
			case gdcm::PixelFormat::UINT16:
			case gdcm::PixelFormat::UINT32:
			case gdcm::PixelFormat::UINT64:

				isSigned = false;
				break;


			case gdcm::PixelFormat::INT8:
			case gdcm::PixelFormat::INT12:
			case gdcm::PixelFormat::INT16:
			case gdcm::PixelFormat::INT32:
			case gdcm::PixelFormat::INT64:

				isSigned = true;
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


		if (isDynamic) imgWindow = new WindowIntDynamic();
		else imgWindow = new WindowIntStatic();

		addIndicator(imgWindow);


		imgWindowInt = (WindowInt *) imgWindow;
		imgWindowInt->setSigned(isSigned);
		imgWindowInt->setMaxValue((uint) ((1 << bitsStored) - 1));
	}

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

	imgWindow->genLUT();

	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::INT8:
			genQPixmapOfType<qint8>();
			break;
		case gdcm::PixelFormat::UINT8:
			genQPixmapOfType<quint8>();
			break;

		case gdcm::PixelFormat::INT16:
			genQPixmapOfType<qint16>();
			break;
		case gdcm::PixelFormat::UINT16:
			genQPixmapOfType<quint16>();
			break;

		case gdcm::PixelFormat::INT32:
			genQPixmapOfType<qint32>();
			break;
		case gdcm::PixelFormat::UINT32:
			genQPixmapOfType<qint32>();
			break;

		case gdcm::PixelFormat::INT64:
			genQPixmapOfType<qint64>();
			break;
		case gdcm::PixelFormat::UINT64:
			genQPixmapOfType<qint64>();
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

	switch (imgWindow->type()) {
		case Window::IntDynamic: {

			auto windowPtr = (WindowIntDynamic *) imgWindow;

			for (uint64_t i = 0; i < dimX * dimY; i++, origin++)
				*buffer++ = windowPtr->getLUT(*origin);
		}
			break;

		case Window::IntStatic: {

			auto windowPtr = (WindowIntStatic *) imgWindow;

			for (uint64_t i = 0; i < dimX * dimY; i++, origin++)
				*buffer++ = windowPtr->getLUT(*origin);
		}
			break;

		default:
			throw WrongScopeException(__FILE__, __LINE__);
	}
}


Monochrome2::Scene::~Scene() {

	delete targetBuffer;
	delete imgWindow;
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

		imgWindow->mvHorizontal(dx);
		imgWindow->mvVertical(dy);


		reloadPixmap();
	}

	DicomScene::mouseMoveEvent(event);
}


void Monochrome2::Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	auto item = this->itemAt(event->scenePos().x(), event->scenePos().y(), QTransform());

//	if (item == text33) {
//		event->accept();
//
//		imgWindowInt->selectWindowingIndicator(this->parentGraphics(), event->screenPos());
//		reloadPixmap();
//	} else {
//		DicomScene::mouseDoubleClickEvent(event);
//	}
}