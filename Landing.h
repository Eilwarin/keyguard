//
// Created by vaia on 11/25/23.
//

#ifndef PASSKEEP_LANDING_H
#define PASSKEEP_LANDING_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "NewKey.h"  // Include the header file for NewKey
#include "OpenKeys.h"
#include "Encryption.h"

class Landing : public QWidget, public Encryption{
public:
    explicit Landing(QWidget *parent = nullptr);
private slots:
    void onAddButtonClick();
    void onOpenButtonClick();
private:
    QPushButton *addButton;
    QPushButton *openButton;
    QLabel *addLabel;
    QLabel *openLabel;
};


#endif //PASSKEEP_LANDING_H
