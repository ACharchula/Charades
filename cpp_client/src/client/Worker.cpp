#include <QTimer>
#include <QThread>

#include <QDebug>
#include "Worker.h"
#include "client.h"
#include "Message.h"


Worker::Worker(Client* client, QObject* parent) : client(client),
        QObject(parent) {}

void Worker::doMethod1() {
    qDebug() << "Starting Method1 in Thread " << thread()->currentThreadId();

    char* message = new char[MAXMESSAGESIZE];
    forever {
        std::cin >> message;
        client->send(message);
        emit valueChanged(QString::number(543543534));
    }
}

void Worker::doMethod2() {
    qDebug() << "Starting Method2 in Thread " << thread()->currentThreadId();

    std::pair<Message*, Message*> data;
    forever {
        data = client->receive();
        delete data.first;
//        delete data.second;
    }
}