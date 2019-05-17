//
// Created by adam on 17.05.19.
//

#include "ChangeTableDialog.h"

ChangeTableDialog::ChangeTableDialog(QWidget* parent){
    dialog = new QDialog(parent);
    dialog->setWindowTitle("ChangeTableDialog");
    dialog->resize(300,100);

    description = new QLabel("Table no.:");
    input = new QLineEdit();
    accept = new QPushButton("Ok");
    layout = new QVBoxLayout;
    layout->addWidget(description);
    layout->addWidget(input);
    layout->addWidget(accept);

    dialog->setLayout(layout);
    dialog->show();

    connect(accept, SIGNAL(clicked()), this, SLOT(changeTable()));
}

void ChangeTableDialog::changeTable() {
    dialog->hide();
    emit change(input->text());
}

void ChangeTableDialog::show() {
    dialog->show();
}
