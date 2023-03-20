#ifndef LOCALIPV4_H
#define LOCALIPV4_H

#include "../../pane.h"
#include "../../settings.h"
#include <QNetworkInterface>

class LocalIPv4AppletPane {
public:
    void readConfig();
    void prepareUI(QListWidget* ifaceList);
    void setCurrentSettings(QListWidget* ifaceList);
    void saveSettings(QListWidget* ifaceList);
    QWidget* createUI(Settings* controlCenter);
};

#endif // LOCALIPV4_H
