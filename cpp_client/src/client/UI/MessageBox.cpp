//
// Created by adam on 26.05.19.
//

#include "MessageBox.h"
#include <QDebug>
#include <QtWidgets/QAbstractButton>

MessageBox::MessageBox(QWidget* parent, QString text, QString informativeText) {

    msgBox = new QMessageBox(parent);
    msgBox->setText(text);
    msgBox->setInformativeText(informativeText);
//    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//    msgBox->setDefaultButton(QMessageBox::Ok);
//    msgBox->setButtonText(QMessageBox::Yes, trUtf8("Repair"));
//    msgBox->setButtonText(QMessageBox::No, trUtf8("Close application"));
    msgBox->exec();
}
