#include <QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include "client/MainWindow.h"

int main(int argc, char** argv) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}