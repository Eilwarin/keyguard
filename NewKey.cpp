//
// Created by vaia on 11/23/23.
//

#include <QCoreApplication>
#include "NewKey.h"

NewKey::NewKey(QWidget *parent) : QWidget(parent){
    setWindowTitle("New Login");
    setFixedSize(1280, 720);

    urlLabel = new QLabel("URL:", this);
    url = new QLineEdit(this);
    url->setMinimumWidth(150);
    usernameLabel = new QLabel("Username:", this);
    username = new QLineEdit(this);
    username->setMinimumWidth(150);
    keyLabel = new QLabel("Key:", this);
    key = new QLineEdit(this);
    key->setMinimumWidth(150);
    key->setEchoMode(QLineEdit::Password);


    getTextFields = new QPushButton("Save", this);
    generateKey = new QPushButton(this);
    generateKey->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/generate.png"));
    generateKey->setIconSize(QSize(16, 16));
    backButton = new QPushButton(this);  // New back button
    backButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/back.png"));
    backButton->setIconSize(QSize(24, 24));
    showKeyButton = new QPushButton(this);
    showKeyButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/show.png"));
    showKeyButton->setIconSize(QSize(16, 16));
    copyToClipboard = new QPushButton(this);
    copyToClipboard->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/copy.png"));
    copyToClipboard->setIconSize(QSize(16, 16));

    auto *layout = new QGridLayout(this);

    layout->addWidget(urlLabel, 0, 0, 12, 4, Qt::AlignCenter);
    layout->addWidget(url, 0, 1, 12, 4, Qt::AlignCenter);
    layout->addWidget(usernameLabel, 1, 0, 12, 4, Qt::AlignCenter);
    layout->addWidget(username, 1, 1, 12, 4, Qt::AlignCenter);
    layout->addWidget(keyLabel, 2, 0, 12, 4, Qt::AlignCenter);
    layout->addWidget(key, 2, 1, 12, 4, Qt::AlignCenter);
    layout->addWidget(getTextFields, 5, 0, 12, 6, Qt::AlignCenter);
    layout->addWidget(showKeyButton, 2, 2, 12, 3, Qt::AlignCenter);
    layout->addWidget(generateKey, 2, 0, 12, 5, Qt::AlignCenter);
    layout->addWidget(copyToClipboard, 3, 0, 12, 6 ,Qt::AlignCenter);
    layout->addWidget(backButton, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);  // Back button in the top left corner

    connect(getTextFields, &QPushButton::clicked, this, &NewKey::onButtonClick);
    connect(generateKey, &QPushButton::clicked, this, &NewKey::onGenerateClick);
    connect(backButton, &QPushButton::clicked, this, &NewKey::onGoBack);
    connect(showKeyButton, &QPushButton::clicked, this, &NewKey::onShowKey);
    connect(copyToClipboard, &QPushButton::clicked, this, &NewKey::onCopyToClipboard);
}
void NewKey::onButtonClick() const {
    QString usr_name = username->text();
    QString site_url = url->text();
    QString gen_key = key->text();

    QFile file("/home/vaia/CLionProjects/PassKeep/logins.json");
    QJsonDocument jsonDocument;

    if (file.exists() && file.open(QIODevice::ReadOnly)){
        jsonDocument = QJsonDocument::fromJson(file.readAll());
        file.close();
    }

    QJsonObject jsonObject;
    jsonObject["url"] = site_url;
    jsonObject["username"] = usr_name;
    jsonObject["key"] = gen_key;
    jsonObject["dateModified"] = QDate::currentDate().toString("dd-MM-yyyy");
    jsonObject["dateAccessed"] = QDate::currentDate().toString("dd-MM-yyyy");


    QJsonArray jsonArray = jsonDocument.isArray() ? jsonDocument.array() : QJsonArray();

    jsonArray.append(jsonObject);
    jsonDocument.setArray(jsonArray);

    if (file.open(QIODevice::WriteOnly)){
        file.write(jsonDocument.toJson());
        file.close();
    }

    url->clear();
    username->clear();
    key->clear();
}

void NewKey::onGenerateClick() const {
    QString generatedKey = onGenerateString();
    key->setText(generatedKey);
}

QString NewKey::onGenerateString() {
    QByteArray randomBytes;
    for(int i = 0; i < 16; ++i){
        randomBytes.append(static_cast<char>(QRandomGenerator::global()->bounded(94) + 33));
    }
    QString secureString = QString::fromLatin1(randomBytes);

    return secureString;
}

void NewKey::onGoBack() {
    // Destroy the current main window
    close();

    // Re-instantiate the main window
    QCoreApplication::exit(773);  // Use a specific exit code to indicate re-instantiation
}

void NewKey::onShowKey() {
    if (!showing){
        showKeyButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/hide.png"));
        key->setEchoMode(QLineEdit::Normal);
    } else{
        key->setEchoMode(QLineEdit::Password);
        showKeyButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/show.png"));
    }
    if (!showing){
        showing = true;
    } else{
        showing = false;
    }
}

void NewKey::onCopyToClipboard() const {
    // Copy the text from the key field to the clipboard
    QApplication::clipboard()->setText(key->text());
}