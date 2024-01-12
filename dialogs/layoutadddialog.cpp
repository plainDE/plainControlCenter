#include "layoutadddialog.h"

void LayoutAddDialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* selectLayoutLabel = new QLabel(tr("Select layout from the list:"));
    selectLayoutLabel->setFont(mTitleFont);
    contentsLayout->addWidget(selectLayoutLabel);

    QListWidget* layoutsListWidget = new QListWidget();
    layoutsListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    layoutsListWidget->setStyleSheet(style);
    contentsLayout->addWidget(layoutsListWidget);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    buttonsLayout->addSpacerItem(new QSpacerItem(0, 0,
                                                 QSizePolicy::MinimumExpanding,
                                                 QSizePolicy::Ignored));

    QPushButton* cancelButton = new QPushButton(tr("Cancel"));
    cancelButton->setFont(mFont);
    buttonsLayout->addWidget(cancelButton);

    QPushButton* addButton = new QPushButton(tr("Add"));
    addButton->setFont(mFont);
    buttonsLayout->addWidget(addButton);

    contentsLayout->addLayout(buttonsLayout);


    // Misc
    QFile layoutsFile("/usr/share/plainDE/layouts.json");
    layoutsFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString layoutsData = layoutsFile.readAll();
    layoutsFile.close();
    mLayouts = QJsonDocument::fromJson(layoutsData.toUtf8()).object().toVariantMap();

    foreach (QVariant layout, mLayouts.keys()) {
        QString flagPath = QString("/usr/share/flags/%1.png").arg(mLayouts[layout.toString()].toString());
        QListWidgetItem* item = new QListWidgetItem(QIcon(flagPath), layout.toString());
        layoutsListWidget->addItem(item);
    }


    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(addButton, &QPushButton::clicked, this, [this, layoutsListWidget]() {
        if (!layoutsListWidget->selectedItems().isEmpty()) {
            foreach (QListWidgetItem* selItem, layoutsListWidget->selectedItems()) {
                QString layoutCode = mLayouts[selItem->text()].toString();
                QString flagPath = QString("/usr/share/flags/%1.png").arg(layoutCode);
                QListWidgetItem* item = new QListWidgetItem(selItem->text());
                item->setIcon(QIcon(flagPath));
                mActiveLayoutsListWidget->addItem(item);
            }
            this->hide();
            delete this;
        }
    });


    finalizeUI();

    setWindowGeometry();
    if (getConfigValue("theme").toString().contains("transparent")) {
        setTransparency(this);
    }
}

LayoutAddDialog::LayoutAddDialog(QJsonObject* cfgObj,
                                 QListWidget* activeLayoutsListWidget) : Dialog(cfgObj,
                                                                                tr("Layout Selection"),
                                                                                "cs-keyboard") {
    mActiveLayoutsListWidget = activeLayoutsListWidget;
}
