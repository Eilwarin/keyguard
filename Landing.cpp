#include "Landing.h"

Landing::Landing(QWidget *parent) : QWidget(parent), Encryption() {
    setWindowTitle("PassKeep - Qt");
    setFixedSize(1280, 720);

    // Create a button with an image and label
    addButton = new QPushButton(this);
    openButton = new QPushButton(this);
    addLabel = new QLabel("New", this);
    openLabel = new QLabel("Open", this);

    addButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/add.png"));
    addButton->setIconSize(QSize(64, 64));
    openButton->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/folder.png"));
    openButton->setIconSize(QSize(64, 64));


    // Create layout
    auto *layout = new QGridLayout(this);

    // Add the button to the layout
    layout->addWidget(addButton, 0, 0, 8, 4, Qt::AlignCenter);
    layout->addWidget(openButton, 0, 1, 8, 4, Qt::AlignCenter);
    layout->addWidget(addLabel, 1, 0, 8, 4, Qt::AlignCenter);
    layout->addWidget(openLabel, 1, 1, 8, 4, Qt::AlignCenter);


    // Set the layout for the main window
    setLayout(layout);

    // Connect the "Add" button's clicked signal to a slot in this class
    connect(addButton, &QPushButton::clicked, this, &Landing::onAddButtonClick);

    // Connect the "Open" button's clicked signal to a slot in this class
    connect(openButton, &QPushButton::clicked, this, &Landing::onOpenButtonClick);
}

void Landing::onAddButtonClick() {
    // Destroy the current main window
    close();

    // Create a new instance of NewKey
    auto *newKey = new NewKey();
    newKey->show();
}

void Landing::onOpenButtonClick() {
    // Destroy the current main window
    close();

    // Create a new instance of NewKey
    auto *openKeys = new OpenKeys();
    openKeys->loadLoginData(QFile(QDir::currentPath() + "/.logins.json"));
    openKeys->show();
}


