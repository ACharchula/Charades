#include <QTimer>
#include <QThread>

#include <QDebug>
#include "Worker.h"
#include "client.h"

const int lineFeed = 10;
const size_t MAXMESSAGESIZE = 1000 * 1000 - 20;
extern const std::string TEXT;

Worker::Worker(Client* client, QObject* parent) : client(client),
                                                  QObject(parent) {}

void Worker::doMethod1() {
    qDebug() << "Starting Method1 in Thread " << thread()->currentThreadId();
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
//        emit valueChanged();
    }
}

void Worker::doMethod2() {
    qDebug() << "Starting Method2 in Thread " << thread()->currentThreadId();
    std::pair<Message*, Message*> data;
    forever {
        data = client->receive();
        data.first->print();
        if (data.second != nullptr) {
            if(data.first->equal(SET) || data.first->equal(UPDATE)){
                std::cout << data.second->getValue().size() << std::endl;
                emit valueChanged(data.second->getValue());
            }
            delete data.second;
        }
        delete data.first;
    }
}