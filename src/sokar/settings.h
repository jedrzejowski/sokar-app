#pragma once

#include <QtCore>

namespace Sokar {

    extern QSettings *qSettings;

    namespace Settings {
        QStringList recentOpen();
        void bumpRecentOpen(const QString &path);
    }
}