#ifndef ADDAUTOSTARTENTRYDIALOG_H
#define ADDAUTOSTARTENTRYDIALOG_H

#include "../dialog.h"

#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QScreen>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>

#include "../panes/autostartpane.h"

class AddAutostartEntryDialog : public Dialog {
public:
    AddAutostartEntryDialog(QJsonObject* cfgObj,
                            AutostartPane* parent);
    virtual void setPaneContents() override;

private:
    QHash<QListWidgetItem*, QString> mEntryByItem;
    AutostartPane* mParent;
};

#endif // ADDAUTOSTARTENTRYDIALOG_H
