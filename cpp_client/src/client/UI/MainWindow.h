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
#include "../Backend/Worker.h"
#include "DrawView.h"
#include "DrawScene.h"
#include "LoginDialog.h"
#include "ChangeTableDialog.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    enum GameState{
        Draw,
        Guess
    };

    GameState gameState;
    Client* client;
    LoginDialog* loginDialog;
    ChangeTableDialog* changeTableDialog = nullptr;
    std::string userName;

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
    QTimer *timer;

    QThread* threadR = nullptr;
    QThread* threadW = nullptr;
    Worker* workerR = nullptr;
    Worker* workerW = nullptr;

private:
    void connectToServer();
    void prepareUI();
    void prepareThreads();
    void connectAllSignalsAndSlots();

signals:
    void sendFrame(QByteArray);
    void sendMessage(QString);

private slots:
    void draw(QString word);
    void analyseStatement(QString state);
    void updateScene(QByteArray byteArray);
    void changeTableReleased();
    void giveUpReleased();
    void sendTextMessage();
    void receiveTextMessage(QString message);
    void login(QString nick);
    void closeApp();
    void sendFrame();
    void changeTable(QString newTable);
    void solution(QString info);
    void ready(QString info);
};

#endif //CPP_CLIENT_MAINWINDOW_H
