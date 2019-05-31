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

    void valueChanged(QString);

    void receiveMessage(QString);

    void updateScene(QByteArray);

    void finished();

public slots:

    void reader();

    void writer();

    void sendFrame(QByteArray byteArray);

    void sendTextMessage(QString message);
};

#endif // WINDOW_H