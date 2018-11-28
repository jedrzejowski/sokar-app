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
#include <sokar/speedtest.h>
#include <gdcmImageHelper.h>

#include "sokar/gdcmSokar.h"

#include "qwidgets/dicomgraphics.h"

#include "monochrome2.h"
#include "windowing/windowintdynamic.h"
#include "windowing/windowintstatic.h"

using namespace Sokar;


Monochrome2::Scene::Scene(SceneParams &sceneParams) : DicomScene(sceneParams) {

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
		imgWindowInt->setMaxValue((uint) (1 << bitsStored));

		imgWindowInt->setWidth(imgWindowInt->getMaxValue());
		imgWindowInt->setCenter(imgWindowInt->getMaxValue() / 2);
	}

	//
	{
		if (gdcmDataSet.FindDataElement(gdcm::TagWindowCenter)) {
			gdcm::assertTagPresence(gdcmDataSet, gdcm::TagWindowWidth);

			auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
			auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');

			if (centers.size() != widths.size())
				throw DicomTagParseError(gdcm::TagWindowWidth, "Number of WindowCenter's and WindowWidth's not match");

			auto center = centers[0].toDouble(&ok);
			if (!ok) throw DicomTagParseError(gdcm::TagWindowCenter);

			auto width = widths[0].toDouble(&ok);
			if (!ok) throw DicomTagParseError(gdcm::TagWindowWidth);


			switch (imgWindow->type()) {
				case Window::IntDynamic:
				case Window::IntStatic:

					imgWindowInt->setCenter(static_cast<__int128_t>(center));
					imgWindowInt->setWidth(static_cast<__int128_t>(width));

					break;

				default:
					throw WrongScopeException(__FILE__, __LINE__);
			}
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

	{
		if (gdcmImage.GetNumberOfOverlays() > 0)
			qDebug() << "Obraz z Overlayem (sprawdzić o co kaman)";
	}
}

bool Monochrome2::Scene::generatePixmap() {

	if (!imgWindow->genLUT()) return false;

//	qDebug("Monochrome2::Scene::generatePixmap");

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

	pixmap.convertFromImage(QImage((uchar *) &targetBuffer[0], dimX, dimY, QImage::Format_RGB888));

	return true;
}


template<typename T>
void Monochrome2::Scene::genQPixmapOfType() {

	auto *buffer = &targetBuffer[0];
	auto *origin = (T *) &originBuffer[0];

	switch (imgWindow->type()) {
		case Window::IntDynamic: {

			auto windowPtr = (WindowIntDynamic *) imgWindow;

			for (uint64_t i = 0; i < dimX * dimY; i++, origin++) {
				*buffer++ = windowPtr->getLUT(*origin);
			}
		}
			break;

		case Window::IntStatic: {

			auto windowPtr = (WindowIntStatic *) imgWindow;

			for (uint64_t i = 0; i < dimX * dimY; i++, origin++) {
				*buffer++ = windowPtr->getLUT(*origin);
			}
		}
			break;

		default:
			throw WrongScopeException(__FILE__, __LINE__);
	}
}


Monochrome2::Scene::~Scene() {

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