
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

	Windowing:
	{

		action.windowing = new QAction(QIcon(":/img/ico/window.png"), tr("Windowing"), this);
		action.windowing->setActionGroup(toggleActionGrp);
		action.windowing->setCheckable(true);
		action.windowing->setDisabled(true);
		addAction(action.windowing);

		connect(action.windowing, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Windowing);
		});

		//https://forum.qt.io/topic/23704/solved-submenu-from-qtoolbar-button/3
		auto btn = (QToolButton *) widgetForAction(action.windowing);
		btn->setPopupMode(QToolButton::MenuButtonPopup);
	}

	Pan:
	{

		action.pan = new QAction(QIcon(":/img/ico/hand.png"), tr("Pan"), this);
		action.pan->setActionGroup(toggleActionGrp);
		action.pan->setCheckable(true);
		addAction(action.pan);

		connect(action.pan, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Pan);
		});

		auto btn = (QToolButton *) widgetForAction(action.pan);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		action.pan->setMenu(menu);

		{
			action.panClean = new QAction(QIcon(":/img/ico/clean.png"), tr("Move To Center"), menu);
			menu->addAction(action.panClean);
			connect(action.panClean, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(ClearPan);
			});
		}
	}

	Zoom:
	{

		action.zoom = new QAction(QIcon(":/img/ico/zoom.png"), tr("Zoom"), this);
		action.zoom->setActionGroup(toggleActionGrp);
		action.zoom->setCheckable(true);
		addAction(action.zoom);

		connect(action.zoom, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Zoom);
		});

		auto btn = (QToolButton *) widgetForAction(action.zoom);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		action.zoom->setMenu(menu);

		{
			action.zoomFit = new QAction(QIcon(":/img/ico/fit2screen.png"), tr("Fit To Screen"), menu);
			menu->addAction(action.zoomFit);
			connect(action.zoomFit, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(Fit2Screen);
			});
		}

		{
			action.zoom11 = new QAction(QIcon(":/img/ico/originalSize.png"), tr("Original Resolution"), menu);
			menu->addAction(action.zoom11);
			connect(action.zoom11, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(OriginalResolution);
			});
		}
	}

	Rotate:
	{

		action.rotate = new QAction(QIcon(":/img/ico/rotate.png"), tr("Rotate"), this);
		action.rotate->setActionGroup(toggleActionGrp);
		action.rotate->setCheckable(true);
		addAction(action.rotate);

		connect(action.rotate, &QAction::toggled, [&](bool checked) {
			if (checked) emit stateToggleSignal(Rotate);
		});

		auto btn = (QToolButton *) widgetForAction(action.rotate);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		action.rotate->setMenu(menu);

		{
			action.rotateRight = new QAction(QIcon(":/img/ico/rotateRight.png"), tr("Rotate Right"), menu);
			menu->addAction(action.rotateRight);
			connect(action.rotateRight, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(RotateRight90);
			});
		}
		{
			action.rotateLeft = new QAction(QIcon(":/img/ico/rotateLeft.png"), tr("Rotate Left"), menu);
			menu->addAction(action.rotateLeft);
			connect(action.rotateLeft, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(RotateLeft90);
			});
		}
		menu->addSeparator();
		{
			action.flipH = new QAction(QIcon(":/img/ico/flipHorizontal.png"), tr("Flip Horizontal"), menu);
			menu->addAction(action.flipH);
			connect(action.flipH, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(FlipHorizontal);
			});
		}
		{
			action.flipV = new QAction(QIcon(":/img/ico/flipVertical.png"), tr("Flip Vertical"), menu);
			menu->addAction(action.flipV);
			connect(action.flipV, &QAction::triggered, [&](bool) {
				emit actionTriggerSignal(FlipVertical);
			});
		}
		menu->addSeparator();
		{
			action.rotateClear = new QAction(QIcon(":/img/ico/clean.png"), tr("Clear Transformation"), menu);
			menu->addAction(action.rotateClear);
			connect(action.rotateClear, &QAction::triggered, [&]() {
				emit actionTriggerSignal(ClearRotate);
			});
		}
	}

	Indicators:
	{
		action.indicator.all = new QAction(QIcon(":/img/ico/dark/typography.png"), tr("Indicators"), this);
		addAction(action.indicator.all);

		connect(action.indicator.all, &QAction::triggered, [&]() {
			auto subactions = action.indicator.all->menu()->actions();
			auto checked = false;

			for (auto &action : subactions)
				checked = checked or action->isChecked();

			checked = not checked;

			for (auto &action : subactions)
				action->setChecked(checked);
		});

		auto btn = (QToolButton *) widgetForAction(action.indicator.all);
		btn->setPopupMode(QToolButton::MenuButtonPopup);

		auto menu = new QMenu(this);
		action.indicator.all->setMenu(menu);

		{
			action.indicator.patientData = new QAction(tr("Patient Data"), menu);
			action.indicator.patientData->setCheckable(true);
			action.indicator.patientData->setChecked(true);
			menu->addAction(action.indicator.patientData);
			connect(action.indicator.patientData, &QAction::toggled, [&](bool checked) {
				emit actionTriggerSignal(PatientData, checked);
			});
		}
		{
			action.indicator.hospital = new QAction(tr("Hospital Data"), menu);
			action.indicator.hospital->setCheckable(true);
			action.indicator.hospital->setChecked(true);
			menu->addAction(action.indicator.hospital);
			connect(action.indicator.hospital, &QAction::toggled, [&](bool checked) {
				emit actionTriggerSignal(HospitalData, checked);
			});
		}
		{
			action.indicator.modality = new QAction(tr("Image Acquisition"), menu);
			action.indicator.modality->setCheckable(true);
			action.indicator.modality->setChecked(true);
			menu->addAction(action.indicator.modality);
			connect(action.indicator.modality, &QAction::toggled, [&](bool checked) {
				emit actionTriggerSignal(ModalityData, checked);
			});
		}
	}

	addSeparator();

	Tags:
	{
		action.tags = new QAction(QIcon(":/img/ico/tags.png"), tr("DICOM Tags"), this);
		addAction(action.tags);
		connect(action.tags, &QAction::triggered, [&](bool) {
			emit actionTriggerSignal(OpenDataSet);
		});
	}

	for (auto &action : actions()) {
		action->setDisabled(true);
	}
}
