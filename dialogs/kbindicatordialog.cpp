#include "kbindicatordialog.h"

void KbIndicatorDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* indicatorTypeLabel = new QLabel(tr("Indicator Type:"));
    indicatorTypeLabel->setFont(mTitleFont);
    contentsLayout->addWidget(indicatorTypeLabel);

    QRadioButton* flagRadioButton = new QRadioButton(tr("Flag"));
    flagRadioButton->setFont(mFont);
    contentsLayout->addWidget(flagRadioButton);

    QRadioButton* isoCodeRadioButton = new QRadioButton(tr("ISO code"));
    isoCodeRadioButton->setFont(mFont);
    contentsLayout->addWidget(isoCodeRadioButton);

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
    if (getConfigValue("useCountryFlag").toBool()) {
        flagRadioButton->setChecked(true);
    }
    else {  // ISO Code
        isoCodeRadioButton->setChecked(true);
    }

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, flagRadioButton]() {
        prepareToSave(flagRadioButton);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void KbIndicatorDialog::prepareToSave(QRadioButton* flagRadioButton) {
    setEntry("useCountryFlag", flagRadioButton->isChecked());
}

KbIndicatorDialog::KbIndicatorDialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                                   tr("Keyboard Layout applet settings"),
                                                                   "cs-keyboard") {

}
