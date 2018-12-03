#pragma once

#include <QtCore>
#include <QtWidgets>

namespace Qt {


	const QImage &QImageCache(const QString &name);

	const QIcon &QuadIcon(const QString &name,
						  const QString &tl, const QString &tr,
						  const QString &br, const QString &bl);
}