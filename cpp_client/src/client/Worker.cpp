#include <QTimer>
#include <QThread>

#include <QDebug>
#include "Worker.h"
#include "client.h"


Worker::Worker(Client* client, QObject* parent) : client(client),
        QObject(parent) {}

void Worker::doMethod1() {
    qDebug() << "Starting Method1 in Thread " << thread()->currentThreadId();

    char* message = new char[MAXMESSAGESIZE];
    for (;;) {
        std::cin >> message;
        client->send(message);
        emit valueChanged(QString::number(543543534));
    }
}

void Worker::doMethod2() {
    qDebug() << "Starting Method2 in Thread " << thread()->currentThreadId();

    const char* message;
    for (;;) {
        message = client->receive();
        std::cout << message;
        delete message;
    }
}