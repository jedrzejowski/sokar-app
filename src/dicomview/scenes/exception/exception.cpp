#include "exception.h"
#include "../../dicomview.h"

using namespace Sokar;

ExceptionScene::ExceptionScene(SceneParams &sceneParams, Sokar::Exception &exception) :
		DicomScene(sceneParams) {

	msgText = addText("");
	msgText->setDefaultTextColor(QColor("white"));

	return;
	switch (exception.type()) {

		case Exception::WrongScope:
			msgText->setHtml("WrongScope");
			break;
		case Exception::ImageTypeNotSupported:
			msgText->setHtml("ImageTypeNotSupported");
			break;
		case Exception::DicomTagMissing:
			msgText->setHtml("DicomTagMissing");
			break;
		case Exception::DicomTagParseError:
			msgText->setHtml("DicomTagParseError");
			break;
	}

	static const gdcm::Tag
			TagSegmentedRedPaletteColorLookupTableData(0x0028, 0x1221);

	auto &redSeq = gdcmDataSet.GetDataElement(TagSegmentedRedPaletteColorLookupTableData);
	auto qq = (quint16 *) redSeq.GetByteValue()->GetPointer();

	auto ww = (quint16 *) &originBuffer[0];

	for (int i = 0; i < imgDimX * imgDimY; i += 2) {
		qDebug() << *ww;
		ww++;
	}
}

ExceptionScene::~ExceptionScene() {

}

bool Sokar::ExceptionScene::generatePixmap() {
	if (!pixmap.isNull())
		return false;

	auto img = QImage(1, 1, QImage::Format_RGB888);
	img.fill(Qt::black);
	pixmap = QPixmap::fromImage(img);
	return true;
}

void ExceptionScene::reposItems() {
	DicomScene::reposItems();

	msgText->setPos((this->width() - msgText->document()->size().width()) / 2,
					(this->height() - msgText->document()->size().height()) / 2);
}

void ExceptionScene::toolBarAdjust() {
	DicomScene::toolBarAdjust();

	auto *toolBar = getDicomView()->getToolBar();

	toolBar->getActionWindowing()->setEnabled(false);
	toolBar->getActionPan()->setEnabled(false);
	toolBar->getActionZoom()->setEnabled(false);
	toolBar->getActionRotate()->setEnabled(false);
}
