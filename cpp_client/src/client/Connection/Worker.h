#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QtCore/QThread>

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include "client.h"

class Worker : public QObject {
Q_OBJECT

private:
    enum GameState{
        Draw,
        Guess,
        Other
    };

    GameState gameState;
    Client* client;

public:
    explicit Worker(Client* client, QObject* parent = 0);

signals:

    void statement(QString);

    void draw(QString);

    void valueChanged(QString);

    void solution(QString);

    void ready(QString);

    void receiveMessage(QString);

    void updateScene(QByteArray);

    void finished();

    void throwException(QString message);

    void tableCreated(QString);

    void listTable(QString);

    void abort(QString);

public slots:

    void reader();

    void writer();

    void sendFrame(QByteArray byteArray);

    void sendTextMessage(QString message);

    void sendRequest(QString request);
};

#endif // WINDOW_H