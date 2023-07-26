#ifndef WORKSPACESAPPLETPANE_H
#define WORKSPACESAPPLETPANE_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>

#include "../../settings.h"
#include "../../pane.h"

class WorkspacesAppletPane {
public:
    void readConfig();
    void setCurrentSettings(QCheckBox* showDesktopNamesCheckBox);
    void saveSettings(QCheckBox* showDesktopNamesCheckBox);
    QWidget* createUI(Settings* controlCenter);
};

#endif // WORKSPACESAPPLETPANE_H
