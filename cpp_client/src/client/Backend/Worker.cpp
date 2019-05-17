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

void Worker::doMethod1() {
//    qDebug() << "Starting Method1 in Thread " << thread()->currentThreadId();
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

void Worker::doMethod2() {
//    qDebug() << "Starting Method2 in Thread " << thread()->currentThreadId();
    std::pair<Message*, Message*> data;
    forever {
        data = client->receive();
        if (data.second != nullptr) {
            if(data.first->equal(SET) || data.first->equal(UPDATE)){
                emit valueChangedV2(QByteArray(data.second->getValue().data(), int(data.second->getValue().size())));
            } else if(data.first->equal(DRAW)){
                gameState = Draw;
                emit valueChanged(QString::fromStdString(DRAW));
            }
            else if(data.first->equal(END))
                gameState = Guess;
            delete data.second;
        }
        delete data.first;
    }
}

void Worker::doMethod3(QByteArray byteArray){
    std::string data(byteArray.constData(), byteArray.length());
    client->send(data, SET);
}