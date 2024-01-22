//
// Created by vaia on 11/23/23.
//

#include "OpenKeys.h"
#include "NewKey.h"


OpenKeys::OpenKeys(QWidget *parent) : QWidget(parent), Encryption() {
    setWindowTitle("View Logins");
    setFixedSize(1024, 768);

    auto *layout = new QGridLayout(this);

    sortLabel = new QLabel("Sort by:", this);
    loginCount = new QLabel(this);


    searchField = new QLineEdit(this);
    searchField->setPlaceholderText("Search");
    searchField->setMinimumWidth(240);
    sortField = new QComboBox(this);
    sortField->addItem("Username (A-Z)");
    sortField->addItem("Username (Z-A)");
    sortField->addItem("Date Modified");
    sortField->addItem("Date Accessed");
    sortField->setMinimumWidth(140);
    connect(sortField, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](){
        onSort(sortField->currentText());
    });
    backButton = new QPushButton(this);  // New back button
    backButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/back.png"));
    backButton->setIconSize(QSize(24, 24));
    connect(backButton, &QPushButton::clicked, this, &OpenKeys::onGoBack);
    importButton = new QPushButton(this); // New import button
    importButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/import.png"));
    importButton->setIconSize(QSize(24, 24));
    connect(importButton, &QPushButton::clicked, this, &OpenKeys::onImport);
    searchButton = new QPushButton(this);
    searchButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/search.png"));
    searchButton->setIconSize(QSize(16, 16));
    connect(searchButton, &QPushButton::clicked, this, &OpenKeys::onSearch);
    loginTable = new QTableWidget(this);
    // Set size policies and minimum size
    loginTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    loginTable->setMinimumSize(800, 300);  // Adjust the minimum size as needed

    layout->addWidget(backButton, 0, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(importButton, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignRight);
    layout->addWidget(searchField, 0, 0, 1, 2, Qt::AlignHCenter);
    layout->addWidget(searchButton, 0, 0, 1, 3, Qt::AlignHCenter);
    layout->addWidget(sortLabel, 0, 1, 1, 2, Qt::AlignHCenter);
    layout->addWidget(sortField, 0, 1, 1, 3, Qt::AlignHCenter);
    layout->setRowStretch(1, 1);
    layout->addWidget(loginCount, 0, 3, 1, 1, Qt::AlignHCenter);
    layout->addWidget(loginTable, 1, 0, 1, 4, Qt::AlignHCenter);


    setupTable();
}

void OpenKeys::onEdit(int row) {
    close();

    auto *newKey = new NewKey();

    auto *url = loginTable->item(row, 0);
    auto *username = loginTable->item(row, 1);
    auto *key = loginTable->item(row, 2);

    if (url && username && key) {
        newKey->url->setText(url->text());
        newKey->username->setText(username->text());
        newKey->key->setText(key->text());
        newKey->show();
    }
}

