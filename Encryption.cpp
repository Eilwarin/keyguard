//
// Created by vaia on 11/25/23.
//


#include "Encryption.h"

Encryption::Encryption() = default;

void Encryption::onDecrypt(const QString &key) {
    QString salt = QCryptographicHash::hash("logins", QCryptographicHash::Sha256).toHex();
    const QString &password = key;
    QString encryptionKey = password + salt;
    runPythonScript("decrypt", encryptionKey);
}

void Encryption::onEncrypt(const QString &key) {
    QString salt = QCryptographicHash::hash("logins", QCryptographicHash::Sha256).toHex();
    const QString &password = key;
    QString encryptionKey = password + salt;
    runPythonScript("encrypt", encryptionKey);
}

void Encryption::onCreateLogins() {
    QFile loginsFile(QDir::currentPath() + "/.logins.json");
    if (loginsFile.open(QIODevice::WriteOnly)) {
        loginsFile.close();
    }
}

void Encryption::runPythonScript(const QString &operation, const QString &key) {
    QProcess process;
    QStringList params;
    params << "/home/vaia/CLionProjects/PassKeep/encryption.py" << operation << key;

    // Assuming python is in your system PATH. Adjust the path accordingly if needed.
    process.start("python3", params);

    process.waitForFinished();
}
