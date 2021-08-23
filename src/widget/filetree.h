#pragma once

#include <QtCore>
#include <QtWidgets>

namespace Sokar {

    class FileTree : public QTreeView {
    Q_OBJECT
        QFileSystemModel model;
        QMenu menu;
    public:
        FileTree(QWidget *parent = nullptr);
    protected:
        void contextMenuEvent(QContextMenuEvent *event) override;
    signals:
        void fileSelected(QString path);
    };
}