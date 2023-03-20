#include "layoutdialog.h"
#include "keyboard.h"

QJsonObject layoutDialogConfig;
QJsonObject layoutList;


void LayoutDialog::readConfig() {
    QString homeDirectory = getenv("HOME");
    QFile file;
    QString data;

    file.setFileName(homeDirectory + "/.config/plainDE/config.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    layoutDialogConfig = QJsonDocument::fromJson(data.toUtf8()).object();
}

QWidget* LayoutDialog::createUI(KeyboardPane* kbPane,
                                QListWidget* activeLayoutsListWidget) {
    readConfig();

    // Read layouts
    QFile file;
    QString data;

    file.setFileName("/usr/share/plainDE/layouts.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    layoutList = QJsonDocument::fromJson(data.toUtf8()).object();


    // UI
    this->setObjectName("layoutDialog");
    this->setWindowTitle("Select layouts");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    this->setLayout(layout);


    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + layoutDialogConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    this->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(layoutDialogConfig["fontFamily"].toString());
    paneFont.setPointSize(layoutDialogConfig["fontSize"].toInt());
    this->setFont(paneFont);

    short width = 350, height = 300;
    this->setGeometry(250, 250, width, height);

    QListWidget* availableLayoutList = new QListWidget;
    availableLayoutList->setStyleSheet("QListView::item:selected { background-color: " + \
                                       layoutDialogConfig["accent"].toString() + \
                                       "; color: #ffffff };");
    availableLayoutList->setSelectionMode(QListView::SelectionMode::ExtendedSelection);
    this->layout()->addWidget(availableLayoutList);
    availableLayoutList->clear();
    foreach (QString currentLayout, layoutList.keys()) {
        availableLayoutList->addItem(currentLayout);
    }

    QPushButton* addLayoutsPushButton = new QPushButton("Add layouts");
    this->layout()->addWidget(addLayoutsPushButton);
    this->connect(addLayoutsPushButton, &QPushButton::clicked, this,
                          [availableLayoutList, kbPane, activeLayoutsListWidget]() {
        if (!availableLayoutList->selectedItems().isEmpty()) {
            for (qint8 i = 0; i < availableLayoutList->selectedItems().length(); ++i) {
                kbPane->addLayout(availableLayoutList->selectedItems()[i]->text(),
                                  activeLayoutsListWidget);
            }
        }
    });

    return this;
}

LayoutDialog::LayoutDialog(QWidget* parent) : QWidget(parent) {

}

LayoutDialog::~LayoutDialog() {

}
