//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MAINWINDOW_H
#define CPP_CLIENT_MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include "../Backend/Worker.h"
#include "DrawView.h"
#include "DrawScene.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    DrawView* drawView;
    DrawScene drawScene;
    QTimer *timer;

    QThread* threadR;
    QThread* threadW;
    Worker* workerR;
    Worker* workerW;

signals:
    void sendNextFrame();

private slots:
    void method(QString);
    void update();
};

#endif //CPP_CLIENT_MAINWINDOW_H
