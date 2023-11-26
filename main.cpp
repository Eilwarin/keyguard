#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "Encryption.h"
#include "Landing.h"

class Main : public QWidget, public Encryption{

public:
    Main() {
        auto masterFile = QString(Encryption::search());
        setWindowTitle("PassKeep - Qt");
        setFixedSize(1280, 720);

        if (masterFile.isEmpty()){
            masterNotFound();
        } else{
            masterFound();
        }
    }

    void masterNotFound(){
        auto *layout = new QGridLayout(this);
        wasFound = new QLabel("Key file not found.", this);
        createMaster = new QPushButton("Create key file", this);
        createMaster->setFixedWidth(140);
        selectMaster = new QPushButton("Select file...", this);
        selectMaster->setFixedWidth(140);

        connect(createMaster, &QPushButton::clicked, [this](){
            createMasterFile();
        });

        layout->addWidget(wasFound, 1, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(createMaster, 2, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(selectMaster, 3, 0, 8, 1, Qt::AlignCenter);
    }

    void masterFound(){
        auto *layout = new QGridLayout(this);
        wasFound = new QLabel("Key file found.", this);
        masterLabel = new QLabel("Enter master key:", this);
        master = new QLineEdit(this);
        master->setEchoMode(QLineEdit::Password);
        decrypt = new QPushButton(this);
        decrypt->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/unlock.png"));
        decrypt->setIconSize(QSize(24, 24));
        connect(decrypt, &QPushButton::clicked, [this](){
            QString key = (QCryptographicHash::hash(QByteArray(master->text().toUtf8()), QCryptographicHash::Sha512) +
                    QCryptographicHash::hash(QByteArray("logins"), QCryptographicHash::Sha256));
            QString masterKey = QCryptographicHash::hash(QByteArray(key.toUtf8()), QCryptographicHash::Sha512).toHex();
            QString loginsFile = QCryptographicHash::hash(QByteArray(master->text().toUtf8()), QCryptographicHash::Sha512).toHex();
            Encryption::loginFile = new QFile(this);
            Encryption::loginFile->setFileName(loginsFile);
            Encryption::masterKey = QCryptographicHash::hash(QByteArray(master->text().toUtf8()), QCryptographicHash::Sha512).toHex();
            Encryption::onDecryptMaster(masterKey, loginsFile);
            Encryption::onEncrypt(masterKey, QDir::currentPath() + "/.master.key");
            close();
            (new Landing())->show();
        });

        layout->addWidget(wasFound, 1, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(masterLabel, 2, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(master, 3, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(decrypt, 4, 0, 8, 1, Qt::AlignCenter);
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
        delete layout;

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
        copyToClipboard = new QPushButton(this);
        copyToClipboard->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/copy.png"));
        copyToClipboard->setIconSize(QSize(16, 16));

        auto *newLayout = new QGridLayout(this);
        newLayout->addWidget(enterMaster, 1, 2, 8, 12, Qt::AlignCenter);
        newLayout->addWidget(newMaster, 2, 2, 8, 12, Qt::AlignCenter);
        newLayout->addWidget(repeatMaster, 3, 2, 8, 12, Qt::AlignCenter);
        newLayout->addWidget(generateMaster, 2, 4, 8, 11, Qt::AlignCenter);
        newLayout->addWidget(copyToClipboard, 2, 4, 8, 12, Qt::AlignCenter);
        newLayout->addWidget(submitMaster, 4, 2, 8, 12, Qt::AlignCenter);

        connect(submitMaster, &QPushButton::clicked, [this](){
            QString key = (QCryptographicHash::hash(QByteArray(repeatMaster->text().toUtf8()), QCryptographicHash::Sha512) +
                           QCryptographicHash::hash(QByteArray("logins"), QCryptographicHash::Sha256));
            QString masterKey = QCryptographicHash::hash(QByteArray(key.toUtf8()), QCryptographicHash::Sha512).toHex();
            Encryption::masterKey = QCryptographicHash::hash(QByteArray(repeatMaster->text().toUtf8()), QCryptographicHash::Sha512).toHex();
            Encryption::onCreateMaster();
            Encryption::onEncrypt(masterKey, QDir::currentPath() + "/.master.key");
            close();
            (new Landing())->show();
        });

        connect(generateMaster, &QPushButton::clicked, [this](){
            QByteArray randomBytes;
            for(int i = 0; i < 48; ++i){
                randomBytes.append(static_cast<char>(QRandomGenerator::global()->bounded(94) + 33));
            }
            QString secureString = QString::fromLatin1(randomBytes);
            newMaster->setText(secureString);
            repeatMaster->setText(secureString);
        });

        connect(copyToClipboard, &QPushButton::clicked, this, &Main::onCopyToClipboard);
    }
    void onCopyToClipboard() const {
        // Copy the text from the key field to the clipboard
        QApplication::clipboard()->setText(repeatMaster->text());
    }
private:
    QLabel *wasFound{};
    QLabel *masterLabel{};
    QLineEdit *master{};
    QPushButton *decrypt{};
    QPushButton *selectMaster{};
    QPushButton *createMaster{};
    QLabel *enterMaster{};
    QLineEdit *newMaster{};
    QLineEdit *repeatMaster{};
    QPushButton *generateMaster{};
    QPushButton *submitMaster{};
    QPushButton *copyToClipboard{};
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    Main mainWindow;
    mainWindow.show();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [](){
        Encryption encryption;
        encryption.onEncryptLogin();
    });

    // Start the application event loop
    return QApplication::exec();
}
