#include <QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include "client/UI/MainWindow.h"

int main(int argc, char** argv) {

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}