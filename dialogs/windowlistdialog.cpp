#include "windowlistdialog.h"

void WindowListDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QHBoxLayout* iconSizeLayout = new QHBoxLayout();

    QLabel* iconSizeLabel = new QLabel(tr("Icon Size:"));
    iconSizeLabel->setFont(mTitleFont);
    iconSizeLayout->addWidget(iconSizeLabel);

    QSpinBox* iconSizeSpinBox = new QSpinBox();
    iconSizeSpinBox->setMinimum(0);
    iconSizeSpinBox->setMaximum(256);
    iconSizeSpinBox->setFont(mFont);
    iconSizeLayout->addWidget(iconSizeSpinBox);

    contentsLayout->addLayout(iconSizeLayout);

    QCheckBox* showTitlesCheckBox = new QCheckBox(tr("Show Window Titles"));
    showTitlesCheckBox->setFont(mTitleFont);
    contentsLayout->addWidget(showTitlesCheckBox);

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
    iconSizeSpinBox->setValue(getConfigValue("winListIconSize").toInt());
    showTitlesCheckBox->setChecked(getConfigValue("winListShowTitles").toBool());

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, iconSizeSpinBox,
                                                    showTitlesCheckBox]() {
        prepareToSave(iconSizeSpinBox, showTitlesCheckBox);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void WindowListDialog::prepareToSave(QSpinBox* iconSizeSpinBox,
                                     QCheckBox* showTitlesCheckBox) {
    setEntry("winListIconSize", iconSizeSpinBox->value());
    setEntry("winListShowTitles", showTitlesCheckBox->isChecked());
}

WindowListDialog::WindowListDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                                 tr("Window List applet settings"),
                                                                 "preferences-system-windows") {

}
