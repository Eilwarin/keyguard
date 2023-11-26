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
    arguments << "decrypt" << QString(search()) << key;

    QProcess aesProcess;
    aesProcess.start("python", QStringList() << QString(search()) << arguments);
    aesProcess.waitForFinished(-1);
    loginKey = aesProcess.readAllStandardOutput();

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
    }
}

void Encryption::onEncrypt(const QString& key, const QString& filename) {
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
    onEncrypt(masterKey, QDir::currentPath() + "/.master.key");
}