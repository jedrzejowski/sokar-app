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

DicomScene::DicomScene(const DicomBundle *dicomBundle, DicomSceneSet* dicomSceneSet) :
		Scene(dicomSceneSet),
		dicomBundle(dicomBundle),
		dataConverter(dicomBundle->dataConverter){


	initIndicators();
}

DicomScene::~DicomScene() {
	qDebug() << "~DicomScene()";
}

DicomView *DicomScene::getDicomView() {
	auto views = this->views();
	if (views.count() != 1) return nullptr;

	return (DicomView *) views[0]->parent()->parent();
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

	if (!isMovieMode()) {

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

	if(!isMovieModeAcceptable(movieMode)){
		return false;
	}

	this->movieMode = movieMode;

	emit movieModeAccepted();

	return true;
}

void DicomScene::disableMovieMode() {
	movieMode = nullptr;

	emit movieModeDisabled();
}

bool DicomScene::isMovieMode() {
	return movieMode != nullptr && movieMode->getOriginScene() != nullptr;
}


bool DicomScene::isMovieModeAcceptable(MovieMode *movieMode) {
	auto *scene = movieMode->getOriginScene();

	if (scene == nullptr)
		return false;

	return true;
}

//endregion

void DicomScene::prepare() {

}

void DicomScene::attached() {
	reposItems();
	toolBarAdjust();
}
