#include <QTimer>
#include <QThread>

#include <QDebug>
#include <fstream>
#include "Worker.h"
#include "client.h"

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
        client->send(message, TEXT);
    }
}

void Worker::reader() {
    std::pair<Message*, Message*> data;
    forever {
        data = client->receive();
        data.first->print();

        if (data.first->equal(WELCOME)){

        } else if (data.first->equal(CHAT)){
            QString message = QString::fromStdString(data.second->getTextMessage());
            emit receiveMessage(message);
        } else if (data.first->equal(WAIT)){

        } else if (data.first->equal(END)){

        } else if (data.first->equal(READY)){

        } else if (data.first->equal(DRAW)){
            emit draw(QString::fromStdString(data.second->getValue()));
        } else if (data.first->equal(CORRECT)){
            emit statement(QString::fromStdString(CORRECT));
        } else if (data.first->equal(INCORRECT)){
            emit statement(QString::fromStdString(INCORRECT));
        } else if (data.first->equal(SET) || data.first->equal(UPDATE)){
            emit updateScene(QByteArray(data.second->getValue().data(), int(data.second->getValue().size())));
        }

        if(data.first->equal(SET) || data.first->equal(UPDATE) || data.first->equal(DRAW) || data.first->equal(CHAT) || data.first->equal(END) || data.first->equal(READY)){
            delete data.second;
        }

        delete data.first;
    }
}

void Worker::sendFrame(QByteArray byteArray){
    std::string data(byteArray.constData(), byteArray.length());
    client->send(data, SET);
}

void Worker::sendTextMessage(QString message) {
    std::string data = message.toStdString();
    client->send(data, TEXT);
}