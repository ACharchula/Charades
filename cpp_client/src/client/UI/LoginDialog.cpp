//
// Created by adam on 17.05.19.
//

#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget* parent) {
    dialog = new QDialog;
    dialog->setWindowTitle("Login");
    dialog->resize(300, 100);

    description = new QLabel("Login:");
    input = new QLineEdit();
    accept = new QPushButton("Ok");
    layout = new QVBoxLayout;
    layout->addWidget(description);
    layout->addWidget(input);
    layout->addWidget(accept);

    dialog->setLayout(layout);
    dialog->show();

    connect(accept, SIGNAL(clicked()), this, SLOT(login()));
    connect(dialog, SIGNAL(rejected()), this, SLOT(exit()));
}

void LoginDialog::login() {
    dialog->hide();
    emit login(input->text());
}

void LoginDialog::exit() {
    emit close();
}