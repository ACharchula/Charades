//
// Created by adam on 26.05.19.
//

#include "MessageBox.h"
#include <QDebug>

MessageBox::MessageBox(QWidget* parent, QString text, QString informativeText) {
    msgBox = new QMessageBox(parent);
    msgBox->setText(text);
    msgBox->setInformativeText(informativeText);
    msgBox->setStandardButtons(QMessageBox::Discard | QMessageBox::Ok);
    msgBox->setDefaultButton(QMessageBox::Ok);
    msgBox->resize(300,100);
    decision = msgBox->exec();
}

int MessageBox::getDecision() {
    return decision;
}
