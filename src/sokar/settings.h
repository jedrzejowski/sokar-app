#pragma once

#include <QtCore>

namespace Sokar {

	extern QSettings *qSettings;

	namespace Settings {
//		void bumpRecentOpen(const QString &path) {
//			auto recentOpen = qSettings->value("recentOpen", QStringList()).value<QStringList>();
//			recentOpen << path;
//			if (recentOpen.size() > 10) recentOpen.pop_front();
//			qSettings->setValue("recentOpen", recentOpen);
//		}
	}
}