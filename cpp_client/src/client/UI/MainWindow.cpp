//
// Created by adam on 13.04.19.
//

#include "MainWindow.h"
#include "DrawView.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidgetItem>

const char* userName = "zetraxus";

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent) {

    //connect
    Client* client = new Client(userName);
    client->run("localhost", 44444);

    //prepare ui
    drawView = new DrawView (0,0);
    drawView->setScene(&drawScene);

    layout = new QHBoxLayout;
    leftVBox = new QVBoxLayout;
    rightVBox = new QVBoxLayout;
    layout->addLayout(leftVBox);
    layout->addLayout(rightVBox);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle("Test");

    lineEdit = new QLineEdit;
    list = new QListWidget;
    changeTable = new QPushButton ("Change table.");
    giveUp = new QPushButton ("Give up.");
    clue = new QLabel;
    giveUp->setVisible(false);
    clue->setVisible(false);

    leftVBox->addWidget(clue);
    leftVBox->addWidget(giveUp);
    leftVBox->addWidget(drawView);
    rightVBox->addWidget(changeTable);
    rightVBox->addWidget(list);
    rightVBox->addWidget(lineEdit);

    resize(QDesktopWidget().availableGeometry(this).size() * 0.4);

    //run sending/receiving data threads
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

    connect(workerW, SIGNAL(valueChanged(QString)), this, SLOT(method(QString)));
    connect(workerW, SIGNAL(valueChangedV2(QByteArray)), this, SLOT(method2(QByteArray)));
    connect(this, SIGNAL(sendNextFrame(QByteArray)), workerW, SLOT(doMethod3(QByteArray)), Qt::DirectConnection);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow() {
    threadR->wait();
    threadW->wait();

    delete threadR;
    delete workerR;
    delete threadW;
    delete workerW;
}

void MainWindow::method(QString type) {
    if(type == QString::fromStdString(DRAW))
        timer->start(250);
}

void MainWindow::method2(QByteArray byteArray) {
    drawScene.updateScene(byteArray);
}

void MainWindow::update() {
    QByteArray byteArray = drawScene.getScene();
    emit sendNextFrame(byteArray);
}

