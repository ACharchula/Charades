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

    void finished();

public slots:

    void doMethod1();

    void doMethod2();

    void doMethod3();

private:
    void saveToFile(const std::string& data);
    void sendNextFrame();
};

#endif // WINDOW_H