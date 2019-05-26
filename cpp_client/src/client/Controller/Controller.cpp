#include <utility>

#include <utility>

//
// Created by adam on 25.05.19.
//

#include "Controller.h"
#include "../Consts.h"
#include "../UI/MessageBox.h"
#include <QTimer>
#include <QDebug>
#include <QtCore/QCoreApplication>

Controller::Controller(QWidget* parent, const Qt::WindowFlags& f) : QWidget(parent, f) {
    mainWindow = new MainWindow();
    auto loginDialog = new LoginDialog();
    connect(loginDialog, SIGNAL(login(QString)), this, SLOT(login(QString)));
    connect(loginDialog, SIGNAL(close()), this, SLOT(close()));
}

Controller::~Controller() {
    if(threadR != nullptr){
        threadR->wait();
        threadW->wait();
    }

    delete threadR;
    delete workerR;
    delete threadW;
    delete workerW;
}

void Controller::connectToServer() {
    client = new Client(userName);
    MessageBox* msg;

    try{
        client->run("localhost", 44444);
    } catch (const std::runtime_error& error) {
        QString info = QString::fromStdString(error.what());

        if(error.what() == ERROROPEN){
            msg = new MessageBox(this, info, "txt");
        } else if(error.what() == ERRORUKNOWNSERVER){
            msg = new MessageBox(this, info, "txt");
        } else if(error.what() == ERRORCONNECT){
            msg = new MessageBox(this, info, "txt");
        } else if(error.what() == ERRORWRITING){
            msg = new MessageBox(this, info, "txt");
        }
    }
}

void Controller::prepareThreads() {
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

    timer = new QTimer();
    timer->start(100);
}

void Controller::connectAllSignalsAndSlots() {
    connect(workerW, SIGNAL(statement(QString)), this, SLOT(analyseStatement(QString)));
    connect(workerW, SIGNAL(draw(QString)), this, SLOT(draw(QString)));
    connect(workerW, SIGNAL(updateScene(QByteArray)), this, SLOT(updateScene(QByteArray)));
    connect(this, SIGNAL(sendFrame(QByteArray)), workerW, SLOT(sendFrame(QByteArray)), Qt::DirectConnection);
    connect(this, SIGNAL(sendRequest(QString)), workerW, SLOT(sendRequest(QString)), Qt::DirectConnection);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendFrame()));

    connect(this, SIGNAL(sendMessage(QString)), workerW, SLOT(sendTextMessage(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(receiveMessage(QString)), this, SLOT(receiveTextMessage(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(solution(QString)), this, SLOT(solution(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(ready(QString)), this, SLOT(ready(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(throwException(QString)), this, SLOT(catchException(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(tableCreated(QString)), this, SLOT(tableCreated(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(abort(QString)), this, SLOT(abort(QString)), Qt::DirectConnection);

    connect(mainWindow->changeTableButton, SIGNAL (released()), this, SLOT (changeTableReleased()));
    connect(mainWindow->giveUp, SIGNAL (released()), this, SLOT (giveUpReleased()));
    connect(mainWindow->textArea, SIGNAL(returnPressed()), this, SLOT(sendTextMessage()), Qt::DirectConnection);
    connect(mainWindow, SIGNAL(close()), this, SLOT(exit()), Qt::DirectConnection);
}

void Controller::draw(QString word) {
    mainWindow->addChatMessage("LOG: You are drawer.");
    mainWindow->draw(std::move(word));
    gameState = GameState::Draw;
}

void Controller::analyseStatement(QString state) {
    std::string statement = state.toStdString();
    if(statement == WELCOME){

    } else if(statement == WAIT){

    } else if(statement == PING){
        sendRequest(QString::fromStdString(PONG));
    } else if(statement == FAIL){

    }
}

void Controller::updateScene(QByteArray byteArray) {
    mainWindow->updateScene(std::move(byteArray));
}

void Controller::changeTableReleased() {
    if (mainWindow->changeTableDialog == nullptr){
        mainWindow->changeTableDialog = new ChangeTableDialog(this);
        connect(mainWindow->changeTableDialog, SIGNAL(change(QString)), this, SLOT(changeTable(QString)), Qt::DirectConnection);
    }
}

void Controller::changeTable(QString table) {
    mainWindow->changeTableDialog = nullptr; // TODO FIX IT!!!
}

void Controller::giveUpReleased() {
    // sendRequest(QString::fromStdString(GIVEUP)); // TODO uncomment when server will implement it
}

void Controller::sendTextMessage() {
    auto message = mainWindow->getTextMessage();
    auto chatMessage = QString::fromStdString(std::string(userName).append(": ").append(message.toStdString()));
    mainWindow->addPlayerChatMessage(chatMessage);
    emit sendMessage(message);
}

void Controller::login(QString nick) {
    userName = nick.toStdString();
    connectToServer();
    mainWindow->prepareUI();
    prepareThreads();
    connectAllSignalsAndSlots();
    mainWindow->show();
}

void Controller::closeApp() {
    close();
}

void Controller::sendFrame() {
    if(gameState == GameState::Draw){
        auto byteArray = mainWindow->getScene();
        emit sendFrame(byteArray);
    }
}

void Controller::solution(QString info) {
    mainWindow->addChatMessage(std::move(info));
    if(gameState == GameState::Draw){
        gameState = GameState::Guess;
        mainWindow->guess();
    }
}

void Controller::ready(QString info) {
    mainWindow->addChatMessage("LOG: New drawer is: " + info);
}

void Controller::receiveTextMessage(QString message) {
    mainWindow->addChatMessage(std::move(message));
}

void Controller::catchException(QString info) {
    std::string message = info.toStdString();

    MessageBox* msg;

    if(message == ERRORWRITING){
        msg = new MessageBox(this, info, "txt2");
    } else if(message == ERRORREADING){
        msg = new MessageBox(this, info, "txt2");
    } else if(message == ERRORREADING){
        msg = new MessageBox(this, info, "txt2");
    } else if(message == ERRORRECEAVING){
        msg = new MessageBox(this, info, "txt2");
    } else{
        QString unexpected = QString::fromStdString("Unexpected" + message);
        qDebug() << unexpected;
    }
}

void Controller::tableCreated(QString) {

}

void Controller::abort(QString) {

}

void Controller::exit() {
    qDebug() << "ciastko";
    QCoreApplication::quit();
    qDebug() << "ciastko2";
    qApp->quit();
//    QApplication::closeAllWindows();
//    QCoreApplication::quit()
}


