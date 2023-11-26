//
// Created by vaia on 11/25/23.
//

#include "Encryption.h"

Encryption::Encryption() = default;

QString Encryption::search(const QString &directory){
    QDir currentDir(directory);
    currentDir.setNameFilters(QStringList(".*"));
    currentDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    QFileInfoList files = currentDir.entryInfoList(QStringList(".master.key"), QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    if (!files.isEmpty()){
        return files.first().absoluteFilePath();
    }

    return nullptr;
}

void Encryption::onDecryptMaster(const QString& key, const QString& filename) {
    QStringList arguments;
    arguments << "decrypt" << QDir::currentPath() + "/.master.key" << key;

    QProcess aesProcess;
    aesProcess.start("python", QStringList() << QDir::currentPath() + "/.master.key" << arguments);
    aesProcess.waitForFinished(-1);
    loginKey = QString(aesProcess.readAllStandardOutput());

    onDecrypt(loginKey, filename);
}
void Encryption::onDecrypt(const QString &key, const QString &filename) {
    QStringList arguments;
    arguments << "decrypt" << filename << key;
    QProcess aesProcess;
    aesProcess.start("python", QStringList() << filename << arguments);
    aesProcess.waitForFinished(-1);
    QString loginData = aesProcess.readAllStandardOutput();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(loginData.toUtf8());
    if (jsonDocument.isArray()){
        logins = jsonDocument.array();
        loginFile->setFileName(QDir::currentPath() + "/" + filename);
    }
}

void Encryption::onEncrypt(const QString &key, const QString& filename) {
    QStringList arguments;
    arguments << "encrypt" << filename << key;

    QProcess aesProcess;
    aesProcess.start("python", QStringList() << filename << arguments);
    aesProcess.waitForFinished(-1);
}

void Encryption::onCreateMaster() {
    QFile masterFile = QDir::currentPath() + "/.master.key";
    if (masterFile.open(QIODevice::WriteOnly)){
        masterFile.close();
    }
}

void Encryption::onEncryptLogin() {
    if (loginFile->exists() && loginFile->open(QIODevice::ReadOnly | QIODevice::Text)){
        QString fileHash = QCryptographicHash::hash(QByteArray(loginFile->readAll()), QCryptographicHash::Sha256).toHex();
        loginKey = QCryptographicHash::hash(QByteArray((masterKey + fileHash).toUtf8()), QCryptographicHash::Sha512);
        onEncrypt(loginKey, loginFile->fileName());
    }
}