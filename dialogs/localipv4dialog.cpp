#include "localipv4dialog.h"

void LocalIPv4Dialog::setPaneContents() {
    mContentsWidget = new QWidget();
    QVBoxLayout* contentsLayout = new QVBoxLayout(mContentsWidget);
    mContentsWidget->setObjectName("innerPane");

    // Adding widgets
    QLabel* networkIfaceLabel = new QLabel(tr("Choose network interface:"));
    networkIfaceLabel->setFont(mTitleFont);
    contentsLayout->addWidget(networkIfaceLabel);

    QListWidget* ifaceListWidget = new QListWidget();
    ifaceListWidget->setFont(mFont);
    QString style = QString("QListView::item:selected { "
                            "background-color: %1; color: #ffffff; "
                            "};").arg(getConfigValue("accent").toString());
    ifaceListWidget->setStyleSheet(style);
    contentsLayout->addWidget(ifaceListWidget);

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

    // Misc
    QList<QNetworkInterface> availableIfaces = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface currentIface, availableIfaces) {
        ifaceListWidget->addItem(currentIface.name());
    }

    // Setting current settings
    QString iface = getConfigValue("ipIfname").toString();
    for (int i = 0; i < ifaceListWidget->count(); ++i) {
        if (!ifaceListWidget->item(i)->text().compare(iface)) {
            ifaceListWidget->setCurrentRow(i);
            break;
        }
    }

    // Making connections
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        this->hide();
        delete this;
    });

    connect(okButton, &QPushButton::clicked, this, [this, ifaceListWidget]() {
        prepareToSave(ifaceListWidget);
        saveConfig();
        this->hide();
        delete this;
    });

    finalizeUI();
    setWindowGeometry();
    setTransparency(this);
}

void LocalIPv4Dialog::prepareToSave(QListWidget* ifaceListWidget) {
    if (!ifaceListWidget->selectedItems().isEmpty()) {
        QString ifname = ifaceListWidget->currentItem()->text();
        setEntry("ipIfname", ifname);
    }
}

LocalIPv4Dialog::LocalIPv4Dialog(QJsonObject* cfgObj) : Dialog(cfgObj,
                                                               tr("Local IPv4 Address applet settings"),
                                                               "cs-network") {

}
