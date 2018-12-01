
#include "toolbar.h"
#include "dataset.h"

using namespace Sokar;

DicomToolBar::DicomToolBar(QWidget *parent) : QToolBar(parent) {

	toggleActionGrp = new QActionGroup(this);

	connect(this, &DicomToolBar::stateToggleSignal, [=](State state) {
		this->state = state;
	});

	initActions();
}

void DicomToolBar::initActions() {

	Windowing:
	{

		auto action = addAction(QIcon(":/img/icons/window.png"), tr("Windowing"));
		action->setActionGroup(toggleActionGrp);
		action->setCheckable(true);

		connect(action, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Windowing);
		});

		//https://forum.qt.io/topic/23704/solved-submenu-from-qtoolbar-button/3
		auto btn = (QToolButton *) widgetForAction(action);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		//TODO import menu z oecnej scenie
	}

	Pan:
	{

		auto action = addAction(QIcon(":/img/icons/hand.png"), tr("Pan"));
		action->setActionGroup(toggleActionGrp);
		action->setCheckable(true);

		connect(action, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Pan);
		});

		auto btn = (QToolButton *) widgetForAction(action);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu();
		action->setMenu(menu);

		{
			auto clearPan = new QAction(QIcon(":/img/icons/clean.png"), tr("Move To Center"));
			menu->addAction(clearPan);
			connect(clearPan, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(ClearPan);
			});
		}
	}

	Zoom:
	{

		auto action = addAction(QIcon(":/img/icons/zoom.png"), tr("Zoom"));
		action->setActionGroup(toggleActionGrp);
		action->setCheckable(true);

		connect(action, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Zoom);
		});

		auto btn = (QToolButton *) widgetForAction(action);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu();
		action->setMenu(menu);

		{
			auto fit2screen = new QAction(QIcon(":/img/icons/fit2screen.png"), tr("Fit To Screen"));
			menu->addAction(fit2screen);
			connect(fit2screen, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(Fit2Screen);
			});

			auto originalResolution = new QAction(QIcon(":/img/icons/originalSize.png"), tr("Original Resolution"));
			menu->addAction(originalResolution);
			connect(originalResolution, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(OriginalResolution);
			});
		}
	}

	Rotate:
	{

		auto action = addAction(QIcon(":/img/icons/rotate.png"), tr("Rotate"));
		action->setActionGroup(toggleActionGrp);
		action->setCheckable(true);

		connect(action, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Rotate);
		});

		auto btn = (QToolButton *) widgetForAction(action);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu();
		action->setMenu(menu);

		{
			auto rotateRight = new QAction(QIcon(":/img/icons/rotateRight.png"), tr("Rotate Right"));
			menu->addAction(rotateRight);
			connect(rotateRight, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(RotateRight90);
			});

			auto rotateLeft = new QAction(QIcon(":/img/icons/rotateLeft.png"), tr("Rotate Left"));
			menu->addAction(rotateLeft);
			connect(rotateLeft, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(RotateLeft90);
			});


			menu->addSeparator();

			auto flipHorizontal = new QAction(QIcon(":/img/icons/flipHorizontal.png"), tr("Flip Horizontal"));
			menu->addAction(flipHorizontal);
			connect(flipHorizontal, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(FlipHorizontal);
			});

			auto flipVertical = new QAction(QIcon(":/img/icons/flipVertical.png"), tr("Flip Vertical"));
			menu->addAction(flipVertical);
			connect(flipVertical, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(FlipVertical);
			});

			menu->addSeparator();

			auto clearTransformation = new QAction(QIcon(":/img/icons/clean.png"), tr("Clear Transformation"));
			menu->addAction(clearTransformation);
			connect(clearTransformation, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(ClearRotate);
			});

		}
	}

	Tags:
	{
		auto action = addAction(QIcon(":/img/icons/tags.png"), tr("DICOM Tags"));
		connect(action, &QAction::triggered, [=](bool) {
			emit actionTriggerSignal(OpenDataSet);
		});
	}
}
