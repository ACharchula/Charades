#include <QTimer>
#include <QThread>

#include <QDebug>
#include <fstream>
#include "Worker.h"
#include "client.h"
#include "../Consts.h"

const int lineFeed = 10;
const size_t MAXMESSAGESIZE = 1000 * 1000 - 20;
extern const std::string TEXT;

Worker::Worker(Client* client, QObject* parent) : client(client), QObject(parent), gameState(Other) {}

void Worker::writer() {
    int nextChar;
    forever {
        std::string message;
        do {
            nextChar = getchar();
            message += static_cast<char>(nextChar);
        } while (nextChar != lineFeed && message.size() != MAXMESSAGESIZE);

        if (message.size() == MAXMESSAGESIZE)
            message += '\0';
        if (message[message.size() - 1] == lineFeed)
            message = message.substr(0, message.size() - 1);

        try{
            client->send(message, TEXT);
        }catch(const std::runtime_error& error){
            throwException(QString::fromStdString(error.what()));
        }
    }
}

void Worker::reader() {
    std::pair<std::unique_ptr<Message>, std::unique_ptr<Message>> data;
    forever {
        try{
            data = client->receive();
        }catch(const std::runtime_error& error){
            throwException(QString::fromStdString(error.what()));
        }
        if (data.first->equal(WELCOME)){
            emit statement(QString::fromStdString(WELCOME));
        } else if (data.first->equal(CHAT)){
            QString message = QString::fromStdString(data.second->getTextMessage());
            emit receiveMessage(message);
        } else if (data.first->equal(WAIT)){
            emit statement(QString::fromStdString(WAIT));
        } else if (data.first->equal(END)){
            QString message = QString::fromStdString(data.second->getWinnerTextMessage());
            emit solution(message);
        } else if (data.first->equal(READY)){
            if(data.second != nullptr)
                emit ready(QString::fromStdString(data.second->getValue()));
        } else if (data.first->equal(DRAW)){
            emit draw(QString::fromStdString(data.second->getValue()));
        } else if (data.first->equal(PING)){
            emit statement(QString::fromStdString(PING));
        } else if (data.first->equal(TABLECREATED)){
            emit tableCreated(QString::fromStdString(data.second->getValue()));
        } else if (data.first->equal(FAIL)){
            emit statement(QString::fromStdString(FAIL));
        } else if (data.first->equal(ABORT)){
            emit abort(QString::fromStdString(data.second->getValue()));
        } else if (data.first->equal(SET) || data.first->equal(UPDATE)){
            emit updateScene(QByteArray(data.second->getValue().data(), int(data.second->getValue().size())));
        }
    }
}

void Worker::sendFrame(QByteArray byteArray){
    std::string data(byteArray.constData(), static_cast<unsigned long>(byteArray.length()));
    client->send(data, SET);
}

void Worker::sendTextMessage(QString message) {
    std::string data = message.toStdString();
    client->send(data, TEXT);
}

void Worker::sendRequest(QString request) {
    std::string req = request.toStdString();
    client->send("", req);
}