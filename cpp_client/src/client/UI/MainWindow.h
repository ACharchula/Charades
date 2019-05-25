//
// Created by adam on 13.04.19.
//

#ifndef CPP_CLIENT_MAINWINDOW_H
#define CPP_CLIENT_MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include "../Connection/Worker.h"
#include "DrawView.h"
#include "DrawScene.h"
#include "LoginDialog.h"
#include "ChangeTableDialog.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

public:
    ChangeTableDialog* changeTableDialog = nullptr;

    QHBoxLayout* layout;
    QVBoxLayout* leftVBox;
    QVBoxLayout* rightVBox;
    QLineEdit* textArea;
    QListWidget* list;
    QPushButton* changeTableButton;
    QPushButton* giveUp;
    QLabel* clue;
    DrawView* drawView;
    DrawScene drawScene;

public:
    void addChatMessage(QString message);
    void addPlayerChatMessage(QString message);
    void draw(QString word);
    void guess();
    void updateScene(QByteArray byteArray);

    QString getTextMessage();
    QByteArray getScene();

    void prepareUI();
};

#endif //CPP_CLIENT_MAINWINDOW_H
