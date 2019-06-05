#include <utility>

//
// Created by adam on 13.04.19.
//

#include "MainWindow.h"
#include <QDebug>
#include <QTimer>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

}

void MainWindow::updateScene(QByteArray byteArray) {
    drawScene.updateScene(std::move(byteArray));
}

QString MainWindow::getTextMessage() {
    return textArea->text();
}

void MainWindow::prepareUI() {
    drawView = new DrawView(0, 0);
    drawView->setScene(&drawScene);

    layout = new QHBoxLayout;
    leftVBox = new QVBoxLayout;
    rightVBox = new QVBoxLayout;
    layout->addLayout(leftVBox);
    layout->addLayout(rightVBox);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle("Test");

    textArea = new QLineEdit;
    list = new QListWidget;
    giveUp = new QPushButton("Give up.");
    clue = new QLabel;
    giveUp->setVisible(false);
    clue->setVisible(false);

    tableList = new QComboBox();
    select = new QPushButton("Select");
    loadTables = new QPushButton("Load table list");
    create = new QPushButton("Create new table");
    statistics = new QPushButton("Show stats");
    changeBox = new QHBoxLayout;

    changeBox->addWidget(tableList);
    changeBox->addWidget(select);
    changeBox->addWidget(loadTables);
    changeBox->addWidget(create);
    changeBox->addWidget(statistics);

    leftVBox->addWidget(clue);
    leftVBox->addWidget(giveUp);
    leftVBox->addWidget(drawView);
    rightVBox->addLayout(changeBox);
    rightVBox->addWidget(list);
    rightVBox->addWidget(textArea);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.4);

    connect(select, SIGNAL(clicked()), this, SLOT(changeTable()));
    connect(create, SIGNAL(clicked()), this, SLOT(createTable()));
    connect(loadTables, SIGNAL(clicked()), this, SLOT(loadTable()));
    connect(statistics, SIGNAL(clicked()), this, SLOT(getStats()));
}

void MainWindow::addChatMessage(QString message) {
    auto* msg = new QListWidgetItem(message);
    list->addItem(msg);
    list->scrollToBottom();
    list->update();
}

void MainWindow::addPlayerChatMessage(QString message) {
    addChatMessage(std::move(message));
    textArea->clear();
}

void MainWindow::draw(QString word) {
    clue->setText(word);
    clue->show();
    giveUp->show();
    textArea->hide();
    drawScene.setDraw(true);
}

void MainWindow::guess() {
    clue->hide();
    giveUp->hide();
    textArea->show();
    drawScene.setDraw(false);
}

void MainWindow::clearChat() {
    list->clear();
}

void MainWindow::clearTextArea() {
    textArea->clear();
}

QByteArray MainWindow::getScene() {
    return drawScene.getScene();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    emit close();
}

void MainWindow::changeTable() {
    emit change(tableList->currentText());
}

void MainWindow::createTable() {
    emit newTable();
}

void MainWindow::loadTable() {
    emit load();
}

void MainWindow::getStats() {
    emit stats();
}

void MainWindow::addTablesToList(QString next) {
    tableList->clear();
    QString temp;
    for (auto i : next) {
        if (i != '\n')
            temp.push_back(i);
        else {
            tableList->addItem(temp);
            temp = "";
        }
    }

    tableList->update();
}

void MainWindow::addStats(QString next) {
    QString user;
    QString val;
    bool loop = false;
    addChatMessage("***************STATS***************");
    for (auto i : next) {
        if (i != '\n')
            !loop ? user.push_back(i) : val.push_back(i);
        else {
           if(loop){
               addChatMessage("--" + user + " -> " + val);
               loop = false;
               user.clear();
               val.clear();
           } else
               loop = true;
        }
    }
    addChatMessage("***************STATS***************");
}