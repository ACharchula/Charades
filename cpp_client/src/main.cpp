#include <QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include "client/UI/MainWindow.h"
#include "client/Controller/Controller.h"

int main(int argc, char** argv) {

    QApplication a(argc, argv);

    Controller controller;

    return a.exec();
}