
#include "dicomtoolbar.h"

using namespace Sokar;

DicomToolBar::DicomToolBar(QWidget *parent) : QToolBar(parent) {

	auto action = addAction("Okienkowanie");

	auto menu = new QMenu();
	menu->addAction(new QAction("dupa"));
	action->setMenu(menu);

	action->setCheckable(true);

	//https://forum.qt.io/topic/23704/solved-submenu-from-qtoolbar-button/3
	auto btn = (QToolButton*) widgetForAction(action);
	btn->setPopupMode(QToolButton::MenuButtonPopup);
}