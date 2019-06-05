//
// Created by adam on 25.05.19.
//

#ifndef CPP_CLIENT_CONTROLLER_H
#define CPP_CLIENT_CONTROLLER_H


#include <QtWidgets/QWidget>
#include "../UI/MainWindow.h"

enum GameState{
    Draw,
    Guess
};

class Controller : public QWidget{
    Q_OBJECT

public:
    explicit Controller(QWidget* parent = 0, const Qt::WindowFlags& f = 0);

    ~Controller() override;

private:
    MainWindow* mainWindow;
    LoginDialog* loginDialog;

    GameState gameState;

    Client* client;
    std::string userName;
    bool sitAtTable = false;

    QTimer *timer;

    QThread* threadR = nullptr;
    QThread* threadW = nullptr;
    Worker* workerR = nullptr;
    Worker* workerW = nullptr;

private:
    bool connectToServer();
    void prepareThreads();
    void connectAllSignalsAndSlots();

signals:
    void sendFrame(QByteArray);
    void sendMessage(QString);
    void sendRequest(QString);
    void enterTable(QString);
    void addTables(QString);
    void errorStatus(QString);

private slots:
    void draw(QString word);
    void analyseStatement(QString state);
    void updateScene(QByteArray byteArray);
    void changeTable(QString table);
    void giveUpReleased();
    void sendTextMessage();
    void receiveTextMessage(QString message);
    void sendFrame();
    void solution(QString info);
    void ready(QString info);
    void catchException(QString info);
    void tableCreated(QString);
    void exit();
    void stats(QString stats);
    void tableList(QString list);
    void createTable();
    void loadTable();

    void login(QString nick);
    void closeApp();
};
#endif //CPP_CLIENT_CONTROLLER_H
