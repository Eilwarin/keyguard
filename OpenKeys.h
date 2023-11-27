//
// Created by vaia on 11/23/23.
//

#ifndef PASSKEEP_OPENKEYS_H
#define PASSKEEP_OPENKEYS_H

#include <QClipboard>
#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QFile>
#include <QJsonArray>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHeaderView>
#include <QDate>
#include <QRegularExpression>
#include <QFileDialog>
#include "Encryption.h"


class OpenKeys : public QWidget, public Encryption{
public:
    explicit OpenKeys(QWidget *parent = nullptr);
    void loadLoginData(QFile);
    void updateLogin();

public slots:
    void onEdit(int);
    void onImport();
    void onSort(const QString &);
    void onSearch();
    void onCopy(int);
    void onDelete(int);
    void onGoBack();

private:
    QLabel *sortLabel;
    QLabel *loginCount;
    QLineEdit *searchField;
    QComboBox *sortField;
    QPushButton *searchButton;
    QPushButton *backButton;
    QPushButton *importButton;
    QJsonArray dataArray;
    void setupTable();
    void populateTable();
    QTableWidget *loginTable;
};


#endif //PASSKEEP_OPENKEYS_H
