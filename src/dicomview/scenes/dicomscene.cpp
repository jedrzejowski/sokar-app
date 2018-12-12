
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "sokar/gdcmSokar.h"
#include "sokar/gdcmSokar.h"

#include "dicomscene.h"
#include "dicomsceneset.h"
#include "../dicomview.h"
#include "../framechooser.h"

using namespace Sokar;

DicomScene::DicomScene(SceneParams &sceneParams) :
		Scene((QObject *) sceneParams.dicomSceneSet),
		gdcmFile(sceneParams.imageReader->GetFile()),
		gdcmImage(sceneParams.imageReader->GetImage()),
		gdcmDataSet(gdcmFile.GetDataSet()) {

	gdcmStringFilter.SetFile(gdcmFile);

	imgDimX = gdcmImage.GetDimension(0);
	imgDimY = gdcmImage.GetDimension(1);

	{
		//Tworzenie sub wektora dla naszych danych
		auto offset = sceneParams.frame * sceneParams.imgSize;

		originBuffer = std::vector<char>(sceneParams.imageBuffer->begin() + offset,
										 sceneParams.imageBuffer->begin() + offset + sceneParams.imgSize);
	}

	targetBuffer.resize(sceneParams.imgSize);

	qImage = QImage((uchar *) &targetBuffer[0], imgDimX, imgDimY, sizeof(Pixel) * imgDimX, QImage::Format_RGB888);

	initIndicators();
}

DicomScene::~DicomScene() {

}

void DicomScene::reposItems() {
	Scene::reposItems();

	if (pixmapItem != nullptr)
		pixmapItem->setPos(this->width() / 2, this->height() / 2);

}

void DicomScene::reloadPixmap() {

	if (!generatePixmap()) return;

	if (pixmapItem == nullptr) {
		pixmapItem = addPixmap(pixmap);
		pixmapItem->setZValue(-1);

		centerTransformat.translate((qreal) pixmap.width() / -2, (qreal) pixmap.height() / -2);
		updatePixmapTransformation();
	} else {
		pixmapItem->setPixmap(pixmap);
	}

}

QTransform DicomScene::pixmapTransformation() {
	QTransform transform;
	transform *= centerTransformat;
	transform *= scaleTransform;
	transform *= rotateTransform;
	transform *= panTransform;
	return transform;
}


void DicomScene::updatePixmapTransformation() {
	pixmapItem->setTransform(pixmapTransformation(), false);

	if (imageOrientationIndicator != nullptr)
		imageOrientationIndicator->setRotateTransform(rotateTransform);
}

SceneAvatar *DicomScene::getAvatar() {
	return new SceneAvatar(this);
}


const QPixmap &DicomScene::getIcon() {
	if (iconPixmap.isNull()) {
		generatePixmap();

		iconPixmap = pixmap.scaledToWidth(128, Qt::SmoothTransformation);
	}

	return iconPixmap;
}

void DicomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	if (event->buttons() & Qt::LeftButton) {

		switch (getDicomView()->getToolBar().getState()) {

			case DicomToolBar::Pan: {
				panTransform.translate(
						event->screenPos().x() - event->lastScreenPos().x(),
						event->screenPos().y() - event->lastScreenPos().y());

				updatePixmapTransformation();
			}
				break;

			case DicomToolBar::Zoom: {
				qreal scale = 1;

				scale -= (event->screenPos().y() - event->lastScreenPos().y()) * 0.01;
				scale -= (event->screenPos().x() - event->lastScreenPos().x()) * 0.001;

				scaleTransform.scale(scale, scale);
				updatePixmapTransformation();
			}
				break;

			case DicomToolBar::Rotate: {

				qreal rotate = 0;

				rotate += (event->screenPos().y() - event->lastScreenPos().y()) * 0.5;
				rotate += (event->screenPos().x() - event->lastScreenPos().x()) * 0.1;

				rotateTransform.rotate(rotate);
				updatePixmapTransformation();

			}
				break;
		}
	}

	QGraphicsScene::mouseMoveEvent(event);
}

//region Indicators

void DicomScene::initIndicators() {
	try {
		initPatientDataIndicator();
		initHospitalDataIndicator();
		initPixelSpacingIndicator();
		initImageOrientationIndicator();
	} catch (Sokar::Exception &) {}
}

