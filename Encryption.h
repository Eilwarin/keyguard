//
// Created by vaia on 11/25/23.
//

#ifndef PASSKEEP_ENCRYPTION_H
#define PASSKEEP_ENCRYPTION_H

#include <QString>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QProcess>

class Encryption {
public:
    Encryption();
     void onDecrypt(const QString&);
     void onEncrypt(const QString&);
     void onCreateLogins();
     void runPythonScript(const QString&, const QString&);
     QString loginKey;
};


#endif //PASSKEEP_ENCRYPTION_H