void OpenKeys::onGoBack() {
    // Destroy the current main window
    close();

    auto *landing = new Landing();
    landing->show();
}
void OpenKeys::onImport() {
    auto filePath = QFileDialog::getOpenFileName(this, tr("Select JSON File"), "", tr("JSON Files (*.json)"));

    if (filePath.isEmpty()){
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    auto importedJson = file.readAll();
    file.close();
    auto importedDocument = QJsonDocument::fromJson(importedJson);

    if (!importedDocument.isArray()){
        return;
    }

    auto importedArray = importedDocument.array();
    dataArray += importedArray;

    updateLogin();
    loadLoginData(QFile(QDir::currentPath() + "/.logins.json"));
}

void OpenKeys::onSort(const QString &sort) {
    Qt::SortOrder sorting = Qt::AscendingOrder;

    if (sort == "Username (Z-A)"){
        loginTable->sortItems(1, Qt::DescendingOrder);
    } else if (sort == "Username (A-Z)"){
        loginTable->sortItems(1, sorting);
    } else if (sort == "Date Modified"){
        loginTable->sortItems(3, sorting);
    } else if (sort == "Date Accessed"){
        loginTable->sortItems(4, sorting);
    }
}

void OpenKeys::onSearch() {
    QString query = searchField->text();
    QRegularExpression regex(query, QRegularExpression::CaseInsensitiveOption);

    if (query.isEmpty()){
        for (int row = 0; row < loginTable->rowCount(); ++row) {
            loginTable->setRowHidden(row, false);
        }
        return;
    }
    for (int row = 0; row < loginTable->rowCount(); ++row){
        bool matchFound = false;

        for (int col = 0; col < 2; ++col) {
            QTableWidgetItem *login = loginTable->item(row, col);
            if (login && regex.match(login->text()).hasMatch()){
                matchFound = true;
                break;
            }
        }
        loginTable->setRowHidden(row, !matchFound);
    }

}

void OpenKeys::setupTable() {
    // Set up the table with 8 headers
    loginTable->setColumnCount(8);
    QStringList headers;
    headers << "URL" << "Username" << "Key" << "Date Modified" << "Date Accessed" << "Edit" << "Copy" << "Delete";
    loginTable->setHorizontalHeaderLabels(headers);

    // Adjust column widths as needed
    loginTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void OpenKeys::populateTable() {
    loginTable->setRowCount(dataArray.size());

    for (int i = 0; i < dataArray.size(); ++i) {
        auto dataObject = dataArray[i].toObject();
        auto *url = new QTableWidgetItem(dataObject["url"].toString());
        auto *username = new QTableWidgetItem(dataObject["username"].toString());
        auto *key = new QTableWidgetItem(dataObject["key"].toString());

        auto *modified = new QTableWidgetItem(dataObject["dateModified"].toString());
        auto *accessed = new QTableWidgetItem(dataObject["dateModified"].toString());

        loginTable->setItem(i, 0, url);
        loginTable->setItem(i, 1, username);
        loginTable->setItem(i, 2, key);
        loginTable->setItem(i, 3, modified);
        loginTable->setItem(i, 4, accessed);

        // Add a button in the sixth column
        auto *button = new QPushButton(this);
        button->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/compose.png"));
        button->setIconSize(QSize(16, 16));
        connect(button, &QPushButton::clicked, [this, i]() { onEdit(i); });
        loginTable->setCellWidget(i, 5, button);

        auto *copyButton = new QPushButton(this);
        copyButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/copy.png"));
        copyButton->setIconSize(QSize(16, 16));
        connect(copyButton, &QPushButton::clicked, [this, i]() {onCopy(i);});
        loginTable->setCellWidget(i, 6, copyButton);

        auto *deleteButton = new QPushButton(this);
        deleteButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/.icons/delete.png"));
        deleteButton->setIconSize(QSize(16, 16));
        connect(deleteButton, &QPushButton::clicked, [this, i]() { onDelete(i);});
        loginTable->setCellWidget(i, 7, deleteButton);
    }
}

void OpenKeys::loadLoginData(QFile logins) {
    int totalLogins;

    QJsonDocument jsonDocument;
    if (logins.open(QIODevice::ReadOnly | QIODevice::Text)){
        QByteArray jsonData = logins.readAll();
        jsonDocument = QJsonDocument::fromJson(jsonData);
        logins.close();
    }
    QJsonArray loginData = jsonDocument.isArray() ? jsonDocument.array() : QJsonArray();

    totalLogins = loginData.size();

    if (!loginData.isEmpty()){
        dataArray = loginData;
        loginCount->setText(QString::number(totalLogins) + " logins");
        populateTable();
    }
}
void OpenKeys::onCopy(int row) {
    if (loginTable->item(row, 2)){
        QApplication::clipboard()->setText(QString(loginTable->item(row, 2)->text()));
        QJsonObject jsonObject = dataArray[row].toObject();
        jsonObject["dateAccessed"] = QDate::currentDate().toString("dd-MM-yyyy");
        dataArray[row] = jsonObject;
        updateLogin();
        loadLoginData(QFile(QDir::currentPath() + "/.logins.json"));
    }
}
void OpenKeys::updateLogin() {
    QFile file(QDir::currentPath() + "/.logins.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QJsonDocument updated(dataArray);
        file.write(updated.toJson());
        file.close();
    }
}
void OpenKeys::onDelete(int row) {
    if (row >=0){
        loginTable->removeRow(row);
        dataArray.removeAt(row);
        updateLogin();
    }
}
