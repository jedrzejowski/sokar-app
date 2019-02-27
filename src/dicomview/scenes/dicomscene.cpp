#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>
#include <src/dicomview/toolbar.h>

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
	QMutexLocker locker(&processing);

	if (!generatePixmap()) return;

	if (pixmapItem == nullptr) {
		pixmapItem = addPixmap(pixmap);
		pixmapItem->setZValue(-1);

		centerTransform.translate((qreal) pixmap.width() / -2, (qreal) pixmap.height() / -2);
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

	pixmapItem->setTransform(isMovieMode() ?
							 movieMode->getOriginScene()->pixmapTransformation() :
							 pixmapTransformation());

	if (imageOrientationIndicator != nullptr) {
		imageOrientationIndicator->setRotateTransform(
				isMovieMode() ?
				movieMode->getOriginScene()->rotateTransform :
				rotateTransform);
	}
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

		auto target = this;

		if (isMovieMode()) target = movieMode->getOriginScene();

		switch (getDicomView()->getToolBar()->getState()) {

			case DicomToolBar::Pan: {
				target->panTransform.translate(
						event->screenPos().x() - event->lastScreenPos().x(),
						event->screenPos().y() - event->lastScreenPos().y());

				updatePixmapTransformation();
			}
				break;

			case DicomToolBar::Zoom: {
				qreal scale = 1;

				scale -= (event->screenPos().y() - event->lastScreenPos().y()) * 0.01;
				scale -= (event->screenPos().x() - event->lastScreenPos().x()) * 0.001;

				target->scaleTransform.scale(scale, scale);
				updatePixmapTransformation();
			}
				break;

			case DicomToolBar::Rotate: {

				qreal rotate = 0;

				rotate += (event->screenPos().y() - event->lastScreenPos().y()) * 0.5;
				rotate += (event->screenPos().x() - event->lastScreenPos().x()) * 0.1;

				target->rotateTransform.rotate(rotate);
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
	} catch (Sokar::Exception &) {}

	try {
		initHospitalDataIndicator();
	} catch (Sokar::Exception &) {}

	try {
		initPixelSpacingIndicator();
	} catch (Sokar::Exception &) {}

	try {
		initImageOrientationIndicator();
	} catch (Sokar::Exception &) {}

	try {
		initModalityIndicator();
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
	pixelSpacingIndicator = new PixelSpacingIndicator(dataConverter);

	const static gdcm::Tag
	/**
	 * All pixel spacing related Attributes are encoded as the physical distance between the centers of each two-dimensional pixel, specified by two numeric values.
	 * The first value is the row spacing in mm, that is the spacing between the centers of adjacent rows, or vertical spacing.
	 * The second value is the column spacing in mm, that is the spacing between the centers of adjacent columns, or horizontal spacing.
	 */     TagPixelSpacing(0x0028, 0x0030);

	if (!dataConverter.hasTagWithData(TagPixelSpacing)) return;
	addIndicator(pixelSpacingIndicator);

	auto spacing = dataConverter.toDecimalString(TagPixelSpacing);

	if (spacing.length() != 2)
		throw DicomTagParseError(TagPixelSpacing);

	pixelSpacingIndicator->setXSpacing(spacing[1]);
	pixelSpacingIndicator->setYSpacing(spacing[0]);

	pixelSpacingIndicator->setXDim(gdcmImage.GetDimension(0));
	pixelSpacingIndicator->setYDim(gdcmImage.GetDimension(1));

	pixelSpacingIndicator->updateLines();
}

void DicomScene::initImageOrientationIndicator() {

	imageOrientationIndicator = new ImageOrientationIndicator(dataConverter);

	imageOrientationIndicator->setOffsetBottomParent(pixelSpacingIndicator);
	imageOrientationIndicator->setOffsetRightParent(pixelSpacingIndicator);

	addIndicator(imageOrientationIndicator);
}

void DicomScene::initModalityIndicator() {
	modalityIndicator = new ModalityIndicator(dataConverter);
	addIndicator(modalityIndicator);
}

//endregion

//region ToolBar Actions

void DicomScene::toolBarAdjust() {
	auto *toolBar = getDicomView()->getToolBar();
	toolBar->getActionPan()->setEnabled(true);
	toolBar->getActionZoom()->setEnabled(true);
	toolBar->getActionRotate()->setEnabled(true);
	toolBar->getActionTags()->setEnabled(true);

	toolBar->getActionIndicators()->setEnabled(true);
	patientDataIndicator->setVisible(toolBar->getActionPatientData()->isChecked());
	hospitalDataIndicator->setVisible(toolBar->getActionHospital()->isChecked());
	modalityIndicator->setVisible(toolBar->getActionModality()->isChecked());
}

void DicomScene::toolBarActionSlot(DicomToolBar::Action action, bool state) {
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

		case DicomToolBar::PatientData:
			patientDataIndicator->setVisible(state);
			break;

		case DicomToolBar::HospitalData:
			hospitalDataIndicator->setVisible(state);
			break;

		case DicomToolBar::ModalityData:
			modalityIndicator->setVisible(state);
			break;
	}

	if (updateTransform) {
		updatePixmapTransformation();
		reposItems();
	}
}

void DicomScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
	QGraphicsScene::wheelEvent(event);

	if (! isMovieMode()) {

		if (event->delta() < 0)
			getDicomSceneSet()->getSceneSequence()->stepForward();

		if (event->delta() > 0)
			getDicomSceneSet()->getSceneSequence()->stepBackward();
	}
}

//endregion

bool DicomScene::saveToFile(const QString &fileName, const char *format, int quality) {
	return qImage.save(fileName);
}

//region MovieMode

bool DicomScene::acceptMovieMode(MovieMode *movieMode) {
	auto *scene = movieMode->getOriginScene();

	if (scene == nullptr) return false;

	if (scene->gdcmImage.GetPhotometricInterpretation() != gdcmImage.GetPhotometricInterpretation())
		return false;

	if (scene->gdcmImage.GetColumns() != gdcmImage.GetColumns())
		return false;

	if (scene->gdcmImage.GetRows() != gdcmImage.GetRows())
		return false;

	if (scene->gdcmImage.GetPixelFormat() != gdcmImage.GetPixelFormat())
		return false;

	this->movieMode = movieMode;

	updatePixmapTransformation();
	return true;
}

void DicomScene::disableMovieMode() {
	movieMode = nullptr;

	updatePixmapTransformation();
}

bool DicomScene::isMovieMode() {
	return movieMode != nullptr && movieMode->getOriginScene() != nullptr;
}

//endregion

void DicomScene::prepare() {
	reloadPixmap();
}

void DicomScene::attached() {
	reposItems();
	toolBarAdjust();
	updatePixmapTransformation();
}
