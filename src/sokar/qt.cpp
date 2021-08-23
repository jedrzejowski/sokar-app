
#include "qt.h"
#include "exception.h"

const QImage &Qt::QImageCache(const QString &name) {

    static QHash<QString, QImage *> cache;

    if (cache.contains(name)) return *cache[name];

    cache[name] = new QImage(name);

    return *cache[name];
}

const QIcon &Qt::QuadIcon(const QString &name,
                          const QString &tl, const QString &tr,
                          const QString &br, const QString &bl) {

    static QHash<QString, QIcon *> cache;

    auto key = name + tl + tr + br + bl;

    if (cache.contains(key)) return *cache[key];

    auto base = QImageCache(":/img/ico/" + name);
    QPainter painter(&base);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    auto h2 = base.height() / 2;
    auto w2 = base.width() / 2;

    if (!tl.isEmpty())
        painter.drawImage(
                QRect(0, 0, w2, h2),
                QImageCache(":/img/ico/" + tl));

    if (!tr.isEmpty())
        painter.drawImage(
                QRect(0, h2, w2, h2),
                QImageCache(":/img/ico/" + tr));

    if (!tl.isEmpty())
        painter.drawImage(
                QRect(w2, h2, w2, h2),
                QImageCache(":/img/ico/" + tl));

    if (!tl.isEmpty())
        painter.drawImage(
                QRect(w2, 0, w2, h2),
                QImageCache(":/img/ico/" + tl));


    cache[key] = new QIcon(QPixmap::fromImage(base));

    return *cache[key];
}
