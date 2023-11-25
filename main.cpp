#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "NewKey.h"  // Include the header file for NewKey

class Main : public QWidget {

public:
    Main() {
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
        connect(addButton, &QPushButton::clicked, this, &Main::onAddButtonClick);

        // Connect the "Open" button's clicked signal to a slot in this class
        connect(openButton, &QPushButton::clicked, this, &Main::onOpenButtonClick);


    }

public slots:
    void onAddButtonClick() {
        // Destroy the current main window
        close();

        // Create a new instance of NewKey
        auto *newKey = new NewKey();
        newKey->show();
    }

    void onOpenButtonClick() {
        // Destroy the current main window
        close();

        // Create a new instance of NewKey
        auto *newKey = new NewKey();
        newKey->show();
    }

private:
    QPushButton *addButton;
    QPushButton *openButton;
    QLabel *addLabel;
    QLabel *openLabel;

};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    Main mainWindow;
    mainWindow.show();

    // Start the application event loop
    return QApplication::exec();
}
