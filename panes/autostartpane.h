#ifndef AUTOSTARTPANE_H
#define AUTOSTARTPANE_H

#include "../pane.h"

#include <QListWidget>
#include <QPushButton>
#include <QJsonArray>
#include <QSettings>
#include <QFile>

class AutostartPane : public Pane {
public:
    AutostartPane(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void addEntry(QString entry);

private:
    QListWidget* mAutostartListWidget;
    QHash<QListWidgetItem*, QString> mEntryByItem;
};

#endif // AUTOSTARTPANE_H
