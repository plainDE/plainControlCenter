#ifndef LAUNCHERDIALOG_H
#define LAUNCHERDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QRadioButton>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QHash>
#include <QJsonArray>

struct DesktopEntry {
    QString name;
    QIcon icon;
};

class LauncherDialog : public Dialog {
public:
    LauncherDialog(QJsonObject* cfgObj, int panelID, bool newApplet, int item, QListWidget* appletsListWidget);
    virtual void setPaneContents() override;
    void prepareToSave(QRadioButton* appRadioButton, QListWidget* entriesListWidget,
                       QLineEdit* pathLineEdit, QLineEdit* iconLineEdit, QLineEdit* titleLineEdit);
    QIcon resolveIconNameOrPath(QString iconNameOrPath);
    DesktopEntry readDesktopEntry(QString path);


private:
    QHash<QListWidgetItem*, QString> mEntryByItem;
    int mPanelID;
    bool mNewApplet;
    int mItem;
    QListWidget* mAppletsListWidget;
};

#endif // LAUNCHERDIALOG_H
