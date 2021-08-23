#include "filetree.h"

using namespace Sokar;

FileTree::FileTree(QWidget *parent) : QTreeView(parent) {

    model.setRootPath("");
    model.setReadOnly(true);
    model.setResolveSymlinks(true);
    model.setFilter(QDir::AllDirs | QDir::AllEntries | QDir::NoDotAndDotDot);

    setModel(&model);

//	setSelectionMode(MultiSelection);

    setColumnHidden(1, true);
    setColumnHidden(2, true);
    setColumnHidden(3, true);

    {
        auto index = model.index(QDir::homePath());
        setCurrentIndex(index);
        expand(index);
        scrollTo(index, PositionAtTop);
    }

    connect(this, &FileTree::doubleClicked, [&](const QModelIndex &index) {
        auto path = model.filePath(index);

        if (QFileInfo(path).isFile())
            fileSelected(path);
    });
}
void FileTree::contextMenuEvent(QContextMenuEvent *event) {

    if (menu.isEmpty()) {
        auto open = new QAction("Otwórz", this);
        menu.addAction(open);
    }

    menu.exec(event->globalPos());
}
