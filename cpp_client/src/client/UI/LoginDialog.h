//
// Created by adam on 17.05.19.
//

#ifndef CPP_CLIENT_LOGINDIALOG_H
#define CPP_CLIENT_LOGINDIALOG_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

class LoginDialog : public QDialog {
Q_OBJECT

public:
    explicit LoginDialog(QWidget* parent = 0);

private:
    QDialog* dialog;
    QLabel* description;
    QLineEdit* input;
    QVBoxLayout* layout;
    QPushButton* accept;

private slots:

    void login();

    void exit();

signals:

    void login(QString);

    void close();
};


#endif //CPP_CLIENT_LOGINDIALOG_H
