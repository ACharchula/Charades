//
// Created by adam on 17.05.19.
//

#ifndef CPP_CLIENT_CHANGETABLE_H
#define CPP_CLIENT_CHANGETABLE_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

class ChangeTableDialog : public QDialog {
Q_OBJECT

public:
    ChangeTableDialog(QWidget* parent = 0);

public:
    void show();

private:
    QDialog* dialog;
    QLabel* description;
    QLineEdit* input;
    QVBoxLayout* layout;
    QPushButton* accept;

private slots:
    void changeTable();

signals:
    void change(QString);
};


#endif //CPP_CLIENT_CHANGETABLE_H
