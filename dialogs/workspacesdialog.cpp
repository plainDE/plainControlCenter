#include "workspacesdialog.h"

void WorkspacesDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* indicatorTypeLabel = new QLabel(tr("Indicator Type:"));
    indicatorTypeLabel->setFont(mTitleFont);
    contentsLayout->addWidget(indicatorTypeLabel);

    QRadioButton* useNumbersRadioButton = new QRadioButton(tr("Show workspaces numbers"));
    useNumbersRadioButton->setFont(mFont);
    contentsLayout->addWidget(useNumbersRadioButton);

    QRadioButton* useTitlesRadioButton = new QRadioButton(tr("Show workspaces names"));
    useTitlesRadioButton->setFont(mFont);
    contentsLayout->addWidget(useTitlesRadioButton);

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

    // Setting current settings
    if (getConfigValue("showDesktopNames").toBool()) {
        useTitlesRadioButton->setChecked(true);
    }
    else {  // Numbers
        useNumbersRadioButton->setChecked(true);
    }

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, useTitlesRadioButton]() {
        prepareToSave(useTitlesRadioButton);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void WorkspacesDialog::prepareToSave(QRadioButton* useTitlesRadioButton) {
    setEntry("showDesktopNames", useTitlesRadioButton->isChecked());
}

WorkspacesDialog::WorkspacesDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                                 tr("Workspaces applet settings"),
                                                                 "workspace-switcher-top-left") {

}
