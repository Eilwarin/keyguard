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
    static void onDecrypt(const QString&, const QString&);
    static void onDecryptMaster(const QString&, const QString&);
    static void onEncrypt(const QString&, const QString&);
    static void onCreateMaster();
    static void onSelectMaster();
    static QJsonArray logins;

private:
    static QString fileName;
    static QString masterKey;
    static QString loginKey;
};


#endif //PASSKEEP_ENCRYPTION_H
