#include "soundspane.h"

void SoundsPane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* logInSoundLabel = new QLabel(tr("Log In Sound"));
    logInSoundLabel->setFont(mTitleFont);
    contentsLayout->addWidget(logInSoundLabel);

    QHBoxLayout* logInSoundLayout = new QHBoxLayout();
    logInSoundLayout->setContentsMargins(10, 1, 1, 1);

    QLineEdit* logInSoundPathLineEdit = new QLineEdit();
    logInSoundPathLineEdit->setFont(mFont);
    logInSoundPathLineEdit->setPlaceholderText(tr("Type path here..."));
    logInSoundPathLineEdit->setClearButtonEnabled(true);
    logInSoundLayout->addWidget(logInSoundPathLineEdit);

    QPushButton* logInSoundChooseButton = new QPushButton("...");
    logInSoundLayout->addWidget(logInSoundChooseButton);

    contentsLayout->addLayout(logInSoundLayout);

    QLabel* logOutSoundLabel = new QLabel(tr("Log Out Sound"));
    logOutSoundLabel->setFont(mTitleFont);
    contentsLayout->addWidget(logOutSoundLabel);

    QHBoxLayout* logOutSoundLayout = new QHBoxLayout();
    logOutSoundLayout->setContentsMargins(10, 1, 1, 1);

    QLineEdit* logOutSoundPathLineEdit = new QLineEdit();
    logOutSoundPathLineEdit->setFont(mFont);
    logOutSoundPathLineEdit->setPlaceholderText(tr("Type path here..."));
    logOutSoundPathLineEdit->setClearButtonEnabled(true);
    logOutSoundLayout->addWidget(logOutSoundPathLineEdit);

    QPushButton* logOutSoundChooseButton = new QPushButton("...");
    logOutSoundLayout->addWidget(logOutSoundChooseButton);

    contentsLayout->addLayout(logOutSoundLayout);

    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));

    QPushButton* saveButton = new QPushButton(tr("Save settings"));
    saveButton->setFont(mFont);
    contentsLayout->addWidget(saveButton);
    saveButton->setVisible(false);

    // Setting current settings
    logInSoundPathLineEdit->setText(getConfigValue("logInSound").toString());
    logOutSoundPathLineEdit->setText(getConfigValue("logOutSound").toString());

    // Making connections
    connect(logInSoundPathLineEdit, &QLineEdit::textChanged, this, [saveButton]() {
        saveButton->setVisible(true);
    });
    connect(logOutSoundPathLineEdit, &QLineEdit::textChanged, this, [saveButton]() {
        saveButton->setVisible(true);
    });
    connect(saveButton, &QPushButton::clicked, this, [this,
                                                      logInSoundPathLineEdit,
                                                      logOutSoundPathLineEdit,
                                                      saveButton]() {
        prepareToSave(logInSoundPathLineEdit,
                      logOutSoundPathLineEdit);
        saveConfig();
        saveButton->setVisible(false);
    });

    connect(logInSoundChooseButton,
            &QPushButton::clicked, this, [this, logInSoundPathLineEdit]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::homePath(),
                                                        "Audio Files (*.mp3 *.wav *.m4a *.ogg *.opus)");
        if (!fileName.isEmpty()) {
            logInSoundPathLineEdit->setText(fileName);
        }
    });
    connect(logOutSoundChooseButton,
            &QPushButton::clicked, this, [this, logOutSoundPathLineEdit]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::homePath(),
                                                        "Audio Files (*.mp3 *.wav *.m4a *.ogg *.opus)");
        if (!fileName.isEmpty()) {
            logOutSoundPathLineEdit->setText(fileName);
        }
    });

    finalizeUI();
}

void SoundsPane::prepareToSave(QLineEdit* logInSoundLineEdit,
                               QLineEdit* logOutSoundLineEdit) {
    setEntry("logInSound", QJsonValue(logInSoundLineEdit->text()));
    setEntry("logOutSound", QJsonValue(logOutSoundLineEdit->text()));
}

SoundsPane::SoundsPane(QJsonObject* cfgObj) : Pane(nullptr,
                                                   cfgObj,
                                                   tr("Sounds"),
                                                   "preferences-system-sound") {

}
