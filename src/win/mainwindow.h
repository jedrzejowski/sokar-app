#pragma once

#include <QMainWindow>
#include <gdcmImage.h>

namespace Ui {
    class MainWindow;
}

namespace Sokar {
    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

    protected:
        void initMenuBar();

    private:
        Ui::MainWindow *ui = nullptr;

    private slots:
        void selectFile();

    };
}
