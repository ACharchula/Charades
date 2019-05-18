//
// Created by adam on 13.04.19.
//

#include "MainWindow.h"
#include "DrawView.h"
#include "LoginDialog.h"
#include "ChangeTableDialog.h"
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

//const char* userName = "zetraxus";

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent) {

    LoginDialog* loginDialog = new LoginDialog();
    connect(loginDialog, SIGNAL(login(QString)), this, SLOT(login(QString)));
    connect(loginDialog, SIGNAL(close()), this, SLOT(closeApp()));
}

MainWindow::~MainWindow() {
    if(threadR != nullptr){
        threadR->wait();
        threadW->wait();
    }

    delete threadR;
    delete workerR;
    delete threadW;
    delete workerW;
}

void MainWindow::analyseStatement(QString state) {
    if (state == QString::fromStdString(CORRECT)) {
        QString message = "LOG: CORRECT CLUE!";
        auto* msg = new QListWidgetItem(message);
        list->addItem(msg);
    } else if (state == QString::fromStdString(INCORRECT)) {
        //TODO
    }
}

void MainWindow::draw(QString word){
    timer->start(100);
    clue->setText(word);
    clue->show();
}

void MainWindow::updateScene(QByteArray byteArray) {
    drawScene.updateScene(byteArray);
}

void MainWindow::sendFrame() {
    QByteArray byteArray = drawScene.getScene();
    emit sendFrame(byteArray);
}

void MainWindow::sendTextMessage() {
    QString message = textArea->text();
    QString messageToShow = QString::fromStdString(std::string(userName).append(": ").append(message.toStdString()));
    textArea->clear();
    list->addItem(messageToShow);
    emit sendMessage(message);
}

void MainWindow::receiveTextMessage(QString message) {
    auto* msg = new QListWidgetItem(message);
    list->addItem(msg);
}

void MainWindow::changeTableReleased() {
    if (changeTableDialog == nullptr){
        changeTableDialog = new ChangeTableDialog(this);
        connect(changeTableDialog, SIGNAL(change(QString)), this, SLOT(changeTable(QString)), Qt::DirectConnection);
    }
}

void MainWindow::giveUpReleased() {
    //TODO
}

void MainWindow::login(QString nick) {
    userName = nick.toStdString();
    connectToServer();
    prepareUI();
    prepareThreads();
    connectAllSignalsAndSlots();
    show();
}

void MainWindow::closeApp() {
    //TODO
}

void MainWindow::changeTable(QString newTable){
    //TODO
    changeTableDialog = nullptr; // TODO FIX IT!!!
}

void MainWindow::connectToServer() {
    client = new Client(userName);
    client->run("localhost", 44444);
}

void MainWindow::prepareUI() {
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

    textArea = new QLineEdit;
    list = new QListWidget;
    changeTableButton = new QPushButton ("Change table.");
    giveUp = new QPushButton ("Give up.");
    clue = new QLabel;
    giveUp->setVisible(false);
    clue->setVisible(false);

    leftVBox->addWidget(clue);
    leftVBox->addWidget(giveUp);
    leftVBox->addWidget(drawView);
    rightVBox->addWidget(changeTableButton);
    rightVBox->addWidget(list);
    rightVBox->addWidget(textArea);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.4);
}

void MainWindow::prepareThreads() {
    threadR = new QThread();
    threadW = new QThread();

    workerR = new Worker(client);
    workerW = new Worker(client);

    workerR->moveToThread(threadR);
    connect(threadR, SIGNAL(started()), workerR, SLOT(writer()));
    connect(workerR, SIGNAL(finished()), threadR, SLOT(quit()), Qt::DirectConnection);

    workerW->moveToThread(threadW);
    connect(threadW, SIGNAL(started()), workerW, SLOT(reader()));
    connect(workerW, SIGNAL(finished()), threadW, SLOT(quit()), Qt::DirectConnection);

    threadR->start();
    threadW->start();

    timer = new QTimer(this);
}

void MainWindow::connectAllSignalsAndSlots() {
    connect(workerW, SIGNAL(statement(QString)), this, SLOT(analyseStatement(QString)));
    connect(workerW, SIGNAL(draw(QString)), this, SLOT(draw(QString)));
    connect(workerW, SIGNAL(updateScene(QByteArray)), this, SLOT(updateScene(QByteArray)));
    connect(this, SIGNAL(sendFrame(QByteArray)), workerW, SLOT(sendFrame(QByteArray)), Qt::DirectConnection);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendFrame()));

    connect(changeTableButton, SIGNAL (released()), this, SLOT (changeTableReleased()));
    connect(giveUp, SIGNAL (released()), this, SLOT (giveUpReleased()));
    connect(textArea, SIGNAL(returnPressed()), this, SLOT(sendTextMessage()), Qt::DirectConnection);
    connect(this, SIGNAL(sendMessage(QString)), workerW, SLOT(sendTextMessage(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(receiveMessage(QString)), this, SLOT(receiveTextMessage(QString)), Qt::DirectConnection);
}
