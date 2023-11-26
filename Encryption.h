//
// Created by vaia on 11/25/23.
//

#ifndef PASSKEEP_ENCRYPTION_H
#define PASSKEEP_ENCRYPTION_H

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QDir>
#include <QString>
#include <QFileInfoList>
#include <QCryptographicHash>
#include <QProcess>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>

class Encryption {
public:
    Encryption();
     static QString search(const QString &directory = QDir::currentPath());
     void onDecrypt(const QString&, const QString&);
     void onDecryptMaster(const QString&, const QString&);
     static void onEncrypt(const QString&, const QString&);
     static void onCreateMaster();
     void onEncryptLogin();
     QJsonArray logins;
     QFile *loginFile{};
     QString loginKey;

protected:
    QString masterKey;
};


#endif //PASSKEEP_ENCRYPTION_H
