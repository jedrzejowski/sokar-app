
#include "toolbar.h"

using namespace Sokar;

DicomToolBar::DicomToolBar(QWidget *parent) : QToolBar(parent) {
	dicomView = (DicomView *) parent;
	initActions();
}

void DicomToolBar::initActions() {

	Windowing:
	{

		auto action = addAction(QIcon(":/img/icons/window.png"), tr("Windowing"));

		auto menu = new QMenu();
		menu->addAction(new QAction("dupa"));
		action->setMenu(menu);

		action->setCheckable(true);

		//https://forum.qt.io/topic/23704/solved-submenu-from-qtoolbar-button/3
		auto btn = (QToolButton *) widgetForAction(action);
		btn->setPopupMode(QToolButton::MenuButtonPopup);
	}

	Tags:
	{

		auto action = addAction(QIcon(":/img/icons/tags.png"), tr("DICOM Tags"));
		connect(action, &QAction::triggered, [](bool checked) {
			qDebug("heja");
		});
	}
}
