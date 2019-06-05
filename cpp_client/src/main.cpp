#include <QApplication>
#include "client/Controller/Controller.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    Controller controller;

    return a.exec();
}