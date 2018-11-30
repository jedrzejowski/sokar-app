
#include "toolbar.h"
#include "dataset.h"

using namespace Sokar;

DicomToolBar::DicomToolBar(QWidget *parent) : QToolBar(parent) {

	toggleActionGrp = new QActionGroup(this);

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

			auto orginalResolution = new QAction(QIcon(":/img/icons/orginalsize.png"), ("Original Resolution"));
			menu->addAction(orginalResolution);
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
		menu->addAction(new QAction("dupa"));
		action->setMenu(menu);

		{
			auto rotateRight = new QAction(QIcon(":/img/icons/rotateRight.png"), tr("Rotate Right"));
			menu->addAction(rotateRight);

			auto rotateLeft = new QAction(QIcon(":/img/icons/rotateLeft.png"), tr("Rotate Left"));
			menu->addAction(rotateLeft);

			menu->addSeparator();

			auto flipHorizontal = new QAction(tr("Rotate Right"));
			flipHorizontal->setIconText(":/img/icons/flipHorizontal.png");
			menu->addAction(flipHorizontal);

			auto flipVertical = new QAction(QIcon(":/img/icons/flipVertical.png"), tr("Rotate Left"));
			menu->addAction(flipVertical);

			menu->addSeparator();

			auto clearTransformation = new QAction(QIcon(":/img/icons/flipHorizontal.png"), tr("Clear Transformation"));
			menu->addAction(clearTransformation);

		}
	}

	Tags:
	{
		auto action = addAction(QIcon(":/img/icons/tags.png"), tr("DICOM Tags"));
		connect(action, &QAction::triggered, [=](bool checked) {
			emit actionTriggerSignal(OpenDataSet);
		});
	}
}
