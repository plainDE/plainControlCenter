#include "usermenudialog.h"

void UserMenuDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* userIconLabel = new QLabel(tr("User Icon"));
    userIconLabel->setFont(mTitleFont);
    contentsLayout->addWidget(userIconLabel);

    QRadioButton* defaultRadioButton = new QRadioButton(tr("Use default icon"));
    defaultRadioButton->setFont(mFont);
    contentsLayout->addWidget(defaultRadioButton);

    QRadioButton* customRadioButton = new QRadioButton(tr("Use custom icon"));
    customRadioButton->setFont(mFont);
    contentsLayout->addWidget(customRadioButton);

    QHBoxLayout* customLayout = new QHBoxLayout();

    QPushButton* customButton = new QPushButton();
    customButton->setFlat(true);
    customButton->setVisible(false);
    customLayout->addWidget(customButton);

    QLineEdit* customIconLineEdit = new QLineEdit();
    customIconLineEdit->setFont(mFont);
    customIconLineEdit->setPlaceholderText(tr("Type icon name or path here..."));
    customIconLineEdit->setVisible(false);
    customLayout->addWidget(customIconLineEdit);

    contentsLayout->addLayout(customLayout);

    QCheckBox* enableShutdownTimerCheckBox = new QCheckBox("Enable Shutdown Timer");
    enableShutdownTimerCheckBox->setFont(mFont);
    contentsLayout->addWidget(enableShutdownTimerCheckBox);

    QHBoxLayout* shutdownTimerLayout = new QHBoxLayout();

    QSpinBox* shutdownTimerSpinBox = new QSpinBox();
    shutdownTimerSpinBox->setFont(mFont);
    shutdownTimerSpinBox->setMinimum(0);
    shutdownTimerSpinBox->setMaximum(INT_MAX);
    shutdownTimerSpinBox->setValue(30);
    shutdownTimerSpinBox->setVisible(false);
    shutdownTimerLayout->addWidget(shutdownTimerSpinBox);

    QLabel* secLabel = new QLabel("sec");
    secLabel->setFont(mFont);
    secLabel->setVisible(false);
    shutdownTimerLayout->addWidget(secLabel);

    shutdownTimerLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                       QSizePolicy::MinimumExpanding,
                                                       QSizePolicy::Ignored));

    contentsLayout->addLayout(shutdownTimerLayout);

    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFont(mFont);
    buttonsLayout->addWidget(cancelButton);

    QPushButton* okButton = new QPushButton(tr("OK"));
    okButton->setFont(mFont);
    buttonsLayout->addWidget(okButton);

    contentsLayout->addLayout(buttonsLayout);

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, customRadioButton,
                                                    customIconLineEdit,
                                                    enableShutdownTimerCheckBox,
                                                    shutdownTimerSpinBox]() {
        prepareToSave(customRadioButton,
                      customIconLineEdit,
                      enableShutdownTimerCheckBox,
                      shutdownTimerSpinBox);
        saveConfig();
        this->hide();
        delete this;
    });

    connect(customRadioButton, &QRadioButton::clicked, this, [customIconLineEdit, customButton]() {
        customIconLineEdit->setVisible(true);
        customButton->setVisible(true);
    });

    connect(defaultRadioButton, &QRadioButton::clicked, this, [customIconLineEdit, customButton]() {
        customIconLineEdit->setVisible(false);
        customButton->setVisible(false);
    });

    connect(customIconLineEdit, &QLineEdit::textChanged, this, [customButton, customIconLineEdit]() {
        QString avatar = customIconLineEdit->text();
        if (QIcon::hasThemeIcon(avatar)) {
            customButton->setIcon(QIcon::fromTheme(avatar));
        }
        else if (QFile::exists(avatar)) {
            customButton->setIcon(QIcon(avatar));
        }
        else {
            customButton->setIcon(QIcon());
        }
    });

    connect(enableShutdownTimerCheckBox, &QCheckBox::stateChanged, this, [enableShutdownTimerCheckBox,
                                                                          shutdownTimerSpinBox,
                                                                          secLabel]() {
        if (enableShutdownTimerCheckBox->isChecked()) {
            shutdownTimerSpinBox->setVisible(true);
            secLabel->setVisible(true);
        }
        else {
            shutdownTimerSpinBox->setVisible(false);
            secLabel->setVisible(false);
        }
    });

    // Setting current settings
    QString avatar = getConfigValue("avatar").toString();
    if (avatar.isEmpty()) {
        defaultRadioButton->setChecked(true);
    }
    else {
        customRadioButton->setChecked(true);
        customIconLineEdit->setText(avatar);
        if (QIcon::hasThemeIcon(avatar)) {
            customButton->setIcon(QIcon::fromTheme(avatar));
        }
        else if (QFile::exists(avatar)) {
            customButton->setIcon(QIcon(avatar));
        }
        else {
            customButton->setIcon(QIcon());
        }
        customButton->setVisible(true);
        customIconLineEdit->setVisible(true);
    }

    int seconds = getConfigValue("secondsUntilPowerOff").toInt();
    if (seconds > -1) {
        enableShutdownTimerCheckBox->setChecked(true);
        shutdownTimerSpinBox->setValue(seconds);
        shutdownTimerSpinBox->setVisible(true);
    }
    else {
        enableShutdownTimerCheckBox->setChecked(false);
    }

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void UserMenuDialog::prepareToSave(QRadioButton* customRadioButton,
                                   QLineEdit* customLineEdit,
                                   QCheckBox* enableShutdownTimerCheckBox,
                                   QSpinBox* shutdownTimerSpinBox) {
    if (customRadioButton->isChecked()) {
        setEntry("avatar", customLineEdit->text());
    }
    else {  // Default icon
        setEntry("avatar", "");
    }

    if (enableShutdownTimerCheckBox->isChecked()) {
        setEntry("secondsUntilPowerOff", QJsonValue(shutdownTimerSpinBox->value()));
    }
    else {
        setEntry("secondsUntilPowerOff", QJsonValue(-1));
    }
}

UserMenuDialog::UserMenuDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                             tr("User Menu applet settings"),
                                                             "preferences-desktop-user") {

}
