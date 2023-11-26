#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "NewKey.h"  // Include the header file for NewKey
#include "Encryption.h"
#include "Landing.h"

class Main : public QWidget {

public:
    Main() {
        auto masterFile = Encryption::search();
        setWindowTitle("PassKeep - Qt");
        setFixedSize(1280, 720);

        if (masterFile.isEmpty()){
            masterNotFound();
        } else{
            masterFound(masterFile);
        }
    }

    void masterNotFound(){
        auto *layout = new QVBoxLayout(this);
        wasFound = new QLabel("Key file not found.", this);
        createMaster = new QPushButton("Create key file", this);
        selectMaster = new QPushButton("Select file...", this);

        connect(createMaster, &QPushButton::clicked, [this](){
            createMasterFile();
        });

        connect(selectMaster, &QPushButton::clicked, [](){
            Encryption::onSelectMaster();
        });

        layout->addWidget(wasFound);
        layout->addWidget(createMaster);
        layout->addWidget(selectMaster);
    }

    void masterFound(const QString& masterFile){
        auto *layout = new QVBoxLayout(this);
        wasFound = new QLabel("Key file found.", this);
        masterLabel = new QLabel("Enter master key:", this);
        master = new QLineEdit(this);
        master->setEchoMode(QLineEdit::Password);
        decrypt = new QPushButton(this);
        decrypt->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/unlock.png"));
        decrypt->setIconSize(QSize(24, 24));
        connect(decrypt, &QPushButton::clicked, [this, &masterFile](){
            QString key = (QCryptographicHash::hash(master->text().toUtf8(), QCryptographicHash::Sha512) +
                    QCryptographicHash::hash("logins", QCryptographicHash::Sha256));
            QString masterKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Sha512).toHex();
            QString loginsFile = QCryptographicHash::hash(master->text().toUtf8(), QCryptographicHash::Sha512).toHex();
            Encryption::onDecryptMaster(masterKey, loginsFile);
            Encryption::onEncrypt(masterKey, masterFile);
            close();
            (new Landing())->show();
        });

        layout->addWidget(wasFound);
        layout->addWidget(masterLabel);
        layout->addWidget(master);
    }

private slots:
    void createMasterFile() {
        // Clear the layout and remove the existing widgets
        QLayout *layout = this->layout();
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        // Set up the new widgets
        enterMaster = new QLabel("Enter master key:", this);
        newMaster = new QLineEdit(this);
        newMaster->setEchoMode(QLineEdit::Password);
        repeatMaster = new QLineEdit(this);
        repeatMaster->setEchoMode(QLineEdit::Password);
        generateMaster = new QPushButton(this);
        generateMaster->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/generate.png"));
        generateMaster->setIconSize(QSize(16, 16));
        submitMaster = new QPushButton("Create", this);

        auto *newLayout = new QGridLayout(this);
        newLayout->addWidget(enterMaster, 1, 0, Qt::AlignCenter);
        newLayout->addWidget(newMaster, 2, 0, Qt::AlignCenter);
        newLayout->addWidget(repeatMaster, 3, 0, Qt::AlignCenter);
        newLayout->addWidget(generateMaster, 2, 1, Qt::AlignCenter);
        newLayout->addWidget(submitMaster, 4, 0, Qt::AlignCenter);

        connect(submitMaster, &QPushButton::clicked, [](){
            Encryption::onCreateMaster();
        });

        connect(generateMaster, &QPushButton::clicked, [this](){
            QByteArray randomBytes;
            for(int i = 0; i < 16; ++i){
                randomBytes.append(static_cast<char>(QRandomGenerator::global()->bounded(94) + 33));
            }
            QString secureString = QString::fromLatin1(randomBytes);
            newMaster->setText(secureString);
            repeatMaster->setText(secureString);
        });
    }
private:
    QLabel *wasFound;
    QLabel *masterLabel;
    QLineEdit *master;
    QPushButton *decrypt;
    QPushButton *selectMaster;
    QPushButton *createMaster;
    QLabel *enterMaster;
    QLineEdit *newMaster;
    QLineEdit *repeatMaster;
    QPushButton *generateMaster;
    QPushButton *submitMaster;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    Main mainWindow;
    mainWindow.show();

    // Start the application event loop
    return QApplication::exec();
}
