//
// Created by vaia on 11/23/23.
//

#ifndef PASSKEEP_NewKey_H
#define PASSKEEP_NewKey_H

#include <QClipboard>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QFile>
#include <QJsonArray>

class NewKey : public QWidget{


public:
    explicit NewKey(QWidget *parent = nullptr);

public slots:
    void onButtonClick();
    void onGenerateClick();
    void onGoBack();
    void onShowKey();
    void onCopyToClipboard();

private:
    QLabel *urlLabel;
    QLabel *usernameLabel;
    QLabel *keyLabel;

    QLineEdit *url;
    QLineEdit *username;
    QLineEdit *key;

    QPushButton *getTextFields;
    QPushButton *generateKey;
    QPushButton *backButton;
    QPushButton *showKeyButton;
    QPushButton *copyToClipboard;
    bool showing = false;
    static QString onGenerateString();
};


#endif //PASSKEEP_NewKey_H
