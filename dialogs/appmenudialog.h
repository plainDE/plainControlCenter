#ifndef APPMENUDIALOG_H
#define APPMENUDIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QSettings>
#include <QHash>
#include <QJsonArray>

class AppMenuDialog : public Dialog {
public:
    AppMenuDialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QLineEdit* menuTitleLineEdit,
                       QLineEdit* menuIconLineEdit,
                       QSpinBox* menuIconSizeSpinBox,
                       QListWidget* favAppsListWidget);
    QIcon resolveIconNameOrPath(QString iconNameOrPath,
                                bool showUnknown);

private:
    QHash<QListWidgetItem*, QString> mEntryByItem;
};

#endif // APPMENUDIALOG_H
