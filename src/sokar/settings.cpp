#include "settings.h"

using namespace Sokar;

#define SETTINGS_RECENTOPEN "recentOpen"

QStringList Settings::recentOpen() {
	return qSettings->value(SETTINGS_RECENTOPEN, QStringList()).value<QStringList>();
}


void Settings::bumpRecentOpen(const QString &path) {
	auto recentOpen = Settings::recentOpen();

	recentOpen << path;

	recentOpen.removeDuplicates();

	if (recentOpen.size() > 10) recentOpen.pop_front();
	
	qSettings->setValue(SETTINGS_RECENTOPEN, recentOpen);
}
