
#include "toolbar.h"
#include "dataset.h"

using namespace Sokar;

DicomToolBar::DicomToolBar(QWidget *parent) : QToolBar(parent) {

	toggleActionGrp = new QActionGroup(this);

	connect(this, &DicomToolBar::stateToggleSignal, [&](State state) {
		this->state = state;
	});

	initActions();
}

void DicomToolBar::initActions() {
	QAction *action;

	Windowing:
	{

		actionWindowing = new QAction(QIcon(":/img/ico/window.png"), tr("Windowing"), this);
		actionWindowing->setActionGroup(toggleActionGrp);
		actionWindowing->setCheckable(true);
		actionWindowing->setDisabled(true);
		addAction(actionWindowing);

		connect(actionWindowing, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Windowing);
		});

		//https://forum.qt.io/topic/23704/solved-submenu-from-qtoolbar-button/3
		auto btn = (QToolButton *) widgetForAction(actionWindowing);
		btn->setPopupMode(QToolButton::MenuButtonPopup);
	}

	Pan:
	{

		actionPan = new QAction(QIcon(":/img/ico/hand.png"), tr("Pan"), this);
		actionPan->setActionGroup(toggleActionGrp);
		actionPan->setCheckable(true);
		addAction(actionPan);

		connect(actionPan, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Pan);
		});

		auto btn = (QToolButton *) widgetForAction(actionPan);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		actionPan->setMenu(menu);

		{
			action = new QAction(QIcon(":/img/ico/clean.png"), tr("Move To Center"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(ClearPan);
			});
		}
	}

	Zoom:
	{

		actionZoom = new QAction(QIcon(":/img/ico/zoom.png"), tr("Zoom"), this);
		actionZoom->setActionGroup(toggleActionGrp);
		actionZoom->setCheckable(true);
		addAction(actionZoom);

		connect(actionZoom, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Zoom);
		});

		auto btn = (QToolButton *) widgetForAction(actionZoom);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		actionZoom->setMenu(menu);

		{
			action = new QAction(QIcon(":/img/ico/fit2screen.png"), tr("Fit To Screen"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(Fit2Screen);
			});
		}

		{
			action = new QAction(QIcon(":/img/ico/originalSize.png"), tr("Original Resolution"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(OriginalResolution);
			});
		}
	}

	Rotate:
	{

		actionRotate = addAction(QIcon(":/img/ico/rotate.png"), tr("Rotate"));
		actionRotate->setActionGroup(toggleActionGrp);
		actionRotate->setCheckable(true);

		connect(actionRotate, &QAction::toggled, [=](bool checked) {
			if (checked) emit stateToggleSignal(Rotate);
		});

		auto btn = (QToolButton *) widgetForAction(actionRotate);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		actionRotate->setMenu(menu);

		{
			action = new QAction(QIcon(":/img/ico/rotateRight.png"), tr("Rotate Right"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(RotateRight90);
			});
		}
		{
			action = new QAction(QIcon(":/img/ico/rotateLeft.png"), tr("Rotate Left"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(RotateLeft90);
			});
		}
		menu->addSeparator();
		{
			action = new QAction(QIcon(":/img/ico/flipHorizontal.png"), tr("Flip Horizontal"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(FlipHorizontal);
			});
		}
		{
			action = new QAction(QIcon(":/img/ico/flipVertical.png"), tr("Flip Vertical"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(FlipVertical);
			});
		}
		menu->addSeparator();
		{
			action = new QAction(QIcon(":/img/ico/clean.png"), tr("Clear Transformation"), menu);
			menu->addAction(action);
			connect(action, &QAction::triggered, [=](bool) {
				emit actionTriggerSignal(ClearRotate);
			});

		}
	}

	Tags:
	{
		actionTags = new QAction(QIcon(":/img/ico/tags.png"), tr("DICOM Tags"), this);
		addAction(actionTags);
		connect(actionTags, &QAction::triggered, [=](bool) {
			emit actionTriggerSignal(OpenDataSet);
		});
	}

	for (auto &action : actions()) {
		action->setDisabled(true);
	}
}
