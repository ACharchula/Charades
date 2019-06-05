//
// Created by adam on 25.05.19.
//

#include <utility>
#include "Controller.h"
#include "../UI/MessageBox.h"
#include "../Consts.h"
#include <QTimer>
#include <QtCore/QCoreApplication>
#include <QDebug>

Controller::Controller(QWidget* parent, const Qt::WindowFlags& f) : QWidget(parent, f) {
    mainWindow = new MainWindow();
    auto loginDialog = new LoginDialog();
    connect(loginDialog, SIGNAL(login(QString)), this, SLOT(login(QString)));
    connect(loginDialog, SIGNAL(close()), this, SLOT(close()));
}

Controller::~Controller() {
    if (threadR != nullptr) {
        threadR->wait();
        threadW->wait();
    }

    delete threadR;
    delete workerR;
    delete threadW;
    delete workerW;
}

bool Controller::connectToServer() {
    client = new Client(userName);
    MessageBox* msg;

    try {
        client->run("localhost", 44444);
    } catch (const std::runtime_error& error) {
        QString info = QString::fromStdString(error.what());
        msg = new MessageBox(this, info, "Try restart application.");
        return false;
    }

    return true;
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
    connect(this, SIGNAL(enterTable(QString)), workerW, SLOT(enterTable(QString)), Qt::DirectConnection);

    connect(timer, SIGNAL(timeout()), this, SLOT(sendFrame()));

    connect(this, SIGNAL(sendMessage(QString)), workerW, SLOT(sendTextMessage(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(receiveMessage(QString)), this, SLOT(receiveTextMessage(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(solution(QString)), this, SLOT(solution(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(ready(QString)), this, SLOT(ready(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(throwException(QString)), this, SLOT(catchException(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(tableCreated(QString)), this, SLOT(tableCreated(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(stats(QString)), this, SLOT(stats(QString)), Qt::DirectConnection);
    connect(workerW, SIGNAL(tableList(QString)), this, SLOT(tableList(QString)), Qt::DirectConnection);

    connect(this, SIGNAL(errorStatus(QString)), workerW, SLOT(changeErrorStatus(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(errorStatus(QString)), workerR, SLOT(changeErrorStatus(QString)), Qt::DirectConnection);

    connect(mainWindow->giveUp, SIGNAL (released()), this, SLOT (giveUpReleased()));
    connect(mainWindow->textArea, SIGNAL(returnPressed()), this, SLOT(sendTextMessage()), Qt::DirectConnection);
    connect(mainWindow, SIGNAL(close()), this, SLOT(exit()), Qt::DirectConnection);

    connect(mainWindow, SIGNAL(change(QString)), this, SLOT(changeTable(QString)), Qt::DirectConnection);
    connect(mainWindow, SIGNAL(newTable()), this, SLOT(createTable()), Qt::DirectConnection);
    connect(mainWindow, SIGNAL(load()), this, SLOT(loadTable()), Qt::DirectConnection);
    connect(mainWindow, SIGNAL(stats()), this, SLOT(getStats()), Qt::DirectConnection);
    connect(this, SIGNAL(addTables(QString)), mainWindow, SLOT(addTablesToList(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(addStats(QString)), mainWindow, SLOT(addStats(QString)), Qt::DirectConnection);
}

void Controller::draw(QString word) {
    mainWindow->addChatMessage("LOG: You are drawer.");
    mainWindow->draw(std::move(word));
    gameState = GameState::Draw;
}

void Controller::analyseStatement(QString state) {
    std::string statement = state.toStdString();
     if (statement == WAIT) {
        mainWindow->addChatMessage("LOG: You need to wait for other players.");
    } else if (statement == PING) {
        sendRequest(QString::fromStdString(PONG));
    }
}

void Controller::updateScene(QByteArray byteArray) {
    mainWindow->updateScene(std::move(byteArray));
}

void Controller::changeTable(QString table) {
    if (table != QString::fromStdString("")) {
        if (sitAtTable)
            emit sendRequest(QString::fromStdString(LEAVE));
        sitAtTable = true;
        gameState = Guess;
        emit enterTable(table);
        mainWindow->clearChat();
    }
}

void Controller::loadTable() {
    emit sendRequest(QString::fromStdString(TABLEREQUEST));
}

void Controller::giveUpReleased() {
    gameState = GameState::Guess;
    mainWindow->guess();
    sendRequest(QString::fromStdString(GIVEUP));
}

void Controller::sendTextMessage() {
    if (sitAtTable) {
        auto message = mainWindow->getTextMessage();
        auto chatMessage = QString::fromStdString(std::string(userName).append(": ").append(message.toStdString()));
        mainWindow->addPlayerChatMessage(chatMessage);
        emit sendMessage(message);
    } else {
        mainWindow->clearTextArea();
        mainWindow->addChatMessage("LOG: You need to sit at table to use chat.");
    }
}

void Controller::login(QString nick) {
    userName = nick.toStdString();
    if (connectToServer()) {
        mainWindow->prepareUI();
        prepareThreads();
        connectAllSignalsAndSlots();
        mainWindow->show();
    }

void Controller::sendFrame() {
    if (gameState == GameState::Draw) {
        auto byteArray = mainWindow->getScene();
        emit sendFrame(byteArray);
    }
}

void Controller::solution(QString info) {
    mainWindow->addChatMessage(std::move(info));
    if (gameState == GameState::Draw) {
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
    emit errorStatus("error");
    MessageBox* msg = new MessageBox(this, info, "Try restar application.");
}

void Controller::tableCreated(QString table) {
    emit enterTable(std::move(table));
    sitAtTable = true;
    mainWindow->clearChat();
}

void Controller::stats(QString stats) {
    emit addStats(std::move(stats));
}

void Controller::tableList(QString list) {
    emit addTables(std::move(list));
}

void Controller::getStats() {
    emit sendRequest(QString::fromStdString(GETSTATS));
}

void Controller::exit() {

}

void Controller::createTable() {
    emit sendRequest(QString::fromStdString(CREATETABLE));
}
