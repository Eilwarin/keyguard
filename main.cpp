#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRandomGenerator>
#include <QFile>
#include <QCoreApplication>
#include <QCryptographicHash>
#include "Encryption.h"
#include "Landing.h"

class Main : public QWidget {
public:
    Main(Encryption &encryption) : encryption(encryption) {
        setWindowTitle("PassKeep - Qt");
        setFixedSize(1280, 720);

        QFile logins(".logins.enc");
        if (!logins.open(QIODevice::ReadOnly)) {
            loginsNotFound();
        } else {
            loginsFound();
        }
    }

    void loginsNotFound() {
        auto *layout = new QGridLayout(this);
        wasFound = new QLabel("No logins found.", this);
        createMaster = new QPushButton("Create new login", this);
        createMaster->setFixedWidth(140);
        selectMaster = new QPushButton("Select file...", this);
        selectMaster->setFixedWidth(140);

        connect(createMaster, &QPushButton::clicked, [this]() {
            createLoginFile();
        });

        layout->addWidget(wasFound, 1, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(createMaster, 2, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(selectMaster, 3, 0, 8, 1, Qt::AlignCenter);
    }

    void loginsFound() {
        auto *layout = new QGridLayout(this);
        wasFound = new QLabel("Logins found.", this);
        masterLabel = new QLabel("Enter encryption key:", this);
        master = new QLineEdit(this);
        master->setEchoMode(QLineEdit::Password);
        decrypt = new QPushButton(this);
        decrypt->setIcon(QIcon("/home/vaia/CLionProjects/PassKeep/unlock.png"));
        decrypt->setIconSize(QSize(24, 24));
        connect(decrypt, &QPushButton::clicked, [this]() {
            encryption.loginKey = QCryptographicHash::hash(QString(master->text()).toUtf8(), QCryptographicHash::Sha512).toHex();
            encryption.onDecrypt(encryption.loginKey);
            close();
            (new Landing())->show();
        });

        layout->addWidget(wasFound, 1, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(masterLabel, 2, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(master, 3, 0, 8, 1, Qt::AlignCenter);
        layout->addWidget(decrypt, 4, 0, 8, 1, Qt::AlignCenter);
    }

private slots:
    void createLoginFile() {
        // Clear the layout and remove the existing widgets
        QLayout *layout = this->layout();
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;

        // Set up the new widgets
        enterMaster = new QLabel("Enter encryption key:", this);
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

        connect(submitMaster, &QPushButton::clicked, [this]() {
            encryption.loginKey = QCryptographicHash::hash(QString(repeatMaster->text()).toUtf8(), QCryptographicHash::Sha512).toHex();
            encryption.onCreateLogins();
            close();
            (new Landing())->show();
        });

        connect(generateMaster, &QPushButton::clicked, [this]() {
            QByteArray randomBytes;
            for (int i = 0; i < 48; ++i) {
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
    Encryption &encryption;
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
    QByteArray loginKey;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create an instance of the Encryption class
    Encryption encryption;

    // Create the main window with a reference to the Encryption instance
    Main mainWindow(encryption);
    mainWindow.show();

    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&encryption]() {
        encryption.onEncrypt(encryption.loginKey);
    });

    // Start the application event loop
    return QApplication::exec();
}
