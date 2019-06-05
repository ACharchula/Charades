//
// Created by adam on 26.05.19.
//

#include "MessageBox.h"

MessageBox::MessageBox(QWidget* parent, QString text, QString informativeText) {

    msgBox = new QMessageBox(parent);
    msgBox->setText(text);
    msgBox->setInformativeText(informativeText);
    msgBox->exec();
}
