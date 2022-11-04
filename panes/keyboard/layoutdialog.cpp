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
    QWidget* layoutDialog = new QWidget;
    layoutDialog->setObjectName("layoutDialog");
    layoutDialog->setWindowTitle("Select layouts");
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(4, 4, 4, 4);
    layoutDialog->setLayout(layout);


    // Theme
    QFile stylesheetReader("/usr/share/plainDE/styles/" + layoutDialogConfig["theme"].toString());
    stylesheetReader.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream styleSheet(&stylesheetReader);
    layoutDialog->setStyleSheet(styleSheet.readAll());
    stylesheetReader.close();

    QFont paneFont;
    paneFont.setFamily(layoutDialogConfig["fontFamily"].toString());
    paneFont.setPointSize(layoutDialogConfig["fontSize"].toInt());
    layoutDialog->setFont(paneFont);

    short width = 350, height = 300;
    layoutDialog->setGeometry(250, 250, width, height);

    QListWidget* availableLayoutList = new QListWidget;
    availableLayoutList->setSelectionMode(QListView::SelectionMode::ExtendedSelection);
    layoutDialog->layout()->addWidget(availableLayoutList);
    availableLayoutList->clear();
    foreach (QString currentLayout, layoutList.keys()) {
        availableLayoutList->addItem(currentLayout);
    }

    QPushButton* addLayoutsPushButton = new QPushButton("Add layouts");
    layoutDialog->layout()->addWidget(addLayoutsPushButton);
    layoutDialog->connect(addLayoutsPushButton, &QPushButton::clicked, layoutDialog,
                          [availableLayoutList, kbPane, activeLayoutsListWidget]() {
        if (!availableLayoutList->selectedItems().isEmpty()) {
            for (qint8 i = 0; i < availableLayoutList->selectedItems().length(); ++i) {
                kbPane->addLayout(availableLayoutList->selectedItems()[i]->text(),
                                  activeLayoutsListWidget);
            }
        }
    });


    return layoutDialog;
}

LayoutDialog::~LayoutDialog() {

}