void DicomScene::initPatientDataIndicator() {
	patientDataIndicator = new PatientDataIndicator;
	patientDataIndicator->loadData(gdcmFile);
	addIndicator(patientDataIndicator);
}

void DicomScene::initHospitalDataIndicator() {
	hospitalDataIndicator = new HospitalDataIndicator;
	hospitalDataIndicator->loadData(gdcmFile);
	addIndicator(hospitalDataIndicator);
}

void DicomScene::initPixelSpacingIndicator() {
	if (!gdcmDataSet.FindDataElement(gdcm::TagPixelSpacing)) return;

	auto pixelSpacings = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagPixelSpacing))
			.split(gdcm::StringSplitter);

	bool ok = false;

	pixelSpacingIndicator = new PixelSpacingIndicator;
	addIndicator(pixelSpacingIndicator);

	if (pixelSpacings.length() >= 1) {
		auto spacing = pixelSpacings[0].toDouble(&ok);

		if (ok) {
			pixelSpacingIndicator->setXSpacing(spacing);
			pixelSpacingIndicator->setXDim(gdcmImage.GetDimension(0));
		}
	}

	if (pixelSpacings.length() >= 2) {

		auto spacing = pixelSpacings[1].toDouble(&ok);

		if (ok) {
			pixelSpacingIndicator->setYSpacing(spacing);
			pixelSpacingIndicator->setYDim(gdcmImage.GetDimension(0));
		}
	}
}

void DicomScene::initImageOrientationIndicator() {
	if (!gdcmDataSet.FindDataElement(gdcm::TagImageOrientationPatient)) return;

	auto orientations = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagImageOrientationPatient));

	imageOrientationIndicator = new ImageOrientationIndicator();
	imageOrientationIndicator->setOrientation(orientations);

	imageOrientationIndicator->setOffsetBottomParent(pixelSpacingIndicator);
	imageOrientationIndicator->setOffsetRightParent(pixelSpacingIndicator);

	addIndicator(imageOrientationIndicator);
}

//endregion

void DicomScene::toolBarAdjust(DicomToolBar *toolBar) {
	toolBar->getActionPan()->setEnabled(true);
	toolBar->getActionZoom()->setEnabled(true);
	toolBar->getActionRotate()->setEnabled(true);
	toolBar->getActionTags()->setEnabled(true);
}

void DicomScene::toolBarActionSlot(DicomToolBar::Action action) {
	bool updateTransform = false;

	switch (action) {

		case DicomToolBar::ClearPan:
			panTransform = QTransform();
			updateTransform = true;
			break;

		case DicomToolBar::Fit2Screen:
			if (!pixmap.isNull()) {
				updateTransform = true;
				scaleTransform = QTransform();

				auto pw = pixmap.size().width();
				auto ph = pixmap.size().height();

				auto sw = sceneRect().width();
				auto sh = sceneRect().height();

				qreal nh, nw;

				nw = sw;
				nh = nw * ph / pw;
				if (nh <= sh) {
					scaleTransform.scale(nw / pw, nh / ph);
					break;
				}

				nh = sh;
				nw = nh * pw / ph;
				if (nw <= sw) {
					scaleTransform.scale(nw / pw, nh / ph);
					break;
				}

			}
			break;

		case DicomToolBar::OriginalResolution:
			updateTransform = true;
			scaleTransform = QTransform();
			break;

		case DicomToolBar::RotateRight90:
			updateTransform = true;
			rotateTransform.rotate(90);
			break;

		case DicomToolBar::RotateLeft90:
			rotateTransform.rotate(-90);
			updateTransform = true;
			break;

		case DicomToolBar::FlipHorizontal:
			rotateTransform.scale(1, -1);
			updateTransform = true;
			break;

		case DicomToolBar::FlipVertical:
			rotateTransform.scale(-1, 1);
			updateTransform = true;
			break;

		case DicomToolBar::ClearRotate:
			rotateTransform = QTransform();
			updateTransform = true;
			break;
	}

	if (updateTransform) {
		updatePixmapTransformation();
		reposItems();
	}
}

void DicomScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
	QGraphicsScene::wheelEvent(event);

	if (event->delta() < 0)
		getDicomView()->getFrameChooser().moveNext();

	if (event->delta() > 0)
		getDicomView()->getFrameChooser().movePrev();
}
