//
// Created by adam on 26.05.19.
//

#ifndef CPP_CLIENT_ERRORDIALOG_H
#define CPP_CLIENT_ERRORDIALOG_H


#include <QtWidgets/QMessageBox>

class MessageBox : public QMessageBox {
Q_OBJECT

public:
    explicit MessageBox (QWidget* parent = 0, QString text = "", QString informativeText = "");
    int getDecision();

private:
    QMessageBox* msgBox;
    int decision;
};

#endif //CPP_CLIENT_ERRORDIALOG_H
