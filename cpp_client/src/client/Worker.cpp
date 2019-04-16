#include <QTimer>
#include <QThread>

#include <QDebug>
#include "Worker.h"
#include "client.h"

const int lineFeed = 10;
const size_t MAXMESSAGESIZE = 9000;
extern const std::string TEXT;

Worker::Worker(Client* client, QObject* parent) : client(client),
                                                  QObject(parent) {}

void Worker::doMethod1() {
    qDebug() << "Starting Method1 in Thread " << thread()->currentThreadId();

    int nextChar;
    forever {
        std::string message;
        do{
            nextChar = getchar();
            message += static_cast<char>(nextChar);
        } while(nextChar != lineFeed && message.size() != MAXMESSAGESIZE);

        if (message.size() == MAXMESSAGESIZE)
            message +='\0';
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
        data.second->print();
        delete data.first;
        delete data.second;
    }
}