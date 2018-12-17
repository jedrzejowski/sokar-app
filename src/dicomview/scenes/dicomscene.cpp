
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "sokar/gdcmSokar.h"
#include "sokar/gdcmSokar.h"

#include "dicomscene.h"
#include "../dicomview.h"
#include "../framechooser.h"

using namespace Sokar;

DicomScene::DicomScene(SceneParams &sceneParams) :
		Scene((QObject *) sceneParams.dicomSceneSet),
		gdcmFile(sceneParams.imageReader->GetFile()),
		gdcmImage(sceneParams.imageReader->GetImage()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		dataConverter(*sceneParams.dataConverter) {

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
	qDebug() << "~DicomScene()";
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

		centerTransform.translate((qreal) pixmap.width() / -2, (qreal) pixmap.height() / -2);
		updatePixmapTransformation();
	} else {
		pixmapItem->setPixmap(pixmap);
	}

}

QTransform DicomScene::pixmapTransformation() {
	QTransform transform;
	transform *= centerTransform;
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

DicomView *DicomScene::getDicomView() {
	auto views = this->views();
	if (views.count() != 1) return nullptr;

	return (DicomView *) views[0]->parent()->parent();
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
	patientDataIndicator = new PatientDataIndicator(dataConverter);
	addIndicator(patientDataIndicator);
}

void DicomScene::initHospitalDataIndicator() {
	hospitalDataIndicator = new HospitalDataIndicator(dataConverter);
	addIndicator(hospitalDataIndicator);
}

void DicomScene::initPixelSpacingIndicator() {
	static gdcm::Tag
			TagPixelSpacing(0x0028, 0x0030);

	if (!gdcmDataSet.FindDataElement(TagPixelSpacing)) return;


	pixelSpacingIndicator = new PixelSpacingIndicator(dataConverter);
	addIndicator(pixelSpacingIndicator);
}

void DicomScene::initImageOrientationIndicator() {
	static gdcm::Tag
			TagImageOrientationPatient(0x0020, 0x0037);

	if (!gdcmDataSet.FindDataElement(TagImageOrientationPatient)) return;

	imageOrientationIndicator = new ImageOrientationIndicator(dataConverter);

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

bool DicomScene::saveToFile(const QString &fileName, const char *format, int quality) {

	qDebug() << fileName;
	qDebug() << qImage;

	return qImage.save(fileName);
}
