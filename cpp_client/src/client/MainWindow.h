//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MAINWINDOW_H
#define CPP_CLIENT_MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include "Worker.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

private:
    QThread* threadR;
    QThread* threadW;
    Worker* workerR;
    Worker* workerW;

private slots:

    void method(const QString& value);
};

#endif //CPP_CLIENT_MAINWINDOW_H
