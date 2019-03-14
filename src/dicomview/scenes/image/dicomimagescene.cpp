#include "dicomimagescene.h"
#include "params.h"

using namespace Sokar;

DicomImageScene::DicomImageScene(SceneParams &sceneParams) :
		DicomScene(sceneParams.dicomBundle, sceneParams.dicomSceneSet) {

	imgDimX = dicomBundle->gdcmImage->GetDimension(0);
	imgDimY = dicomBundle->gdcmImage->GetDimension(1);

	{
		//Tworzenie sub wektora dla naszych danych
		auto offset = sceneParams.frame * sceneParams.imgSize;

		originBuffer = std::vector<char>(sceneParams.imageBuffer->begin() + offset,
										 sceneParams.imageBuffer->begin() + offset + sceneParams.imgSize);
	}

	targetBuffer.resize(sceneParams.imgSize);

	qImage = QImage((uchar *) &targetBuffer[0], imgDimX, imgDimY, sizeof(Pixel) * imgDimX, QImage::Format_RGB888);

	initIndicators();

	connect(this, &DicomScene::movieModeAccepted, this, &DicomImageScene::updatePixmapTransformation);
	connect(this, &DicomScene::movieModeDisabled, this, &DicomImageScene::updatePixmapTransformation);
}


void DicomImageScene::reloadPixmap() {
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

QTransform DicomImageScene::pixmapTransformation() {
	QTransform transform;
	transform *= centerTransform;
	transform *= scaleTransform;
	transform *= rotateTransform;
	transform *= panTransform;
	return transform;
}

void DicomImageScene::updatePixmapTransformation() {

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

SceneAvatar *DicomImageScene::getAvatar() {
	return new SceneAvatar(this);
}

const QPixmap &DicomImageScene::getIcon() {
	if (iconPixmap.isNull()) {
		generatePixmap();

		iconPixmap = pixmap.scaledToWidth(128, Qt::SmoothTransformation);
	}

	return iconPixmap;
}

void DicomImageScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

	if (event->buttons() & Qt::LeftButton) {

		auto target = this;

//		if (isMovieMode()) target = movieMode->getOriginScene();

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


void DicomImageScene::initPixelSpacingIndicator() {
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

	pixelSpacingIndicator->setXDim(imgDimX);
	pixelSpacingIndicator->setYDim(imgDimY);

	pixelSpacingIndicator->updateLines();
}

void DicomImageScene::initImageOrientationIndicator() {

	imageOrientationIndicator = new ImageOrientationIndicator(dataConverter);

	imageOrientationIndicator->setOffsetBottomParent(pixelSpacingIndicator);
	imageOrientationIndicator->setOffsetRightParent(pixelSpacingIndicator);

	addIndicator(imageOrientationIndicator);
}

void DicomImageScene::prepare() {
	DicomScene::prepare();
	reloadPixmap();
}

void DicomImageScene::attached() {
	DicomScene::attached();
	updatePixmapTransformation();
}

bool DicomImageScene::isMovieModeAcceptable(MovieMode *movieMode) {
	if (!DicomScene::isMovieModeAcceptable(movieMode))
		return false;

	auto *scene = movieMode->getOriginScene();
	auto &sceneBundle = scene->getDicomBundle();

	if (sceneBundle.gdcmImage->GetPhotometricInterpretation() != dicomBundle->gdcmImage->GetPhotometricInterpretation())
		return false;

	if (sceneBundle.gdcmImage->GetColumns() != dicomBundle->gdcmImage->GetColumns())
		return false;

	if (sceneBundle.gdcmImage->GetRows() != dicomBundle->gdcmImage->GetRows())
		return false;

	if (sceneBundle.gdcmImage->GetPixelFormat() != dicomBundle->gdcmImage->GetPixelFormat())
		return false;

	return true;
}
