//
// Created by adam on 13.04.19.
//

#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent) {

    Client* client = new Client();
    client->run("localhost", 44444);

    threadR = new QThread();
    threadW = new QThread();

    workerR = new Worker(client);
    workerW = new Worker(client);

    workerR->moveToThread(threadR);
    connect(threadR, SIGNAL(started()), workerR, SLOT(doMethod1()));
    connect(workerR, SIGNAL(finished()), threadR, SLOT(quit()), Qt::DirectConnection);

    workerW->moveToThread(threadW);
    connect(threadW, SIGNAL(started()), workerW, SLOT(doMethod2()));
    connect(workerW, SIGNAL(finished()), threadW, SLOT(quit()), Qt::DirectConnection);

    threadR->start();
    threadW->start();
    connect(workerR, SIGNAL(valueChanged(QString)), this, SLOT(method(QString)));
}

MainWindow::~MainWindow() {
    threadR->wait();
    threadW->wait();

    delete threadR;
    delete workerR;
    delete threadW;
    delete workerW;
}

void MainWindow::method(const QString& value) {
    qDebug() << value;
}