#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <gdcmDataElement.h>
#include <gdcmValue.h>
#include <gdcmTag.h>
#include <gdcmImageApplyLookupTable.h>
#include <gdcmStringFilter.h>
#include <gdcmImageHelper.h>

#include "sokar/speedtest.h"
#include "sokar/gdcmSokar.h"

#include "../../dicomview.h"
#include "../../graphics.h"

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


	{ // Tworzenie obiekty okienka

		auto bitsStored = (ushort) *(gdcmDataSet.GetDataElement(gdcm::TagBitsStored).GetByteValue()->GetPointer());
		bool isDynamic = imgDimX * imgDimY < ((1 << bitsStored) - 1), isSigned = false;

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

		auto maxValue = quint64(1) << bitsStored;
		if (isSigned) maxValue /= 2;
		imgWindowInt->setMaxValue(maxValue);

		imgWindowInt->setWidth(imgWindowInt->getMaxValue());
		imgWindowInt->setCenter(imgWindowInt->getMaxValue() / 2);

		connect(imgWindow, &Window::forceRefreshSignal, this, &DicomScene::reloadPixmap);
	}

	//
	{
		if (gdcmDataSet.FindDataElement(gdcm::TagWindowCenter)) {
			gdcm::assertTagPresence(gdcmDataSet, gdcm::TagWindowWidth);

			auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
			auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');
			QStringList names;

			if (gdcmDataSet.FindDataElement(gdcm::TagWindowCenterWidthExplanation))
				names = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenterWidthExplanation))
						.split('\\');

			if (centers.size() != widths.size())
				throw DicomTagParseError(gdcm::TagWindowWidth, "Number of WindowCenter's and WindowWidth's not match");

			for (int i = 0; i < centers.size(); i++) {

				imgWindowInt->pushDefaultValues(
						static_cast<__int128_t>(centers[i].toDouble()),
						static_cast<__int128_t>(widths[i].toDouble()),
						names.isEmpty() ? "" : names[i]
				);
			}

			imgWindowInt->setCenter(static_cast<__int128_t>(centers[0].toDouble()));
			imgWindowInt->setWidth(static_cast<__int128_t>(widths[0].toDouble()));
		} else {

		}


	}

	//

	if (gdcmDataSet.FindDataElement(gdcm::TagPixelPaddingValue)) {
		auto background = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagPixelPaddingValue)).toInt(&ok);

		if (!ok) throw DicomTagParseError(gdcm::TagPixelPaddingValue);

		imgWindowInt->setBackgroundLvl(background);
	}

	//

	if (gdcmDataSet.FindDataElement(gdcm::TagRescaleIntercept)) {
		gdcm::assertTagPresence(gdcmDataSet, gdcm::TagRescaleSlope);

		auto rescale = gdcm::ImageHelper::GetRescaleInterceptSlopeValue(gdcmFile);

		if (rescale.size() == 2) {
			imgWindowInt->setRescaleIntercept(rescale[0]);
			imgWindowInt->setRescaleSlope(rescale[1]);
		}
	}

	{
		if (gdcmImage.GetNumberOfOverlays() > 0)
			qDebug() << "Obraz z Overlayem (sprawdzić o co kaman)";
	}
}

bool Monochrome2::Scene::generatePixmap() {

	if (!imgWindow->genLUT()) return false;

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
			genQPixmapOfType<quint32>();
			break;

		case gdcm::PixelFormat::INT64:
			genQPixmapOfType<qint64>();
			break;
		case gdcm::PixelFormat::UINT64:
			genQPixmapOfType<quint64>();
			break;

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	auto img = QImage((uchar *) &targetBuffer[0], imgDimX, imgDimY, sizeof(Pixel) * imgDimX, QImage::Format_RGB888);
	pixmap.convertFromImage(img);

	return true;
}

template<typename T>
void Monochrome2::Scene::genQPixmapOfType() {

	auto *buffer = &targetBuffer[0];
	auto *origin = (T *) &originBuffer[0];

	switch (imgWindow->type()) {
		case Window::IntDynamic: {

			auto windowPtr = (WindowIntDynamic *) imgWindow;

			for (quint64 i = 0; i < imgDimX * imgDimY; i++, origin++) {
				*buffer++ = windowPtr->getLUT(*origin);
			}
		}
			break;

		case Window::IntStatic: {

			auto windowPtr = (WindowIntStatic *) imgWindow;

			for (quint64 i = 0; i < imgDimX * imgDimY; i++, origin++) {
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

void Monochrome2::Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	if (event->buttons() & Qt::LeftButton) {

		switch (getDicomView()->getToolBar().getState()) {

			case DicomToolBar::Windowing: {
				event->accept();

				int dx = event->lastScreenPos().x() - event->screenPos().x(),
						dy = event->lastScreenPos().y() - event->screenPos().y();

				imgWindow->mvHorizontal(dx);
				imgWindow->mvVertical(dy);


				reloadPixmap();
			}
				break;
		}
	}

	DicomScene::mouseMoveEvent(event);
}

void Monochrome2::Scene::toolBarAdjust(DicomToolBar *toolbar) {
	DicomScene::toolBarAdjust(toolbar);

	auto winAction = toolbar->getActionWindowing();
	winAction->setMenu(imgWindow->getMenu());
	winAction->setEnabled(true);
}
