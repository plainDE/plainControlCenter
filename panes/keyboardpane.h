#ifndef KEYBOARDPANE_H
#define KEYBOARDPANE_H

#include "../pane.h"

#include <QListWidget>
#include <QPushButton>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>

#include "../dialogs/layoutadddialog.h"

class KeyboardPane : public Pane {
public:
    KeyboardPane(QJsonObject* cfgObj);
    void setPaneContents();
    void prepareToSave(QListWidget* activeLayoutsListWidget,
                       QListWidget* toggleMethodsListWidget);

private:
    QVariantMap mLayoutCodes;
    QVariantMap mToggleMethods;
};

#endif // KEYBOARDPANE_H
