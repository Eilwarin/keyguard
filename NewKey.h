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
#include <QDate>

class NewKey : public QWidget{


public:
    explicit NewKey(QWidget *parent = nullptr);
    QLineEdit *url;
    QLineEdit *username;
    QLineEdit *key;

public slots:
    void onButtonClick() const;
    void onGenerateClick() const;
    void onGoBack();
    void onShowKey();
    void onCopyToClipboard() const;

private:
    QLabel *urlLabel;
    QLabel *usernameLabel;
    QLabel *keyLabel;

    QPushButton *getTextFields;
    QPushButton *generateKey;
    QPushButton *backButton;
    QPushButton *showKeyButton;
    QPushButton *copyToClipboard;
    bool showing = false;
    static QString onGenerateString();
};


#endif //PASSKEEP_NewKey_H
