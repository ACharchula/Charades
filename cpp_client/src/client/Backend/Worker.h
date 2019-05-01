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
    Client* client;

public:
    explicit Worker(Client* client, QObject* parent = 0);

signals:

    void valueChanged(const QString& value);

    void finished();

public slots:

    void doMethod1();

    void doMethod2();
};

#endif // WINDOW_H