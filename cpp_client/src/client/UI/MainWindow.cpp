#include <utility>

//
// Created by adam on 13.04.19.
//

#include "MainWindow.h"
#include "DrawView.h"
#include "LoginDialog.h"
#include "ChangeTableDialog.h"
#include "../Consts.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {

}



void MainWindow::updateScene(QByteArray byteArray) {
    drawScene.updateScene(std::move(byteArray));
}

QString MainWindow::getTextMessage(){
    return textArea->text();
}

void MainWindow::prepareUI() {
    drawView = new DrawView (0,0);
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
    changeTableButton = new QPushButton ("Change table.");
    giveUp = new QPushButton ("Give up.");
    clue = new QLabel;
    giveUp->setVisible(false);
    clue->setVisible(false);

    leftVBox->addWidget(clue);
    leftVBox->addWidget(giveUp);
    leftVBox->addWidget(drawView);
    rightVBox->addWidget(changeTableButton);
    rightVBox->addWidget(list);
    rightVBox->addWidget(textArea);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.4);
}

void MainWindow::addChatMessage(QString message) {
    auto *msg  = new QListWidgetItem(message);
    list->addItem(msg);
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

QByteArray MainWindow::getScene() {
    return drawScene.getScene();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    auto resBtn = QMessageBox::question(this, "Exit", tr("Are you sure?"), QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if (resBtn == QMessageBox::Yes)
        emit close();
}