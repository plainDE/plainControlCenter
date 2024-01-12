#ifndef LOCALIPV4DIALOG_H
#define LOCALIPV4DIALOG_H

#include "../dialog.h"

#include <QJsonObject>
#include <QListWidget>
#include <QNetworkInterface>
#include <QPushButton>

class LocalIPv4Dialog : public Dialog {
public:
    LocalIPv4Dialog(QJsonObject* cfgObj);
    virtual void setPaneContents() override;
    void prepareToSave(QListWidget* ifaceListWidget);
};

#endif // LOCALIPV4DIALOG_H
