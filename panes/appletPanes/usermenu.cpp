#include "usermenu.h"

QJsonObject userMenuAppletPaneConfig;

void UserMenuAppletPane::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    userMenuAppletPaneConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

void UserMenuAppletPane::setCurrentSettings(QRadioButton* useDefaultIcon,
                                            QRadioButton* useAvatar,
                                            QLineEdit* avatarPath,
                                            QPushButton* previewButton) {
    QString avatarEntry = userMenuAppletPaneConfig["avatar"].toString();
    bool avatarEntryEmpty = avatarEntry.isEmpty();
    if (avatarEntryEmpty) {
        useDefaultIcon->setChecked(true);
    }
    else {
        useAvatar->setChecked(true);
        avatarPath->setText(avatarEntry);
        previewButton->setIcon(QIcon(avatarEntry));
    }
}

void UserMenuAppletPane::saveSettings(QRadioButton* useDefaultIcon,
                                      QRadioButton* useAvatar,
                                      QLineEdit* avatarPath) {
    if (useDefaultIcon->isChecked()) {
        userMenuAppletPaneConfig["avatar"] = QJsonValue("");
    }
    else {
        userMenuAppletPaneConfig["avatar"] = QJsonValue(avatarPath->text());
    }
    Pane::saveConfig(userMenuAppletPaneConfig);
}

QWidget* UserMenuAppletPane::createUI(Settings* controlCenter) {
    readConfig();
    QIcon::setThemeName(userMenuAppletPaneConfig["iconTheme"].toString());

    // UI
    QWidget* userMenuAppletPane = new QWidget();
    userMenuAppletPane->setObjectName("userMenuAppletPane");
    userMenuAppletPane->setWindowTitle("plainControlCenter");
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(4, 4, 4, 4);
    userMenuAppletPane->setLayout(layout);

    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + userMenuAppletPaneConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    userMenuAppletPane->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(userMenuAppletPaneConfig["fontFamily"].toString());
    paneFont.setPointSize(userMenuAppletPaneConfig["fontSize"].toInt());
    userMenuAppletPane->setFont(paneFont);

    short width = 400, height = 500;
    userMenuAppletPane->setGeometry(650, 250, width, height);

    QPushButton* backPushButton = new QPushButton("Close");
    backPushButton->setIcon(QIcon::fromTheme("go-previous"));
    layout->addWidget(backPushButton);

    QRadioButton* useDefaultIconRadioButton =
        new QRadioButton("Use default icon");
    layout->addWidget(useDefaultIconRadioButton);

    QRadioButton* useAvatarRadioButton = new QRadioButton("Use avatar");
    layout->addWidget(useAvatarRadioButton);

    QLineEdit* avatarPathLineEdit = new QLineEdit();
    layout->addWidget(avatarPathLineEdit);

    QString username = getenv("USER");
    QPushButton* previewButton = new QPushButton(username);
    layout->addWidget(previewButton);

    layout->addItem(new QSpacerItem(0,
                                    0,
                                    QSizePolicy::Ignored,
                                    QSizePolicy::Expanding));

    QPushButton* revertPushButton = new QPushButton("Revert");
    layout->addWidget(revertPushButton);

    QPushButton* savePushButton = new QPushButton("Save");
    layout->addWidget(savePushButton);

    setCurrentSettings(useDefaultIconRadioButton,
                       useAvatarRadioButton,
                       avatarPathLineEdit,
                       previewButton);

    // Make connections
    userMenuAppletPane->connect(useDefaultIconRadioButton,
                                &QRadioButton::toggled,
                                userMenuAppletPane,
                                [this, useDefaultIconRadioButton,
                                 avatarPathLineEdit, previewButton] {
        if (useDefaultIconRadioButton->isChecked()) {
            avatarPathLineEdit->setEnabled(false);
            previewButton->setIcon(QIcon::fromTheme("computer"));
        }
        else {
            avatarPathLineEdit->setEnabled(true);
            previewButton->setIcon(QIcon(avatarPathLineEdit->text()));
        }
    });

    userMenuAppletPane->connect(avatarPathLineEdit,
                                &QLineEdit::textChanged,
                                userMenuAppletPane,
                                [this, avatarPathLineEdit,
                                 previewButton]() {
        previewButton->setIcon(QIcon(avatarPathLineEdit->text()));
    });

    userMenuAppletPane->connect(revertPushButton, &QPushButton::clicked, userMenuAppletPane,
                               [this, useDefaultIconRadioButton,
                                useAvatarRadioButton,
                                avatarPathLineEdit,
                                previewButton]() {
        setCurrentSettings(useDefaultIconRadioButton,
                           useAvatarRadioButton,
                           avatarPathLineEdit,
                           previewButton);
    });

    userMenuAppletPane->connect(savePushButton, &QPushButton::clicked, userMenuAppletPane,
                               [this, useDefaultIconRadioButton,
                                useAvatarRadioButton,
                                avatarPathLineEdit]() {
       saveSettings(useDefaultIconRadioButton,
                    useAvatarRadioButton,
                    avatarPathLineEdit);
    });
    userMenuAppletPane->connect(backPushButton, &QPushButton::clicked, userMenuAppletPane,
                               [this, userMenuAppletPane, controlCenter]() {
                                   controlCenter->mUserMenuWidgetVisible = false;
                                   delete userMenuAppletPane;
                                   delete this;
                               });

    return userMenuAppletPane;
}
