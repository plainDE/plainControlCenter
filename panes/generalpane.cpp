#include "generalpane.h"

void GeneralPane::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QCheckBox* enableAnimationCheckBox = new QCheckBox("Enable Animation");
    enableAnimationCheckBox->setFont(mFont);
    contentsLayout->addWidget(enableAnimationCheckBox);

    QVBoxLayout* triangularTabsLayout = new QVBoxLayout();

    QCheckBox* useTriangularTabsCheckBox = new QCheckBox("Use Triangular Tabs");
    useTriangularTabsCheckBox->setFont(mFont);
    triangularTabsLayout->addWidget(useTriangularTabsCheckBox);

    QLabel* samplePictureLabel = new QLabel();
    triangularTabsLayout->addWidget(samplePictureLabel);

    contentsLayout->addLayout(triangularTabsLayout);

    contentsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                  QSizePolicy::Ignored,
                                                  QSizePolicy::MinimumExpanding));

    QPushButton* saveButton = new QPushButton(tr("Save settings"));
    saveButton->setFont(mFont);
    contentsLayout->addWidget(saveButton);
    saveButton->setVisible(false);

    // Setting current settings
    enableAnimationCheckBox->setChecked(getConfigValue("enableAnimation").toBool());
    useTriangularTabsCheckBox->setChecked(getConfigValue("useTriangularTabs").toBool());
    if (useTriangularTabsCheckBox->isChecked()) {
        samplePictureLabel->setPixmap(QPixmap("/usr/share/plainDE/triangular-tabs.png"));
    }
    else {
        samplePictureLabel->setPixmap(QPixmap("/usr/share/plainDE/flat-tabs.png"));
    }

    // Making connections
    connect(enableAnimationCheckBox, &QCheckBox::stateChanged, saveButton, &QPushButton::show);
    connect(useTriangularTabsCheckBox, &QCheckBox::stateChanged, this, [this, saveButton,
                                                                        useTriangularTabsCheckBox,
                                                                        samplePictureLabel]() {
        saveButton->setVisible(true);

        if (useTriangularTabsCheckBox->isChecked()) {
            samplePictureLabel->setPixmap(QPixmap("/usr/share/plainDE/triangular-tabs.png"));
        }
        else {
            samplePictureLabel->setPixmap(QPixmap("/usr/share/plainDE/flat-tabs.png"));
        }
    });
    connect(saveButton, &QPushButton::clicked, this, [this,
                                                      enableAnimationCheckBox,
                                                      useTriangularTabsCheckBox,
                                                      saveButton]() {
        prepareToSave(enableAnimationCheckBox,
                      useTriangularTabsCheckBox);
        saveConfig();
        saveButton->setVisible(false);
    });

    finalizeUI();
}

void GeneralPane::prepareToSave(QCheckBox* enableAnimationCheckBox,
                                QCheckBox* useTriangularTabsCheckBox) {
    setEntry("enableAnimation", QJsonValue(enableAnimationCheckBox->isChecked()));
    setEntry("useTriangularTabs", QJsonValue(useTriangularTabsCheckBox->isChecked()));
}

GeneralPane::GeneralPane(QJsonObject* cfgObj) : Pane(nullptr,
                                                     cfgObj,
                                                     tr("General"),
                                                     "cs-general") {

}
