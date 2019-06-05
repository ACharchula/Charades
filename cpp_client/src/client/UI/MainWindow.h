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
#include <QtWidgets/QComboBox>
#include "../Connection/Worker.h"
#include "DrawView.h"
#include "DrawScene.h"
#include "LoginDialog.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

public:
    QHBoxLayout* layout;
    QVBoxLayout* leftVBox;
    QVBoxLayout* rightVBox;
    QHBoxLayout* changeBox;
    QLineEdit* textArea;
    QListWidget* list;
    QPushButton* giveUp;
    QLabel* clue;
    DrawView* drawView;
    DrawScene drawScene;

    QPushButton* create;
    QPushButton* select;
    QPushButton* loadTables;
    QComboBox* tableList;

public:
    void addChatMessage(QString message);

    void addPlayerChatMessage(QString message);

    void draw(QString word);

    void guess();

    void updateScene(QByteArray byteArray);

    void clearChat();

    void clearTextArea();

    QString getTextMessage();

    QByteArray getScene();

    void prepareUI();

    void closeEvent(QCloseEvent* event) override;

signals:

    void close();

    void change(QString);

    void newTable();

    void load();

private slots:

    void changeTable();

    void createTable();

    void loadTable();

    void addTablesToList(QString);
};

#endif //CPP_CLIENT_MAINWINDOW_H